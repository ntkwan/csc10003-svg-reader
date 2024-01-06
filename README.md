# csc10003-svg-reader
  
## Features

- Parse SVG files and render them with basic tags (Circle, Rect, Ellipse, etc).
- Support ViewBox for SVG.
- Interactivity (Zoom In/Out, Rotate, Dragging).
- Render group of shapes and their transformations.
- Render path with most of its tags.
- Support Radial/Linear gradient for shapes.

## Release

The application only runs on Windows platform. We also supports windows toolchain to run 'virtually' on Linux. However, we don't ensure its perform stably on Linux.

## Demonstration
[Here is the video demo on Youtube](https://youtu.be/PYpJSK_OGnI?si=Ip6UWNFg1aipduIO)

## Dependencies

### Application

- C++20 (any GNU versions)
- CMake 3.14
- [gdiplus](https://github.com/mono/libgdiplus)
- [rapidxml](https://rapidxml.sourceforge.net)

### Documentation

- [Doxygen](https://www.doxygen.nl/)
- [PlantUML](https://plantuml.com/)
- [hpp2plantuml](https://github.com/thibaultmarin/hpp2plantuml)

## Installation

- Clone this repository
- `cmake -S. -Bbuild` (with `-G "MinGW Makefiles"` if your system is Windows-based)
- `make -Cbuild` 
- The executable `svg-reader` in the directory will appear.


## Documentation

For the LaTeX version, refer to [`latex/refman.pdf`](https://github.com/ntkwan/csc10003-svg-reader/tree/main/latex).

For the web version, refer to [this webpage](ntkwan.github.io/csc10003-svg-reader/).
