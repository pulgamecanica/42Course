/**
 * @file Obj.hpp
 * @brief Header file for the `Obj` class, which demonstrates documentation in Markdown style.
 */

#ifndef OBJ_HPP
#define OBJ_HPP

#include <string>
#include <stdexcept>

/**
 * @class Obj
 * @brief A demonstration class with various attributes and methods.
 *
 * The `Obj` class serves as an example of a well-documented C++ class.
 * It contains several attributes, constructors, and methods to illustrate
 * the documentation needed for Doxygen and Sphinx integration.
 */
class Obj {
public:
    /**
     * @brief Default constructor for `Obj`.
     *
     * Initializes the object with default values.
     */
    Obj();

    /**
     * @brief Constructor that initializes the object with specific values.
     * 
     * @param name The name to assign to the object.
     * @param value An integer value associated with the object.
     *
     * ## Example
     * 
     * ```cpp
     * Obj myObj("Example", 42);
     * ```
     */
    Obj(const std::string &name, int value);

    /**
     * @brief Destructor for `Obj`.
     *
     * Handles any necessary cleanup for the object.
     */
    ~Obj();

    /**
     * @brief Sets the name of the object.
     * 
     * @param name The new name for the object.
     * @throws std::invalid_argument if `name` is empty.
     *
     * ## Example
     * ```cpp
     * Obj obj;
     * obj.setName("New Name");
     * ```
     */
    void setName(const std::string &name);

    /**
     * @brief Retrieves the name of the object.
     * 
     * @return The current name of the object.
     *
     * ## Example
     * ```cpp
     * Obj obj("Example", 42);
     * std::cout << obj.getName();
     * ```
     */
    std::string getName() const;

    /**
     * @brief Sets the value of the object.
     * 
     * @param value The new integer value for the object.
     *
     * ## Example
     * ```cpp
     * Obj obj;
     * obj.setValue(100);
     * ```
     */
    void setValue(int value);

    /**
     * @brief Retrieves the current value of the object.
     * 
     * @return The integer value of the object.
     *
     * ## Example
     * ```cpp
     * Obj obj("Example", 42);
     * int value = obj.getValue();
     * ```
     */
    int getValue() const;

    /**
     * @brief Displays information about the object.
     * 
     * Prints the `name` and `value` to standard output in a readable format.
     *
     * ## Example
     * ```cpp
     * Obj obj("Example", 42);
     * obj.displayInfo();
     * ```
     */
    void displayInfo() const;

private:
    std::string name_;  ///< The name of the object.
    int value_;         ///< The integer value associated with the object.
};

#endif // OBJ_HPP
