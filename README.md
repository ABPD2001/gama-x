# Gama-X Programming language

A powerful replacement for traditional, logic-less **DSLs** (_Domain static language_). **Gama-X** is a _fast_, _lightweight_, and _logic-driven_ intermediate language that combines low-level efficiency with the ease of high-level programming.

## why this language?

Combining the flexibility of high-level logic with a hybrid **Compiler-Interpreter** architecture, _Gama-X_ delivers significantly faster performance than conventional interpreters.

Gama-X incorporates advanced logical constructs, including:

- **Labels & Branching:** Enabling modular code structure and efficient jumps.

- **Conditional Logic:** Supporting complex decision-making processes during execution.

- **Arithmetic & Binary Logic:** Full support for mathematical operations and bitwise manipulations.

- **Dynamic Variables:** Efficient memory management through symbolic variable handling.

> _note:_ its important to know, _Gama-X is **not** executable as native_, actually, machines with its driver can execute it.

> _note_: also you must know, _Gama-X is **logic-simulated** language_, all logics, computes on your personal computer, then, all calculated results, runs on machine as a raw output file.

### Theoretical Complexity

While G-Code is widely used, it suffers from the overhead of being an interpreted language. Based on empirical data and interpreter analysis, the comparison is as follows:

| Feature                 | G-Code (Legacy)                 | Gama-X (Modern)                     |
| :---------------------- | :------------------------------ | :---------------------------------- |
| **Time Complexity**     | **$O(n  \log (n))$** (Variable) | **$O(1)$** (Constant)               |
| **Execution Model**     | Line-by-line Parsing            | Direct Binary Execution             |
| **Real-time Stability** | Low (Non-deterministic)         | **Highest (Deterministic)**         |
| **Logic Support**       | None (Motion only)              | Full (Conditions, Loops, Variables) |

#### Why $O(1)$?

In **G-Code**, the controller must read text, tokenize it, and search through symbol tables for every single line. This process is time-consuming and its duration fluctuates ($O(n \log n)$).

In **Gama-X**, all logical overhead is shifted to the **Pre-compilation Phase**. The target hardware receives pre-calculated binary data. Writing a value to a register or memory address takes the same amount of time regardless of program size, resulting in a **Constant Time Complexity of O(1)**.

> _note:_ This makes Gama-X the ideal choice for **High-Precision Real-time Systems** where every microsecond counts and execution time must be perfectly predictable.

## How to use this language?

check [wiki/basic.md](https://github.com/ABPD2001/gama-x/tree/main/wiki) as start of documention.

---

_by Abolfazl Pouretemadi._
