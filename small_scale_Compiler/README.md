# Overview
This is a custom compiler that processes:

- Arithmetic operations
- A custom inbuilt sigmoid function
- Can store the value in a variable.
- The compiler is built using make, and it utilizes NASM, GCC, and LD for assembly and linking.

# Features
- Variable Handling: Stores data in a variable to be used further.
- Arithmetic Operations: Executes standard arithmetic operations (+, -, *, /).
- Inbuilt Sigmoid Function: Provides an efficient, built-in implementation of the sigmoid function, useful for mathematical and machine learning-related tasks.

# Dependencies
Ensure the following are installed on your system:

- NASM (Netwide Assembler): Used for assembly code generation.
- GCC (GNU Compiler Collection): Used for compiling.
- LD (GNU Linker): For linking compiled object files.
- CMAKE: To build the compiler.
- Use LINUX environment(prefferd).

# Building the Compiler
- To build the compiler, go to the **compiler_CODES** folder
- Run the following in bash:      **make**

This will generate the executable files for the compiler.

# Usage
- Compile the source code:          **./compiler test.txt**
- Run the code:                     **./output**  

The compiler will process the code, perform the necessary operations, and output the results.

# Sigmoid Function Details
- The custom sigmoid function is defined as: sigmoid(x) = 1 / ( 1 + e ^ ( - x ))

- Use the sigmoid keyword in your source file to calculate the sigmoid of a given value.

# File Structure
- compiler_CODES/: Contains source code files for the compiler.
- Makefile: Automates the build process.
- README.md: Provides documentation for the compiler.

# Future Enhancements
**Planned updates include:**

- Support for additional mathematical functions.
- Enhanced error handling and debugging capabilities.
- Optimization improvements for faster computation.

# Contributing
Contributions are welcome! Submit pull requests or report issues for feature suggestions and bug fixes.
