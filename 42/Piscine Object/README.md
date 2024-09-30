# Piscine Object C++

Welcome to the **Piscine Object**, a comprehensive and immersive journey through advanced C++ programming and software design principles. This project is a compilation of progressive modules, each designed to deepen your understanding of object-oriented programming, UML modeling, and design patterns. By the end of this piscine, you'll have honed your skills in writing clean, efficient, and scalable C++ code while mastering the art of software architecture.

---

## Table of Contents

- [Project Overview](#project-overview)
- [Modules](#modules)
  - [Module 00: Encapsulation](#module-00-encapsulation)
  - [Module 01: Relationships](#module-01-relationships)
  - [Module 02: UML Modeling](#module-02-uml-modeling)
  - [Module 03: SOLID Principles](#module-03-solid-principles)
  - [Module 04: Design Patterns](#module-04-design-patterns)
  - [Module 05: CppOnRails](#module-05-cpponrails)
- [Conclusion](#conclusion)
- [Resources](#resources)

---

## Project Overview

This piscine is structured as a series of hands-on modules that progressively introduce complex concepts in C++ and software design. Each module builds upon the previous ones, ensuring a cohesive learning experience that bridges theory with practical application. You'll engage with real-world scenarios, tackle challenging exercises, and develop robust solutions that adhere to industry best practices.

---

## Modules

### Module 00: Encapsulation

**Objective:** Understand and implement the principle of encapsulation in C++.

**Overview:**

Encapsulation is a core tenet of object-oriented programming that involves bundling data and methods that operate on that data within a single unit, typically a class. In this module, you'll:

- Implement classes with private data members.
- Provide public getter and setter methods to control access.
- Ensure data integrity by preventing unauthorized modifications.

**Key Takeaways:**

- Mastery of access specifiers (`private`, `protected`, `public`).
- Ability to design classes that hide internal implementation details.
- Understanding the importance of interfaces in class design.

---

### Module 01: Relationships

**Objective:** Explore the different types of relationships between classes in C++.

**Overview:**

Relationships between classes are crucial for modeling complex systems. This module delves into:

- **Composition and Aggregation:** Understanding whole-part relationships and object lifecycles.
- **Inheritance:** Implementing base and derived classes to promote code reusability.
- **Associations:** Managing references between objects without implying ownership.

**Highlights:**

- Design a system of tools (`Hammer`, `Shovel`) and workers who use them.
- Implement rules governing ownership, sharing, and transfer of tools.
- Utilize polymorphism to handle different tool types seamlessly.

**Challenges:**

- Enforce constraints such as a worker not owning the same tool twice.
- Manage ownership transfers with appropriate error handling and warnings.
- Design workshops that filter workers based on the tools they possess.

---

### Module 02: UML Modeling

**Objective:** Use UML diagrams to model complex systems before coding.

**Overview:**

Unified Modeling Language (UML) is a standardized way to visualize system architecture. In this module, you'll:

- Create detailed UML class diagrams for a car system.
- Develop sequence diagrams illustrating interactions during key operations like accelerating, braking, steering, and gear changing.

**Activities:**

- Model components such as `Engine`, `Transmission`, `Wheel`, and `Brake`.
- Define relationships and interactions between these components.
- Use tools like Umbrello UML Modeller for diagram creation.

**Visual Aids:**

- Include class diagrams and sequence diagrams in your documentation.
- Utilize UML notation for clarity and standardization.

---

### Module 03: SOLID Principles

**Objective:** Learn and apply the SOLID principles to your C++ codebase.

**Overview:**

SOLID is an acronym representing five design principles that make software designs more understandable, flexible, and maintainable.

**Principles Covered:**

1. **Single Responsibility Principle (SRP):** A class should have only one reason to change.
2. **Open/Closed Principle (OCP):** Classes should be open for extension but closed for modification.
3. **Liskov Substitution Principle (LSP):** Objects should be replaceable with instances of their subtypes without altering correctness.
4. **Interface Segregation Principle (ISP):** Clients should not be forced to depend on interfaces they do not use.
5. **Dependency Inversion Principle (DIP):** Depend upon abstractions, not concretions.

**Exercises:**

- Refactor existing code to adhere to these principles.
- Identify violations of SOLID principles and correct them.
- Discuss the impact of these principles on software design.

---

### Module 04: Design Patterns

**Objective:** Implement common design patterns to solve recurring software design problems.

**Overview:**

Design patterns are proven solutions to common problems in software design. This module focuses on:

- **Creational Patterns:** Singleton, Factory, Builder.
- **Structural Patterns:** Adapter, Composite, Facade.
- **Behavioral Patterns:** Observer, Strategy, Command.

**Project - In Praise of Design:**

- Design a complex school system with classes like `Room`, `Course`, `Person`, `Student`, `Professor`, `Secretary`, and `Headmaster`.
- Apply design patterns to manage interactions and relationships.
- Create UML diagrams to visualize the architecture.

**Key Concepts:**

- Understand when and how to use each design pattern.
- Recognize patterns in existing code and refactor accordingly.
- Balance code readability with complexity and scalability.

---

### Module 05: CppOnRails

**Objective:** Develop a C++ application simulating train schedules and railway systems with robust error handling.

**Overview:**

CppOnRails is a comprehensive application that brings together the concepts learned in previous modules into a real-world project.

**Features:**

- Parse railway system elements (nodes, rails, events) from an elements file.
- Parse multiple train schedule files.
- Calculate optimal train travel times considering acceleration, deceleration, and maximum speed.
- Implement detailed error reporting with precise location information.
- Utilize multithreading for simulation and GUI updates.

**Technical Details:**

- **Parsing:** Read and validate complex input files with strict formats.
- **Error Handling:** Provide informative error messages with file, line, and column references.
- **Simulation:** Model train movements, events, and scheduling in a dynamic environment.
- **Optimization:** Calculate least travel times using physics formulas for acceleration and deceleration.

**Example Components:**

- **Elements File:** Defines nodes (stations), rails (connections), and events (like accidents or delays).
- **Schedule Files:** Contain train schedules, including departure times and routes.
- **Calculations:** Use kinematic equations to determine travel times and peak speeds.

**Challenges Addressed:**

- Managing bidirectional rails and complex network topologies.
- Handling events that affect train schedules and routes.
- Ensuring thread safety and performance in a multithreaded application.

---

## Conclusion

Embarking on this piscine project will not only enhance your proficiency in C++ but also equip you with a robust understanding of software design principles and patterns. By integrating theory with hands-on practice, you'll be prepared to tackle complex programming challenges and contribute effectively to any software development team.

---

## Resources

- **Umbrello UML Modeller:** [Umbrello Documentation](https://docs.kde.org/stable5/en/umbrello/umbrello/introduction.html)
- **SOLID Principles Explained:** [SOLID Principles](https://en.wikipedia.org/wiki/SOLID)
- **Design Patterns Catalog:** [Refactoring Guru](https://refactoring.guru/design-patterns/catalog)
- **C++17 Features:** [C++17 Overview](https://www.modernescpp.com/index.php/c-17-the-complete-guide)
- **Multithreading in C++:** [C++ Multithreading](https://www.cplusplus.com/reference/thread/thread/)
- **Physics for Game Developers:** Kinematic equations and motion formulas.
- **Train Stations Scrapper:** [EU Train Stations](https://github.com/trainline-eu/stations)

---

Feel free to explore each module in detail, and don't hesitate to dive deep into the additional resources provided. Happy coding!