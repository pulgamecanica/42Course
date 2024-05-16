//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __SINGLETON_HPP__
# define __SINGLETON_HPP__

#include <iostream>
#include <vector>
#include <mutex>
#include <algorithm>
#include <typeinfo>

#include "School.inc"

/**
 * The Singleton class defines the `GetInstance` method that serves as an
 * alternative to constructor and lets clients access the same instance of this
 * class over and over from any thread.
 */
// T: Objects to Store, S: Singleton Class
template<typename T, typename S>
class Singleton {
    public:
        // Singletons have no copy-constructor or assignment operators
        Singleton(const Singleton &) = delete;
        Singleton(Singleton &cl) = delete;
        void operator=(const Singleton &) = delete;
        void operator=(Singleton &) = delete;
        Singleton(): class_name_(typeid(S).name()) {
            ;
        }
        virtual ~Singleton() {
            if (p_instance_ != nullptr)
                delete p_instance_;
            items_.clear();
            p_instance_ = nullptr;
        }
        // // GetInstance singleton method, LETS Fucking GOOOO!
        static S *GetInstance() {
            std::lock_guard<std::mutex> lock(mutex_);
            if (p_instance_ == nullptr)
                p_instance_ = new S();
            return p_instance_;
        }
        // Missing operator[]
        T & operator[](int index) {
            return *(items_.at(index));
        }
        bool has(T *c) {
            return std::find(items_.begin(), items_.end(), c) !=  items_.end();   
        }
        void add(T *c) {
            if (!has(c)) {
                items_.push_back(c);
                if (DEBUG)
                    std::cout << YELLOW << "[Singleton:" << class_name_ << "] " << GREEN << "ADD\t" << ENDC << *c << std::endl;
            } else if (DEBUG)
                std::cout << YELLOW << "[Singleton:" << class_name_ << "] " << RED << "ADD FAIL\t" << ENDC << *c << " is in the list" << std::endl;
        }
        void remove(T *c) {
            typename std::vector<T *>::iterator it = std::find(items_.begin(), items_.end(), c);
            
            if (it != items_.end()) {
                items_.erase(it);
                if (DEBUG) {
                    std::cout << YELLOW << "[Singleton:" << class_name_ << "] " << GREEN << "REMOVE\t" << ENDC << *c << std::endl;
                }
            } else if (DEBUG) {
                std::cout << YELLOW << "[Singleton:" << class_name_ << "] " << RED << "REMOVE FAIL\t" << ENDC << *c << " is not in the list" << std::endl;
            }
        }
        size_t size() const {
            return items_.size();
        }
        template<typename P>
        T * get() {
            for (typename std::vector<T *>::iterator i = items_.begin(); i != items_.end(); ++i) {
                if (dynamic_cast<P>(*i))
                    return *i;
            }
            return nullptr;
        }
    protected:
        std::vector<T *> items_;
    private:
        std::string class_name_;
        static std::mutex mutex_;
        static S * p_instance_ ;
};
// std::ostream& operator<<(std::ostream&, const Singleton&);

template<typename T, typename S>
std::mutex Singleton<T, S>::mutex_{};

template<typename T, typename S>
S *Singleton<T, S>::p_instance_{nullptr};

#endif
