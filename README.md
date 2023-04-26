# cpp-module-resolver

Tool to determine the build order of C++-files using the C++20-Modules feature. This is useful for example for Makefiles.

## Building

Run `make`

## Usage

Just run the built binary in your project's root folder, then the tool will locate your `src/` folder and output the build order.

## Additional Notes

The algorithm used just iterates over the files, until everything has been evaluated. This could be improoved by using a graph.
