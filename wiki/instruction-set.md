# Instruction set

In Gama-X, processing instructions are classified into the following categories:

- **Arithmetic Instructions:** For performing mathematical operations (e.g., addition, subtraction).

- **Logical Instructions:** For bitwise and boolean logic operations.

- **Control Flow (Branch) Instructions:** For jumping between different parts of the code.

- **Memory Instructions:** For data transfer between registers and system memory (Load/Store).

  > _Note:_ Gama-X utilizes a simulated logic-based memory system rather than direct physical memory mapping.

## Quick menu

- **[Quick menu](#quick-menu)**
- **[Arithmetic Instructions](#arithmetic-instructions)**
- **[Logical Instructions](#logical-instructions)**
  - **[`shf` Instruction](#shf-instruction)**
  - **[`log` Instruction](#log-instruction)**
- **[Control Flow (Branch) Instructions](#control-flow-branch-instructions)**
  - **[`call` Instruction](#call-instruction)**
  - **[`jmp` Instruction](#jmp-instruction)**
  - **[List of conditions](#list-of-conditions)**
- **[Memory Instructions](#memory-instructions)**
  - **[Current Status](#current-status)**
  - **[Memory Instruction set](#memory-instruction-set)**
- **[Next step](#next-step)**
- **[Author](#author)**

## Arithmetic Instructions

The following instructions comprise the **arithmetic operations** in Gama-X, as detailed in the table below:

| Instruction | Syntax       | Effect         | Limit                                                      | Description            |
| :---------- | :----------- | :------------- | :--------------------------------------------------------- | :--------------------- |
| `add`       | `add Ra, Rb` | `Ra = Ra + Rb` | Signed Double (64-Bits)                                    | Adds two numbers       |
| `sub`       | `sub Ra, Rb` | `Ra = Ra - Rb` | Signed Double (64-Bits)                                    | Subs two numbers       |
| `mul`       | `mul Ra, Rb` | `Ra = Ra * Rb` | Signed Double (64-Bits)                                    | Multiplies two numbers |
| `div`       | `div Ra, Rb` | `Ra = Ra / Rb` | Signed Double (64-Bits) and Division by zero is prohibited | Divides two numbers    |

Here is example of each instruction of table:

```armasm
@ Examples

add r0,#1
@ r0 = r0+1
sub r0,#2
@ r0 = r0-2
mul r0,#3
@ r0 = r0*3
div r0,#4
@ r0 = r0/4
```

## Logical Instructions

The following instructions comprise the **logical (bitwise) operations** in Gama-X, as detailed in the table below:

| Instruction | Syntax          | Limit                   | Description                                             |
| :---------- | :-------------- | :---------------------- | :------------------------------------------------------ |
| `shf`       | `shf Ra,Rb,op1` | Signed Double (64-Bits) | Implements shifting logic on _Ra_ with amount of _Rb_.  |
| `log`       | `log Ra,Rb,op1` | Signed Double (64-Bits) | Implements logical bitwise operations on _Ra_ and _Rb_. |

### `shf` Instruction

This instruction is used to perform shifting operations, supporting both left and right shifts. The syntax and functional usage are as follows:

```armasm
shf Ra,Rb,Op1
```

Parameters:

- `Ra` (_Target Register_): The register on which the shifting operation will be performed.

- `Rb` (_Amount_): Specifies the number of bits to shift.

- `Op1` (_Direction_): The direction of the shift. Use `LEFT` for a left shift or `RIGHT` for a right shift.

```armasm
@ Example
shf r0,#2,LEFT
@ Description: shift register r0 with amount of 2 and direction of left.
```

### `log` Instruction

This instruction is used for the implementation of binary operations. The syntax and usage are as follows:

```armasm
log Ra,Rb,Op1
```

Parameters:

- `Ra` (_First Operand Register_): The primary register for the operation.

- `Rb` (_Second Operand Register_): The secondary register used in conjunction with the first.

- `Op1` (_Operation Type_): Specifies the type of binary operation to be executed (e.g., AND, OR, XOR).

`Op1` operation are these:
| Name | Effect | Description |
| :--- | :--- | :--- |
| `AND` | `Ra = Ra & Rb` | Bitwise and operation|
| `ORR` | `Ra = Ra \| Rb` | Bitwise or operation|
| `EOR` | `Ra = Ra ^ Rb` | Bitwise xor operation|
| `NOT` | `Ra = ~Ra` | Bitwise not operation|
| `NAND` | `Ra = ~(Ra & Rb)` | Bitwise not and operation|
| `NEOR` | `Ra = ~(Ra ^ Rb)` | Bitwise not xor operation|

_Important:_ in `NOT` operation `Rb` does not effects on operation and its recommneded to set `Rb` to `#0`.

```armasm
@ Examples

log r0,#0,NOT
@ r0 = ~r0
log r0,r1,AND
@ r0 = r0&r1
log r0,#2,AND
@ r0 = r0&2
log r0,r1,ORR
@ r0 = r0|r1
log r0,r1,EOR
@ r0 = r0^r1
log r0,r1,NAND
@ r0 = ~(r0&r1)
log r0,r1,NEOR
@ r0 = ~(r0^r1)
```

## Control Flow (Branch) Instructions

Branch instructions are used to redirect the execution flow from one point to another.

### Current Implementation & Constraints:

- **Label-Based Branching:** Since Gama-X operates in a simulated environment, branching currently relies on Label Names rather than direct memory addresses.

- **Future Roadmap:** While future updates may introduce simulated memory addresses (similar to physical hardware), the current version requires labels for all jump operations.

- **Special Case (Line Jumping):** There is only one specific instruction that accepts arguments other than labels. Note that these arguments are not memory addresses, but they allow for jumping between specific lines of code.

  > Note: Always ensure the label is defined before attempting to branch to it to avoid execution errors.

### Instruction table

| Instruction | Syntax                         | Limit                      | Description                                                           |
| :---------- | :----------------------------- | :------------------------- | :-------------------------------------------------------------------- |
| `call`      | `call <label>,<conditiond>`    | only labels.               | change execution flow (_and return_) to a label.                      |
| `jmp`       | `jmp <label-line>,<condition>` | only line of current label | change exection flow (_no return_) to a line of current running label |

### `call` Instruction

The `call` instruction is specifically designed to execute a **subroutine** (_label_). Upon completion of the subroutine, execution automatically returns to the original call site.

Paramters:

- `<label>`: name of label to jump.
- `<condition>`: condition of jumping.

### `jmp` Instruction

The `jmp` instruction is specifically designed to jump on a line (_not a label or something like that_), but there is a exception, `jmp` instruction only can jump inside of current running label, also, passed argument is relative to current running label.

> _Note:_ explained more at examples.

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

my_label:
  add x,#1
  mov y,#23
  cmp x,y
  call my_label,NE
  @ jump to my_label (if x != y).

my_label2:
  add x,#1
  @ ^^^^^^ --> this is line 0.
  add y,#23
  @ ^^^^^^^ --> this is line 1.
  mov r0,#102
  sub r0,y
  cmp r0,y
  jmp 1,GT
  @ ^^^^^^ --> jump on line 1 nth.
```

## Memory Instructions

**Unlike other assembly languages,** memory instructions in Gama-X are currently limited as the project is still under active development. Since Gama-X operates within a simulated environment, these limitations might not be immediately apparent during basic usage, but users should exercise caution when managing data.

> _Warning:_ **Simulated Memory Management System** is in its early stages. Improper use of memory instructions may lead to unexpected behavior in the simulation.

### Current Status

- **Environment:** Logic-simulated memory (not physical).

- **Availability:** Limited instruction set for data movement.

- **Precaution:** Ensure all memory operations are thoroughly tested within the simulator's scope.

### Memory Instruction set

| Instruction | Syntax    | Effect         | Limit              | Description                 |
| :---------- | :-------- | :------------- | :----------------- | :-------------------------- |
| `pull`      | `pull Ra` | `Memory <= Ra` | only for `doubles` | Load from memroy into `Ra`. |
| `push`      | `push Ra` | `Memory => Ra` | only for `doubles` | Store `Ra` into memory.     |

> _Note:_ Additional instructions will be introduced in future updates. Currently, as a _numerical memory simulation system_ has not yet been implemented, the available instructions are limited to the current set.

```armasm
@ Example

test_label:
  mov r0,#10
  mov r1,#9
  sub r0,r1
  push r0
  @ store info memory from r0 (r0 ==> memory).
  add r0,#10
  pull r1
  @ load from memory into r1 (memory ==> r1).
  add r0,r1
```

---

## Next step

check [wiki/directive-set.md](https://github.com/ABPD2001/gama-x/blob/main/wiki/directive-set.md) for directives.

## Author

by ABPD2001 (_Abolfazl Pouretemadi_)
