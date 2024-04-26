
---

# ASCII Art Image Generator

This program is designed to generate PNG images from ASCII art files containing lines and points. It can be run with one or two parameters, where the first parameter is the input ASCII art file, and the second parameter is the output PNG file. If run without any arguments, the program generates a random configuration.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
  - [Generating Images from ASCII Art](#generating-images-from-ascii-art)
  - [Using Random Configuration](#using-random-configuration)
- [ASCII Art File Format](#ascii-art-file-format)
- [Dependencies](#dependencies)
- [License](#license)

## Installation

To compile the program, make sure you have a C++ compiler installed on your system. Then, you can compile the program using the provided Makefile by running:

```bash
make
```

This will generate the executable file `ex01_bonus `.

## Usage

### Generating Images from ASCII Art

To generate an image from an ASCII art file, run the program with the input ASCII art file as the first parameter and the desired output PNG file as the second parameter.<br>
For example:

```bash
./ex01_bonus  input.txt output.png 
```

This command will read the ASCII art from `input.txt` and generate an image saved as `output.png`.

### Using Random Configuration

If you want to generate an image with a random configuration, simply run the program without any arguments:

```bash
./ex01_bonus 
```

This will generate an image with a random configuration and save it as `output.png`.

## ASCII Art File Format

The ASCII art file should contain lines and points, each defined on a separate line. Here's the format for defining lines and points:

- **Point**: `point x y color`
- **Line**: `line x1 y1 x2 y2 color`

Where:
- `x` and `y` are the coordinates of the point.
- `x1`, `y1`, `x2`, and `y2` are the coordinates of the line endpoints.
- `color` is the color of the point or line in the format `#RRGGBB` or `#RRGGBBAA`, where `RR`, `GG`, `BB`, and `AA` are hexadecimal values representing the red, green, blue, and alpha components of the color, respectively.

Example of an ASCII art file:

```plaintext
point 20 20 #FF0000
point 40 40 #00FF00
line 20 20 40 40 #0000FF
```

## Dependencies

This program depends on the following standard C++ libraries:
- `<iostream>`
- `<fstream>`
- `<sstream>`
- `<cstdlib>`
- `<ctime>`
- `<cstdint>`
- `<string>`
- `<vector>`
- `<algorithm>`
- `<cstdlib>`
- [lodepng](https://github.com/lvandeve/lodepng)

---

# Example:

![tux](https://github.com/pulgamecanica/42Course/assets/28810331/091912c8-875d-4991-8953-33650f2557d9)

***