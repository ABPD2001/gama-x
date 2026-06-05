# Documention

To begin learning **Gama-X**, start here. This page provides a comprehensive breakdown of the language's **grammar** and **syntax**.

## Quick menu

- **[Quick menu](#quick-menu)**
- **[Prerequisites](#prerequisites)**
- **[Getting Started with Syntax](#getting-started-with-syntax)**
- **[Next step](#next-step)**
- **[Author](#author)**

## Prerequisites

Before getting started, please note that working with this intermediate language requires a basic understanding of the following concepts:

- **Assembly Fundamentals:** Basic knowledge of assembly language principles

- **Programming Concepts:** General familiarity with core programming logic

- **System Drivers:** Understanding how driver(s) operate and interface with the target system

## Getting Started with Syntax

This language directly inherits its syntax from **Assembly**. To ensure maximum readability, the syntax has been designed to closely resemble **ARM Assembly**, which is widely regarded as one of the most intuitive and straightforward assembly languages available.

In ARM Assembly, the syntax looks like this:

```armasm
.command_x arg_m
.command_y arg_n
.command_z
@ this is a comment

label_x:
    op1 arg1, arg2
    opX Rd, argY, argZ
```

and Gama-X uses this syntax directly as its syntax.

> _Note:_ also in _ARM Assembly_, registers are named `r0 ~ r15` (in **armv7**) and its same with _Gama-X_ `r0 ~ r5` (by default configurations).

> _Note:_ All general-purpose and special registers are customizable (including names and initial values) via the language’s user interface, without requiring any code injection.

It is important to realize that you can customize and extend this language to meet your specific requirements whether that involves adding more registers, implementing advanced memory control, or even developing an entirely new language with Gama-X as its foundation.

> _Note_:\_ its not required to use `end` instruction at end of each label declaration, but it is maybe usable for sensetive applications.

## Language Command Types

Similar to ARM Assembly, this language distinguishes between two primary types of commands:

1. **Assembler Directives** `.commands`:

   These commands communicate directly with the assembler software.
   - **.Syntax:** Must be prefixed with a dot (e.g., `.directive`)
   - **Flexibility:** There are no structural restrictions on where they can be placed. You can define them within a label or, for cleaner code, group them at the top of your file.

   > _Note:_ if you use `.end` directive, then you are not allowed anymore to use any directive.

2. **Instruction Commands**
   These are the actual processing instructions executed by the system.
   - **Syntax:** Standard mnemonic format (no dot prefix).
   - **Constraint:** Unlike directives, instructions must be placed within a functional body (such as a label or procedure) to be valid.

---

## Next step

check [wiki/instruction-set.md](https://github.com/ABPD2001/gama-x/tree/main/wiki/directives-set.md) for directives set.

## Author

by ABPD2001 (_Abolfazl Pouretemadi_)
