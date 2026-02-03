# CLI-SlideEngine

This is a command-line application for creating and managing presentations. I built this to practice Object-Oriented Design (OOD), Command patterns, and complex data structures in C++.

## How it works

* **The Controller**: This is the brain of the app. It runs the main loop and connects the user's input to the project data.
* **The Parser**: I wrote a custom parser that takes string commands from the terminal, tokenizes them, and validates if the command exists (like `addSlide` or `addShape`).
* **The Item System**: 
    * **Project**: Holds a collection of Slides.
    * **Slide**: Holds different Shapes (like Rectangles).
    * **Shapes**: Uses inheritance so the project can handle different types of shapes using a single `Shape` base class.
* **Command Pattern**: Every action (adding, removing, helping) is its own class, which makes the code much easier to organize and expand.

## Key Features
* Custom command-line interface (CLI).
* Support for shapes with coordinates and bounding boxes.
* Slide management (Add, Remove, Reorder).
* CSV Serialization (saving your project to a file).

## Files included
* `Controller.h / .cpp`: Handles the application flow and command library.
* `Item.h / .cpp`: Defines the shapes, slides, and project logic.
* `main.cpp`: The entry point that starts the controller.
