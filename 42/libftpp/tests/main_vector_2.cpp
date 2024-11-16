#include <iostream>
#include "ivector2.hpp"

int main() {
    IVector2<int> vec1(3, 4);
    IVector2<int> vec2(1, 2);
    
    std::cout << "Vec1 : " << vec1.x << " / " << vec1.y << std::endl;
    std::cout << "Vec2 : " << vec2.x << " / " << vec2.y << std::endl;
    

    // Test operator overloads
    auto vecAdd = vec1 + vec2;
    std::cout << "vec1 + vec2 = (" << vecAdd.x << ", " << vecAdd.y << ")" << std::endl; 
    // Expected: vec1 + vec2 = (4, 6)

    auto vecSub = vec1 - vec2;
    std::cout << "vec1 - vec2 = (" << vecSub.x << ", " << vecSub.y << ")" << std::endl; 
    // Expected: vec1 - vec2 = (2, 2)

    auto vecMul = vec1 * vec2;
    std::cout << "vec1 * vec2 = (" << vecMul.x << ", " << vecMul.y << ")" << std::endl; 
    // Expected: vec1 * vec2 = (3, 8)

    auto vecDiv = vec1 / vec2;
    std::cout << "vec1 / vec2 = (" << vecDiv.x << ", " << vecDiv.y << ")" << std::endl; 
    // Expected: vec1 / vec2 = (3, 2)

    bool isEqual = vec1 == vec2;
    std::cout << "vec1 == vec2: " << (isEqual ? "true" : "false") << "" << std::endl; 
    // Expected: vec1 == vec2: false

    bool isNotEqual = vec1 != vec2;
    std::cout << "vec1 != vec2: " << (isNotEqual ? "true" : "false") << "" << std::endl; 
    // Expected: vec1 != vec2: true

    // Test additional methods
    float len = vec1.length();
    std::cout << "Length of vec1: " << len << "" << std::endl; 
    // Expected: Length of vec1: 5 (or sqrt(3*3 + 4*4))

    auto normVec = vec1.normalize();
    std::cout << "Normalized vec1 = (" << normVec.x << ", " << normVec.y << ")" << std::endl; 
    // Expected: Normalized vec1 = (0.6, 0.8)

    float dotProd = vec1.dot(vec2);
    std::cout << "Dot product of vec1 and vec2: " << dotProd << "" << std::endl; 
    // Expected: Dot product of vec1 and vec2: 11 (or 3*1 + 4*2)

    auto crossProd = vec1.cross();
    std::cout << "Cross product of vec1: (" << crossProd.x << ", " << crossProd.y << ")" << std::endl; 
    // Expected: Cross product of vec1: (some_value, some_value)

    return 0;
}

