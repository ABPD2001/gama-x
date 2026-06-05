# Directive set

_Directives_ are special instructions used to communicate directly with the **Assembler**. Unlike standard operational instructions, directives are used to control how the application assembles the code and manages the build process.

> _Note:_ Current Status: The set of available directives in **Gama-X** is currently limited. Many advanced features are still under active development and will be expanded in future releases.

## Quick menu

- **[Quick menu](#quick-menu)**
- **[The Build Process: Linker & Assembler](#the-build-process-linker--assembler)**
  - **[The Build Process: Linker & Assembler](#the-build-process-linker--assembler)**
  - **[The Linker](#1-the-linker)**
  - **[The Assembler](#2-the-assembler)**
  - **[Project Workflow](#project-workflow)**
- **[Directives table](#directives-table)**
  - **[Assembler Directives](#assembler-directives)**
  - **[Linker Directives](#linker-directives)**
    - **[`.module`](#module)**
- **[Next step](#next-step)**
- **[Author](#author)**

## The Build Process: Linker & Assembler

Compiling a complete program, especially for large-scale projects, requires two essential components: the Linker and the Assembler.

### 1. The Linker

The Linker is responsible for **managing libraries**, **resolving dependencies**, and **merging** multiple source files and modules into a single, unified file.

### 2. The Assembler

The Assembler takes that single unified file and processes it to generate the final executable output.

### Project Workflow

In a typical large-scale project, the workflow follows these steps:

1. **Linking Phase:** The Linker bundles all external libraries and project files into one consolidated source.

2. **Assembly Phase:** The Assembler converts this consolidated source into a processable, executable program.

## Directives table

there is directives table:

| Directive  | Syntax                    | Description                                                                                                                                          | Limit                                                                                        |
| :--------- | :------------------------ | :--------------------------------------------------------------------------------------------------------------------------------------------------- | :------------------------------------------------------------------------------------------- |
| `.main`    | `.main <label>`           | Used to specify the program's execution starting point.                                                                                              | only _labels_ are accepted.                                                                  |
| `.module`  | `.module`                 | Used to declare this is a dependent code snippet, or in other words, it doesnt have main point to start. point.                                      | no limitation, just for more reliability dont call it more than once.                        |
| `.replace` | `.replace <name> <value>` | Used to replace a name (_macro_) with a specific value throughout the code.                                                                          | no limitation, anything accepted but space betwen _directive_ and _arguments_ are sensetive. |
| `.include` | `.include "<path>"`       | It links a code snippet from another file into the program during compilation.                                                                       | path must be a valid file.                                                                   |
| `.import`  | `.import <library>`       | imports a library from specefied user interface defined repository, then links it to application at compile time.                                    | library must be a valid library based on specefied repository.                               |
| `.argular` | `.argular "<path>"`       | Read a file-based argument and parse it. It is used together with its corresponding instruction [`argint`]("/") to read argument values from a file. | the path must be a valid argument based file.                                                |
| `.extern`  | `.extern "<path>" <name>` | Read contents of a file. It is used together with its corresponding instruction [`exint`]("/") to read numeric value of file (by using its name).    | the path must be a numeric-value based file and name must be unique.                         |
| `.end`     | `.end`                    | Used to signal the end of directive processing.                                                                                                      | no limitation, just its better to not call it more than once.                                |

```armasm
@ Examples

.replace test1 #1234
@ replace every "test1" word to "#1234".

.import "mylib"
@ imports library 'mylib' at compile time.

.include "./my-file.s"
@ includes './my-file.s' file content at compile time.

.extern "./extern-file.txt" extern1
@ reads './extern-file.txt' file content as a numeric file, its usable by 'extern1' name and 'exint' instruction.

.argint "./arg-file.txt"
@ reads './arg-file.txt' file content and parse it to argument based values, its usable with 'argint' instruction.

.main _start
@ set start point to "_start" label.

.end
@ end of directive instructions (no directive after this instruction allowed).

_start:
  @ body of label.
```

---

## Next step

## Author

by ABPD2001 (_Abolfazl Pouretemadi_)
