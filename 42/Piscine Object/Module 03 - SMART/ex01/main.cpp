//***************************//
//*Template by pulgamecanica*//
//***************************//

#include <map>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <vector>

#include "ex01.inc"
#include "Command.hpp"

int main(void)
{
    srand(time(NULL));
    if (DEBUG)
        std::cout << "Debug ON!" << std::endl;
    ex01::Article a1("Teddy Bear", 20);
    ex01::Article a2("Beer", 12.42);
    ex01::Article a3("Chocolate", 1);
    ex01::Article a4("Phone", 122.42);
    ex01::Article a5("Tennis Racket" ,45.67);
    ex01::Article a6("Guitar" ,89.01);
    ex01::Article a7("Sneakers" ,43.21);
    ex01::Article a8("Watch" ,87.65);
    ex01::Article a9("Camera" ,10.11);
    ex01::Article a10("Perfume" ,65.43);
    ex01::Article a11("Umbrella" ,21.22);
    ex01::Article a12("Wallet" ,76.54);
    ex01::Article a13("Scarf" ,32.10);
    ex01::Article a14("Bicycle" ,98.76);
    ex01::Article a15("Jacket" ,54.32);
    ex01::Article a16("Smartphone" ,9.87);
    ex01::Article a17("Handbag" ,87.12);
    ex01::Article a18("Necklace" ,65.43);
    ex01::Article a19("Socks" ,43.21);
    ex01::Article a20("Hat" ,21.54);
    ex01::Article a21("T-shirt" ,43.21);
    ex01::Article a22("Jeans" ,65.43);
    ex01::Article a23("Skirt" ,87.65);
    ex01::Article a24("Dress" ,98.76);
    ex01::Article a25("Hoodie" ,10.11);
    ex01::Article a26("Toy Car" ,32.10);
    ex01::Article a27("Board Game" ,54.32);
    ex01::Article a28("Alarm Clock" ,76.54);
    ex01::Article a29("Telescope" ,21.22);
    ex01::Article a30("Cookware Set" ,65.43);
    ex01::Article a31("Tool Kit" ,10.11);
    ex01::Article a32("Painting Supplies" ,87.65);
    ex01::Article a33("Fitness Tracker" ,32.10);
    ex01::Article a34("Sleeping Bag" ,54.32);
    ex01::Article a35("Tent" ,65.43);
    ex01::Article a36("Flashlight" ,21.22);

    std::map<ex01::Article*, int> m;
    m[&a1] = rand() % 8 + 2; // Random quantity between 2 and 10
    m[&a4] = rand() % 8 + 2; // Random quantity between 2 and 10
    m[&a2] = rand() % 8 + 2; // Random quantity between 2 and 10
    m[&a3] = rand() % 8 + 2; // Random quantity between 2 and 10;
    m[&a4] = rand() % 8 + 2; // Random quantity between 2 and 10
    m[&a5] = rand() % 8 + 2; // Random quantity between 2 and 10
    m[&a6] = rand() % 8 + 2; // Random quantity between 2 and 10
    m[&a7] = rand() % 8 + 2; // Random quantity between 2 and 10
    m[&a8] = rand() % 8 + 2; // Random quantity between 2 and 10
    m[&a9] = rand() % 8 + 2; // Random quantity between 2 and 10
    m[&a10] = rand() % 8 + 2; // Random quantity between 2 and 10
    m[&a11] = rand() % 8 + 2; // Random quantity between 2 and 10
    m[&a12] = rand() % 8 + 2; // Random quantity between 2 and 10
    m[&a13] = rand() % 8 + 2; // Random quantity between 2 and 10
    m[&a14] = rand() % 8 + 2; // Random quantity between 2 and 10

    std::cout << "- - - Normal Command - - -" << std::endl;
    ex01::Command c(43, "18-Oct-2024", "Laura", m);
    std::cout << c << std::endl;

    std::cout << "- - - Thuesday Discount Command - - -" << std::endl;
    ex01::ThuesdayDiscountCommand tdc1(42, "12-Oct-2024", "Ana Marie", m);
    std::cout << tdc1 << std::endl;

    std::cout << "- - - Polymorphic Thuesday Discount Command - - -" << std::endl;
    ex01::ThuesdayDiscountCommand *tdc2 = new ex01::ThuesdayDiscountCommand(42, "10-Oct-2024", "Bill", m);
    std::cout << *tdc2 << std::endl;
    m.clear();
    delete tdc2;

    m[&a15] = rand() % 2; // Random quantity between 0 and 1
    m[&a16] = rand() % 2; // Random quantity between 0 and 1
    m[&a17] = rand() % 2; // Random quantity between 0 and 1
    m[&a18] = rand() % 2; // Random quantity between 0 and 1
    std::cout << "- - - Normal Command - - -" << std::endl;
    ex01::Command c2(43, "18-Oct-2024", "Laura", m);
    std::cout << c2 << std::endl;

    std::cout << "- - - Package Reduction Discount Command - - -" << std::endl;
    ex01::PackageReductionDiscountCommand prdc1(42, "12-Oct-2024", "Ana Marie", m);
    std::cout << prdc1 << std::endl;

    std::cout << "- - - Polymorphic Package Reduction Discount Command - - -" << std::endl;
    ex01::PackageReductionDiscountCommand *prdc2 = new ex01::PackageReductionDiscountCommand(42, "10-Oct-2024", "Bill", m);
    std::cout << *prdc2 << std::endl;
    m.clear();
    delete prdc2;
    return (0);
}
