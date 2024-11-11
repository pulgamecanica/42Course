/**
 * @file Obj.cpp
 * @brief Implementation of the Obj class.
 */

#include "Obj.hpp"
#include <iostream>

Obj::Obj() : name_("Unnamed"), value_(0) {}

Obj::Obj(const std::string &name, int value) : name_(name), value_(value) {}

Obj::~Obj() {}

void Obj::setName(const std::string &name) {
    if (name.empty()) {
        throw std::invalid_argument("Name cannot be empty.");
    }
    name_ = name;
}

std::string Obj::getName() const {
    return name_;
}

void Obj::setValue(int value) {
    value_ = value;
}

int Obj::getValue() const {
    return value_;
}

void Obj::displayInfo() const {
    std::cout << "Obj Name: " << name_ << ", Value: " << value_ << std::endl;
}

