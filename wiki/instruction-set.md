# Instruction set

In Gama-X, processing instructions are classified into the following categories:

- **Arithmetic Instructions:** For performing mathematical operations (e.g., addition, subtraction).

- **Logical Instructions:** For bitwise and boolean logic operations.

- **Control Flow (Branch) Instructions:** For jumping between different parts of the code.

- **Input Processing Instruction:** parses and uses external arguments or external files as numeric values.

- **Assembler Instructions:** instructs assembler directly to preform a output writing, or resetings register values/stack values, or even debug messages.

- **Memory Instructions:** For data transfer between registers and system memory (Load/Store).

  > _Note:_ Gama-X utilizes a simulated logic-based memory system rather than direct physical memory mapping.

## Quick menu

- **[Arithmetic Instructions](#arithmetic-instructions)**

- **[Logical Instructions](#logical-instructions)**
  - **[`shf` Instruction](#shf-instruction)**
  - **[`log` Instruction](#log-instruction)**
- **[Control Flow Instructions](#control-flow-branch-instructions)**
  - **[`call` Instruction](#call-instruction)**
  - **[`jmp` Instruction](#jmp-instruction)**
  - **[List of conditions](#list-of-conditions)**
- **[Input Processing Instructions](#input-processing-instructions)**
  - **[`argint` Instruction](#argint-instruction)**
  - **[`exint` Instruction](#exint-instruction)**
- **[Assembler Instructions](#assembler-instructions)**
  - **[`transpile` Instruction](#transpile-instruction)**
  - **[`debug` Instruction](#debug-instruction)**
  - **[`reset` Instruction](#reset-instruction)**
    - **[List of actions](#list-of-actions)**
- **[Memory Instructions](#memory-instructions)**
  - **[Current Status](#current-status)**
  - **[Memory Instruction set](#memory-instruction-set)**
- **[Next step](#next-step)**
- **[Author](#author)**

## Arithmetic Instructions

The following instructions comprise the **arithmetic operations** in Gama-X, as detailed in the table below:

| Instruction | Syntax          | Effect                   | Limit                       | Description                   |
| :---------- | :-------------- | :----------------------- | :-------------------------- | :---------------------------- |
| `add`       | `add Rd,Ra, Rb` | `Rd = Ra + Rb`           | No limitation.              | Adds two numbers              |
| `sub`       | `sub Rd,Ra, Rb` | `Rd = Ra - Rb`           | No limitation.              | Subs two numbers.             |
| `mul`       | `mul Rd,Ra, Rb` | `Rd = Ra * Rb`           | No limitation.              | Multiplies two numbers        |
| `div`       | `div Rd,Ra, Rb` | `Rd = Ra / Rb`           | Do not divide by zero.      | Divides two numbers.          |
| `sqrt`      | `sqrt Rd, Ra`   | `Rd = square root of Ra` | Do not use negetive values. | Square root of number.        |
| `ln`        | `ln Rd,Ra`      | `Rd = ln(Ra)`            | No limitation.              | Natural logarithm of number.  |
| `sin`       | `sin Rd,Ra`     | `Rd = sin(Ra)`           | No limitation.              | Sinus of number.              |
| `sinh`      | `sin Rd,Ra`     | `Rd = sinh(Ra)`          | No limitation.              | Hyperbolic sinus of number.   |
| `cos`       | `cos Rd,Ra`     | `Rd = cos(Ra)`           | No limitation.              | Cosine of number.             |
| `cosh`      | `cosh Rd,Ra`    | `Rd = cosh(Ra)`          | No limitation.              | Hyperbolic cosine of number.  |
| `tan`       | `tan Rd,Ra`     | `Rd = tan(Ra)`           | No limitation.              | Tangent of number.            |
| `tanh`      | `tanh Rd,Ra`    | `Rd = tanh(Ra)`          | No limitation.              | Hyperbolic tangent of number. |

Here is example of each instruction of table:

```armasm
@ Examples

add r0,r0,#1
@ r0 = r0+1
sub r0,r0,#2
@ r0 = r0-2
mul r0,r0,#3
@ r0 = r0*3
div r0,r0,#4
@ r0 = r0/4
ln r0,#23
@ r0 = ln(23)
cos r0,#10
@ r0 = cos(10)
sin r0,#10
@ r0 = sin(10)
sinh r0,#10
@ r0 = sinh(10)
tanh r0,#10
@ r0 = tanh(10)
cosh r0,#10
@ r0 = cosh(10)
abs r0,#10
@ r0 = |10| (absolute value of 10)
rnd r0,r0
@ r0 = rounded value of r0
ceil r0,r0
@ r0 = ceiled value of r0
flr r0,r0
@ r0 = floored value of r0
```

> _Note:_ `Ra`/`Rb` may be either registers or literal values, whereas `Rd` must always be a register.

## Logical Instructions

The following instructions comprise the **logical (bitwise) operations** in Gama-X, as detailed in the table below:

| Instruction | Syntax             | Limit          | Description                                                            |
| :---------- | :----------------- | :------------- | :--------------------------------------------------------------------- |
| `shif`      | `shf Rd,Ra,op1`    | No limitation  | Implements shifting logic on _`Rd`_ with amount of _`Ra`_.             |
| `logi`      | `log Rd,Ra,Rb,op1` | No limitation. | Implements logical bitwise operations of _`Ra`_ and _`Rb`_, on _`Rd`_. |

### `shif` Instruction

This instruction is used to perform shifting operations, supporting both left and right shifts. The syntax and functional usage are as follows:

```armasm
shf Rd,Ra,Op1
```

Parameters:

- `Rd` (_Destination Register_): The register on which the shifting operation will be performed.

- `Rb` (_Amount_): Specifies the number of bits to shift.

- `Op1` (_Direction_): The direction of the shift. Use `LEFT` for a left shift or `RIGHT` for a right shift.

```armasm
@ Example
shif r0,#2,LEFT
@ Description: shift register r0 with amount of 2 and direction of left.
```

### `logi` Instruction

This instruction is used for the implementation of binary operations. The syntax and usage are as follows:

```armasm
logi Rd,Ra,Rb,Op1
```

Parameters:

- `Rd` (_Destination Register_): register to store result on it.

- `Ra` (_First Operand Register_): The first register for the operation.

- `Rb` (_Second Operand Register_): The second register used in conjunction with the first.

- `Op1` (_Operation Type_): Specifies the type of binary operation to be executed (e.g., AND, OR, XOR).

`Op1` operation are these:
| Name | Effect | Description |
| :--- | :--- | :--- |
| `AND` | `Ra = Ra & Rb` | Bitwise and operation|
| `ORR` | `Ra = Ra \| Rb` | Bitwise or operation|
| `XOR` | `Ra = Ra ^ Rb` | Bitwise xor operation|
| `NOT` | `Ra = ~Ra` | Bitwise not operation|
| `NAND` | `Ra = ~(Ra & Rb)` | Bitwise not and operation|
| `NXOR` | `Ra = ~(Ra ^ Rb)` | Bitwise not xor operation|

_Important:_ in `NOT` operation `Rb` does not effects on operation and its recommneded to set `Rb` to `#0`.

```armasm
@ Examples

logi r0,#0,NOT
@ r0 = ~r0
logi r0,r1,AND
@ r0 = r0&r1
logi r0,#2,AND
@ r0 = r0&2
logi r0,r1,ORR
@ r0 = r0|r1
logi r0,r1,XOR
@ r0 = r0^r1
logi r0,r1,NAND
@ r0 = ~(r0&r1)
logi r0,r1,NXOR
@ r0 = ~(r0^r1)
```

## Control Flow (Branch) Instructions

Branch instructions are used to redirect the execution flow from one point to another.

### Current Implementation & Constraints:

- **Label-Based Branching:** Since Gama-X operates in a simulated environment, branching currently relies on Label Names rather than direct memory addresses.

- **Return or No-Return Branching:** it is possible to branch a label and do not return back to main label, also its possible to return main label.

### Instruction table

| Instruction | Syntax                      | Limit                                           | Description                                      |
| :---------- | :-------------------------- | :---------------------------------------------- | :----------------------------------------------- |
| `call`      | `call <label>,<conditiond>` | only labels.                                    | change execution flow (_and return_) to a label. |
| `jmp`       | `jmp <label>,<condition>`   | only labels.                                    | change exection flow (_no return_) to a label    |
| `jmp`       | `cmp Ra,Rb`                 | `Ra` and `Rb` can be literal value or register. | sets condition flags                             |

### `call` Instruction

The `call` instruction is specifically designed to execute a **subroutine** (_label_). Upon completion of the subroutine, execution automatically returns to the original call site.

Paramters:

- `<label>`: name of label to jump.
- `<condition>`: condition of jumping.

### `jmp` Instruction

The `jmp` instruction is specifically designed to jump on a label (like `call`), but it doesnt back to label where came from.

> _Note:_ explained more at examples.

### `cmp` Instruction

The `cmp` instruction sets flags of condition by comparing two values, _(flags = value1-value2)_.

```armasm
@ syntax
cmp Ra,Rb
```

- **`Ra`**: first register _(value1)_.
- **`Rb`**: second register _(value2)_.

### List of conditions

Below is the list of available conditions for the `call` instruction:

| Name | Flags     | Description                |
| :--- | :-------- | :------------------------- |
| `LE` | `N or Z`  | Less than or equal (<=)    |
| `LS` | `N`       | Less than (<)              |
| `GE` | `~N or Z` | Greater than or equal (>=) |
| `GT` | `~N`      | Greater than (>)           |
| `EQ` | `Z`       | Equal (==)                 |
| `NE` | `~Z`      | Not Equal (!=)             |

> _Note:_ these are same for `call` and `jmp` instructions.

```armasm
@ Examples

loop_label:
  add x,x,#1
  mov y,#23
  cmp x,y
  call loop_label,NE
  @ branch to loop_label (if x != y).

my_label:
  add x,x,#1
  add y,y,#23
  mov r0,#102
  sub r0,r0,y

main:
    call loop_label
    jmp my_label
```

## Input Processing Instructions

Gama-X is designed to accept external inputs in a manner similar to conventional applications. These inputs are provided through files and are divided into two categories:

- **Argument-based input files:** These files follow a predefined syntax and format, allowing arguments to be defined as key-value pairs. The values can then be accessed and used within the program.

- **Numeric-value input files:** These files contain only a numeric value and do not require any specific syntax or formatting.

> **Note:** Inputs can be used for a variety of purposes. One of their most important applications is generating different program outputs from a single Gama-X application by supplying different input values.

### `argint` Instruction

This instruction iterates through the list of all parsed arguments, searches for the requested key, and, if found, stores its numeric value in the specified register.

```armasm
@ syntax
argint Rd,"<key>"

@ Example
argint r0,"my-key"
```

- **`Rd`**: Destination register in which the retrieved value will be stored.
- **`<key>`**: The key whose associated value should be retrieved.

### `exint` Instruction

This instruction searches through all loaded external input files, retrieves the numeric value from the requested file, and stores it in the specified register.

```armasm
@ syntax
exint Rd,"<file-name>"

@ Example
exint r0,"my-file"
```

- **`Rd`**: Destination register in which the retrieved value will be stored.
- **`<file-name>`**: The name of readed file to use its content.

> _Note:_ Unlike `argint`, which searches argument-based input files and retrieves values by key, this instruction operates on numeric input files and directly uses the file's numeric content.

> **Recommendtion:** for better understanding of _input files_ concept in _Gama-X_, check [wiki/advanced-features]().

## Assembler Instructions

Assembler instruction are commands that communicate directly with the assembler itself. Unlike regular instructions, they do not perform any computational operations. However, they are essential for controlling output generation, debugging, and optimizing the compilation process. In practice, assembler instruction play a critical role in the development workflow and are often required for efficient program compilation.

### `transpile` Instruction

This directive reads the values of special registers and inserts them into the output file according to the user's instructions. each time you need to write output into output file, call `transpile` instruction.

> **Important:** means of _user's instructions_ is the arguments and flags passed to app about format of output file.

```armasm
@ Example
main:
    mov x,#1
    mov y,#10
    transpile
    @ writes special registers value into output file.
    add x,x,#10
    sub y,y,#9
    transpile
    @ writes special registers value into output file.

@ output would be like this (in an imaginary format):
@ First output: X:1, Y:10
@ Second output: X:10, Y:1

@ Note: this format isnt actual real format of output!
```

> _Note:_ if not any `transpile` instruction called, then output will be empty.

> _Better to know:_ _Gama-X_ uses snipping method to create its outputs and put them into format of output, means, each time `transpile` instruction called, new snippet of current special registers values creates, then after transpiling whole application into snippets, Gama-X turns snippets into text/binary format or user's format.

### `debug` Instruction

This instruction prints the provided message only when it is executed during the compilation process.

```armasm
@ syntax

debug(<text>)

@ Example
debug(just a message)

@ at compiling time after executing this, somthing like this would print:
@ [debug, line X]: just a message
```

- `<text>`: message to print at compile time.

### `reset` instruction

Depending on the requested operation, this instruction clears general-purpose registers, special registers, or both, as well as the virtual stack.

```armasm
@ syntax
reset <action>

@ Example
reset all
@ resets every registers (both general and special purposes).
reset general
@ reset only general-purpose registers.
```

- `<action>`: determines which values to reset.

#### list of actions

- `general`: only general-purpose registers resets.
- `special`: only special registers resets.
- `all`: both general-purpose and special registers resets.
- `stack`: reset virtual stack (memory of `pull`/`push` instructions).

## Memory Instructions

**Unlike other assembly languages,** memory instructions in Gama-X are currently limited as the project is still under active development. Since Gama-X operates within a simulated environment, these limitations might not be immediately apparent during basic usage, but users should exercise caution when managing data.

> _Note:_ currently, its just a virtual stack.

### Current Status

- **Environment:** Logic-simulated memory (not physical).

- **Availability:** Limited instruction set for data movement.

- **Precaution:** Ensure all memory operations are thoroughly tested within the simulator's scope.

### Memory Instruction set

| Instruction | Syntax    | Effect         | Limit          | Description                       |
| :---------- | :-------- | :------------- | :------------- | :-------------------------------- |
| `pull`      | `pull Ra` | `Memory <= Ra` | just registers | Load from memroy stack into `Ra`. |
| `push`      | `push Ra` | `Memory => Ra` | just registers | Store `Ra` into memory stack.     |

> _Note:_ Additional instructions will be introduced in future updates. Currently, as a _numerical memory simulation system_ has not yet been implemented, the available instructions are limited to the current set.

```armasm
@ Example

test_label:
  mov r0,#10
  mov r1,#9
  sub r0,r0,r1
  push r0
  @ store info memory from r0 (r0 ==> memory).
  add r0,r0,#10
  pull r1
  @ load from memory into r1 (memory ==> r1).
  add r0,r0,r1
```

---

## Next step

check [wiki/advanced-features.md](https://github.com/ABPD2001/gama-x/blob/main/wiki/advanced-features.md) for adavnced features.

## Author

by ABPD2001 (_Abolfazl Pouretemadi_)
