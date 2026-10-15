# project_root# Small Language Model (slm)

A C++ command-line tool that reads a text file, infers a $k$-gram language model from it with optional pseudocount smoothing ($\alpha$), and generates new text.

## Project Structure
- `src/`: Contains source code (`main.cpp`, `Model.h`, `Model.cpp`).
- `Makefile`: Automates compilation and cleaning.

## Compilation
To compile the project and build the executable `slm`, run the following command in the root directory:
```bash
make