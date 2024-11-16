# Pool - Summary

This section provides an overview of two important data structures used in the project:
the `Pool` class and the `DataBuffer` class. These classes are fundamental for resource 
management and object serialization within the library.

The following sections describe each class in detail, including their purpose, methods, and usage.

The `Pool` class is a **resource pool manager** that handles a collection of reusable objects.
It enables efficient memory management by pooling objects and reusing them, reducing the cost of 
repeated allocations and deallocations.

   
## Purpose

The `Pool` class is designed to pre-allocate a certain number of objects of type `TType` and allow 
for efficient acquisition and release of these objects. When an object is no longer needed, it is returned 
to the pool, thus eliminating the need to delete or reallocate memory.

This class follows the **RAII** (Resource Acquisition Is Initialization) principle, ensuring that 
objects are automatically returned to the pool when they go out of scope.

## Key Features:
- **Pre-allocation of objects**: The pool can be resized to allocate a set number of objects upfront.
- **Automatic resource management**: When an object is acquired, it is wrapped in an `Pool::Object` that automatically returns the object to the pool when it goes out of scope.
- **Efficient reuse of objects**: Objects that are no longer in use are returned to the pool for reuse.

## Methods:
- **resize**: Resizes the pool to a specific number of objects.
- **acquire**: Acquires an object from the pool. If the pool is empty, it allocates a new object.

## Example Usage:

```cpp
Pool<MyClass> myPool;

// Resize the pool to pre-allocate 10 objects
myPool.resize(10);

// Acquire an object from the pool
auto obj1 = myPool.acquire();
obj1->use();

// If the pool runs out of objects, acquire will create a new one
auto obj2 = myPool.acquire();
obj2->use();
```

For more detailed documentation, see the class definition below.


