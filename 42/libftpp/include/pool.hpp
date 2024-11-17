/**
 * @file pool.hpp
 * @brief Defines the Pool class for managing reusable objects with automatic memory management.
 */

#ifndef POOL_HPP
#define POOL_HPP

#include <stack>
#include <vector>
#include <memory>
#include <stdexcept>

/**
 * @class Pool
 * @brief Manages a collection of reusable objects of type `TType` using resource pooling.
 *
 * Provides efficient memory management by reusing a fixed number of type `TType` pre-allocated objects.
 * If the pool is exhausted, new objects can still be allocated on demand and returned to the pool.
 *
 * @tparam TType The type of object to manage in the pool.
 */
template<typename TType>
class Pool {
public:
  /**
   * @class Object
   * @brief RAII-style wrapper for objects managed by the Pool.
   * 
   * `Pool::Object` is designed to handle the lifecycle of an acquired resource from the pool.
   * When an instance of `Object` goes out of scope, it automatically returns the object back to
   * the pool, ensuring safe and efficient resource management.
   */
  class Object {
   public:
    /**
     * @brief Constructor for `Object`, used internally by `Pool` to wrap a resource.
     * 
     * @param ptr Pointer to the `TType` object acquired from the pool.
     * @param pool Reference to the parent `Pool` instance, used to return the object on destruction.
     * 
     * ## Note:
     * 
     * Choosing Pool& pool instead of Pool* pool helps enforce safety, improves code readability,
     * and clarifies that Object always expects a valid pool instance without taking ownership,
     * perfectly aligning with RAII design principles.
     */
    Object(TType* ptr, Pool& pool) : ptr_(ptr), pool_(pool) {}

    /**
     * @brief Destructor for `Object`, automatically returns the resource to the pool.
     *
     * When the `Object` instance is destroyed, the managed object is returned to the pool
     * instead of being deallocated.
     */
    ~Object() {
      if (ptr_) {
        pool_.release(ptr_);
      }
    }

    /**
     * @brief Arrow operator overload to access the underlying `TType` object.
     * @return Pointer to the `TType` object.
     */
    TType* operator->() { return ptr_; }

   private:
    TType* ptr_;   ///< Pointer to the managed object.
    Pool& pool_;   ///< Reference to the parent pool, for returning the object on destruction.
  };

  /**
   * @brief Resizes the pool to hold the specified number of pre-allocated objects.
   *
   * This method will add or remove objects in the pool based on the given `new_size`.
   * - **If expanding**: New objects are added to the pool up to `new_size`.
   * - **If shrinking**: Objects are only removed if they are not in use.
   *
   * @param new_size The new target number of objects for the pool.
   * @throws std::invalid_argument If `new_size` is less than the number of currently in-use objects.
   * 
   * ## Example
   * ```cpp
   * Pool<TestObject> myPool;
   * myPool.resize(10);  // Pre-allocates 10 objects
   * myPool.resize(5);   // Reduces pool size to 5 if possible
   * ```
   */
  void resize(const size_t new_size) {
    size_t current_pool_size = pool_.size();
    size_t current_total_size = allocated_.size();
    size_t in_use = current_total_size - current_pool_size;

    if (new_size < in_use) {
      throw std::invalid_argument("Cannot resize pool below the number of active objects.");
    }

    if (new_size > current_total_size) {
      // Expanding the pool
      for (size_t i = current_total_size; i < new_size; ++i) {
        auto new_obj = std::make_unique<TType>();
        pool_.push(new_obj.get());
        allocated_.emplace_back(std::move(new_obj));
      }
    } else if (new_size < current_total_size) {
      // Shrinking the pool
      size_t excess = current_total_size - new_size;
      while (excess > 0 && !pool_.empty()) {
        pool_.pop();
        allocated_.pop_back();
        --excess;
      }
    }
  }

  /**
   * @brief Acquires an object from the pool, constructing it if necessary.
   * 
   * This method provides an object from the pool, or constructs one if none are available.
   * It uses perfect forwarding (`std::forward<TArgs>(args)`) to pass arguments to the constructor,
   * enabling efficient resource acquisition and initialization.
   * 
   * @tparam TArgs Variadic template parameter pack for constructor arguments.
   * @param args Constructor arguments to initialize the object.
   * @return A `Pool::Object` that manages the acquired resource.
   *
   * @throws std::runtime_error if the pool is empty and unable to provide a new object.
   *
   * ## Example
   * ```cpp
   * Pool<MyClass> pool;
   * pool.resize(5);  // Pre-allocates 5 objects
   * 
   * // Acquire an object, initializing it with a specific constructor
   * auto obj = pool.acquire(42);
   * obj->use();
   * 
   * // If all objects are in use, the method will resize the pool:
   * auto newObj = pool.acquire();
   * newObj->use();
   * ```
   */
  template<typename... TArgs>
  Object acquire(TArgs&&... args) {
    if (pool_.empty()) {
      resize(allocated_.size() + 1);
      return acquire(std::forward<TArgs>(args)...);
    } else {
      TType* ptr = pool_.top();
      pool_.pop();
      new (ptr) TType(std::forward<TArgs>(args)...);
      return Object(ptr, *this);
    }
  }

private:
  /**
   * @brief Returns an object to the pool.
   *
   * This method is called by the `Object` class's destructor when an `Object` instance
   * goes out of scope.
   *
   * @param ptr Pointer to the object being returned to the pool.
   */
  void release(TType* ptr) {
    ptr->~TType(); // Explicitly call the destructor without deallocating memory
    pool_.push(ptr);
  }

  std::stack<TType*> pool_;                       ///< Stack of available objects in the pool
  std::vector<std::unique_ptr<TType>> allocated_; ///< Storage for all allocated objects
};

#endif // POOL_HPP
