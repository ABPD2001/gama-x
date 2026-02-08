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

there is two type of directives:

- _assembler:_ direct access to assembling process.
- _linker:_ direct access to linking process.

### Assembler Directives

| Directive    | Syntax                    | Description                                                                                                                | Limit                                                                                        |
| :----------- | :------------------------ | :------------------------------------------------------------------------------------------------------------------------- | :------------------------------------------------------------------------------------------- |
| `.main`      | `.main <label>`           | Used to specify the program's execution starting point.                                                                    | only _labels_ are accepted                                                                   |
| `.replace`   | `.replace <name> <value>` | Used to replace a name (_macro_) with a specific value throughout the code.                                                | no limitation, anything accepted but space betwen _directive_ and _arguments_ are sensetive. |
| `.transpile` | `.transpile`              | This instruction is used whenever you need to write the current values of the output registers into the final output file. | no limitation.                                                                               |
| `.end`       | `.end`                    | Used to signal the end of directive processing.                                                                            | no limitation, just its better to not call it more than once.                                |

```armasm
@ Example

.replace test1 #1234
@ replace every "test1" word to "#1234".
.main _start
@ set start point to "_start" label.
.end
@ end of directive instructions (no directive after this instruction allowed).

_start:
    mov x,#10
    mov y,#0
    .transpile
    @ store x and y current value into output file.
    end
```

> _Important:_ mean of output registers are $x$/$y$/$t$ registers, also, its important to konw you (as a developer) can develop your own _Gama-X_ variant, like changing name of output registers or anything else, but, we use $x$/$y$/$t$ as default output registers.

### Linker Directives

there is only on directive for linker and its `.module`.

#### `.module`

This directive informs the Linker that the file has no entry point and must be treated strictly as a library or side-module.

It prevents the linker from searching for a starting address, marking the file as a reusable component for other projects.

> _Note:_ `.module` doesnt accept any argument, for usage, just call it before `.end` directive.

but, _assembler_ directives affect on linker process.

| Directive | Effect                                                                                 | Description                                                                                                                               |
| :-------- | :------------------------------------------------------------------------------------- | :---------------------------------------------------------------------------------------------------------------------------------------- |
| `.main`   | Declares to _linker_ this file is a main executable file.                              | Acts as the inverse of `.module`, marking the file as the primary executable.                                                             |
| `.end`    | Required for the _linker_ to distinguish directive sections from the rest of the file. | if omitted during the _linking phase_, the Linker will exhibit undefined behavior, failing to correctly identify or merge the directives. |

```armasm
@ Examples


.module
@ declare file as a non-executable file.
.end
@ .end is required for linker.

@ <---- OR ---->

.main _start
@ decalre file as a primary executable file.
.end
@ .end is required for linker.
```

---

## Next step

_Documention for users is done, there is no next step._

But _developers_ interested in customization and further contributions can refer to the advanced documentation at [wiki/developers-guide.md](https://github.com/ABPD2001/gama-x/blob/main/wiki/directive-set.md).

## Author

by ABPD2001 (_Abolfazl Pouretemadi_)
