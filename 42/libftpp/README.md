# **libftpp - A C++ Library Project**

`libftpp` is a custom C++ library with extensive automated documentation generation. This project is set up with a `Makefile` that compiles the library, manages object and binary cleanup, and automates documentation generation using Doxygen and Sphinx, with Read the Docs support.

## Table of Contents
- [Project Overview](#project-overview)
- [Installation](#installation)
- [Compilation and Testing](#compilation-and-testing)
  - [Makefile Targets](#makefile-targets)
- [Documentation Generation](#documentation-generation)
  - [Doxygen + Sphinx Setup](#doxygen--sphinx-setup)
  - [Makefile Integration](#makefile-integration)
- [Cleaning Up](#cleaning-up)

---

## Project Overview

This project, `libftpp`, is designed with an emphasis on:
- Clean code practices with C++17 standards.
- Automated documentation generated from source code comments.
- Consistent build and test workflow.

## Installation

To set up the environment, you will need to have the following dependencies installed:
- **Doxygen**: for generating XML documentation from source code.
- **Sphinx**: for building documentation in various formats.
- **Read the Docs (RTD)**: to enable a web-hosted documentation view using Sphinx.

Install Doxygen and Sphinx via your package manager or using `pip` for Sphinx:

```bash
# Doxygen installation
# On Ubuntu
sudo apt update
sudo apt install doxygen

# On macOS
brew install doxygen

# Sphinx installation
pip install sphinx
pip install sphinx-rtd-theme  # Recommended for Read the Docs theme
```

## Compilation and Testing

This project includes a Makefile with multiple targets for compiling, testing, and cleaning the project.

### Makefile Targets

| Target       | Description |
|--------------|-------------|
| `make` (same as make all)   | Compiles the library and generates the `libftpp` executable. |
| `make all`   | Compiles the library and generates the `libftpp` executable. |
| `make test`  | Rebuilds the project, runs the compiled binary, and displays test output. |
| `make valgrind` | Runs the executable with `valgrind` for memory leak checking. |
| `make clean` | Cleans up all object files and documentation files. |
| `make fclean`| Performs `clean` and removes the compiled binary and XML files. |
| `make re`    | Cleans and recompiles the project from scratch. |
| `make help`  | Displays available Sphinx documentation commands. |
| `make <doc>` | Executes a Sphinx documentation build based on the specified output type (e.g., `make html`, `make latex`). |

---

## Documentation Generation

### Doxygen + Sphinx Setup

This project is set up to generate documentation in two stages:
1. **Doxygen**: Parses C++ source code comments, outputs XML data.
2. **Sphinx with Breathe**: Converts Doxygen's XML output into various documentation formats (HTML, PDF, etc.) using Sphinx.

#### Doxygen

The `Doxyfile` is configured to:
- Parse only `.cpp`, `.hpp`, and `.inc` files.
- Generate XML output in the `xml` folder, used by Sphinx/Breathe.
- Allow Markdown and render it correctly for Sphinx, although there are som \*bugs
- Disable `html` and `latex` outputs, to avoid unnecessary folders.

> \* I founded that sometimes it will not parse Markdown correctly, specially higher level titles `#### For Example`

#### Sphinx with Breathe

Sphinx is configured to read the XML output from Doxygen. The following important settings in `conf.py` allow Sphinx to render API documentation:
- **Extensions**: Includes `breathe`, `sphinx.ext.autodoc`, and `sphinx_rtd_theme`.
- **Breathe Configuration**: Specifies the project name and XML directory for Breathe to locate Doxygen files.

### Makefile Integration

The Makefile simplifies the documentation generation by integrating both the Doxygen and Sphinx steps:
- **Automatic Documentation Generation**: The Makefile contains a catch-all rule at the end, which runs `doxygen` followed by a Sphinx build command when any unknown target (e.g., `make html`) is called. This avoids the need for a separate script to build the docs.
- **Cleaning Rules**: The `clean` and `fclean` rules also clean up documentation files (`xml` and `_build` folders).

Example of generating HTML documentation with Sphinx:
```bash
make html
```
This command will:
1. Run Doxygen to generate XML files in the `xml` folder.
2. Use Sphinx to convert the XML data into HTML documentation, available in `_build/html`.

---

## Cleaning Up

To maintain a clean working directory, the Makefile offers the following cleaning targets:
- **`make clean`**: Removes all object files and temporary files generated during documentation creation.
- **`make fclean`**: Removes all generated files, including object files, the compiled library (`libftpp`), and documentation output files in `xml`.

---

## Notes

- **Valgrind Usage**: `make valgrind` allows you to run tests with Valgrind for memory analysis. You can pass arguments to `valgrind` by setting the `libftpp_ARGS` environment variable.
- **Color-Coded Output**: The Makefile includes color-coded output for better readability in the terminal.

This setup ensures a streamlined workflow for compiling, testing, and documenting `libftpp`. Documentation is fully automated with the integration of Doxygen and Sphinx, making it easy to maintain and distribute.

---

Enjoy coding with `libftpp`!