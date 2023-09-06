# sklib
Helper library for my C++ projects

## Description
This helper library is the top level project of all the `sklib` modules I have made.

### `sklib`
SkLib (skuyuy helper library) is a library containing several modules for all kinds of C++ projects.
Each module is its own CMake subproject which can either be used seperately or as a part of the sklib project

## Modules

### Rules
- A new module must serve an own purpose, prevent things like two string helper libraries
- A module has to be its own CMake project, based on some common CMake project structure
- Module namespaces can have a maximum depth of 2:
  - `sklib::os` ✔️
  - `sklib::filesystem` ✔️
  - `sklib::string::format` X
- Modules should have as little external dependencies as possible (avoid big cmake projects)