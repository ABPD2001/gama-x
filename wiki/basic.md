# Documention

Before getting started, please note that working with this intermediate language requires a basic understanding of the following:

- **Assembly Fundamentals:** Basic knowledge of assembly language principles.

- **Programming Concepts:** General familiarity with core programming logic.

- **System Drivers:** Understanding how drivers operate and interface with the target system.

## Getting Started with Syntax

This language directly inherits its syntax from **Assembly**. To ensure maximum readability, the syntax has been designed to closely resemble **ARM Assembly**, which is widely regarded as one of the most intuitive and straightforward assembly languages available.

in ARM Assembly, syntax is like this:

```s
@ this is comment
.command_x arg_m
.command_y arg_n
.command_z

label_x:
    op1 arg1,arg2
    opX argY,argZ
```

and Gama-X uses this syntax directly as its syntax.

> _Note:_ also in _ARM Assembly_, registers are named `r0 ~ r15` (in **armv7**) and its same with _Gama-X_.

It is important to realize that you can customize and extend this language to meet your specific requirements whether that involves adding more registers, implementing advanced memory control, or even developing an entirely new language with Gama-X as its foundation.

## Introduction to instruction sets
