//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "CovariantTemplates.inc"
#include "Base.hpp"
#include "Derived.hpp"
#include "CovariantObj.hpp"

int main(void)
{
    {
    // Polymorphism! Awesome
    Derived d("Child 1");
    Base * base = &d;
    (void)base;
    }
    {
    // Now lets try template covariants
    CovariantObj<Derived> c("SomeCovariant");
    CovariantObj<Derived> *c_ptr = &c;

    // Now lets try to use polymorphism to convert CovariantObj<Derived> to CovariantObj<Base>
    CovariantObj<Base> c_b = c; // Illegal without covariant enabler // This is creating another object
    (void)c_b;
    // Implementation with in-between class, this does not keeps the pointer
    CovariantObj<Base> *c_b_ptr1 = &c_b; // WOW NO FUCKING WAAAYY !!!! I DID IT! BUT It's not the same pointer :(
    (void)c_b_ptr1;
    std::cout << "Original [" << &c << "] vs Converted [" << c_b_ptr1 << "]" << std::endl;

    // Polymorphic Covariant Implementation To pointer
    // CovariantObj<Base> *c_b_ptr2 = c_ptr;
    CovariantObj<Base> *c_b_ptr2 = dynamic_cast<CovariantObj<Base> *>(c_ptr);
    (void)c_b_ptr2;
    std::cout << "Original [" << &c << "] vs Converted [" << c_b_ptr2 << "]" << std::endl;

    
    }
    return (0);
}
