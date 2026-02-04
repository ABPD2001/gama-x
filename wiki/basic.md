# Documention

here is quick menu:

- **[Prerequisites](#prerequisites)**
- **[Getting Started with Syntax](#getting-started-with-syntax)**
- **[Next step](#next-step)**

## Prerequisites

Before getting started, please note that working with this intermediate language requires a basic understanding of the following:

- **Assembly Fundamentals:** Basic knowledge of assembly language principles.

- **Programming Concepts:** General familiarity with core programming logic.

- **System Drivers:** Understanding how drivers operate and interface with the target system.

## Getting Started with Syntax

This language directly inherits its syntax from **Assembly**. To ensure maximum readability, the syntax has been designed to closely resemble **ARM Assembly**, which is widely regarded as one of the most intuitive and straightforward assembly languages available.

in ARM Assembly, syntax is like this:

```armasm
.command_x arg_m
.command_y arg_n
.command_z
@ this is a comment

label_x:
    op1 arg1,arg2
    opX argY,argZ
```

and Gama-X uses this syntax directly as its syntax.

> _Note:_ also in _ARM Assembly_, registers are named `r0 ~ r15` (in **armv7**) and its same with _Gama-X_.

It is important to realize that you can customize and extend this language to meet your specific requirements whether that involves adding more registers, implementing advanced memory control, or even developing an entirely new language with Gama-X as its foundation.

## Language Command Types

Similar to ARM Assembly, this language distinguishes between two primary types of commands:

1. **Assembler Directives** `.commands`:

   These commands communicate directly with the assembler software.
   - **.Syntax:** Must be prefixed with a dot (e.g., `.directive`)
   - **Flexibility:** There are no structural restrictions on where they can be placed. You can define them within a label or, for cleaner code, group them at the top of your file.

2. **Instruction Commands**
   These are the actual processing instructions executed by the system.
   - **Syntax:** Standard mnemonic format (no dot prefix).
   - **Constraint:** Unlike directives, instructions must be placed within a functional body (such as a label or procedure) to be valid.

---

## Next step

check [wiki/instruction-set.md](https://github.com/ABPD2001/gama-x/tree/main/wiki/instruction-set.md) for instruction set.
