# Instruction set

In Gama-X, processing instructions are classified into the following categories:

- **Arithmetic Instructions:** For performing mathematical operations (e.g., addition, subtraction).

- **Logical Instructions:** For bitwise and boolean logic operations.

- **Control Flow (Branch) Instructions:** For jumping between different parts of the code.

- **Memory Instructions:** For data transfer between registers and system memory (Load/Store).

  > _Note:_ Gama-X utilizes a simulated logic-based memory system rather than direct physical memory mapping.

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

## Memory Instructions
