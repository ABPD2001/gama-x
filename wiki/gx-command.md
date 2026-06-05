# `gx` Binary File

`gx` command transpiles _Gama-X_ application to a raw output where is usable for machines with correct drivers.

> _Note:_ Gamx-X output file isnt executable natively.

## Quick Menu

- **[Manually Setting Registers](#manually-setting-registers)**
- **[Configuring Binary Output](#configuring-binary-output)**
  - **[Format of setting registers manually](#format-of-setting-registers-manually)**
  - **[Example](#example)**
- **[Error Formatting](#error-formatting)**
- **[Libraries and Repositories](#libraries-and-repositories)**

here is options:

- `-o --output`: output path of output file.
- `-r --repository`: repository to use for applications requested libraries.
- `-v --values`: initial value for registes (more functional at setting registers manually).
- `-g --registers`: name for registers (setting registers manually).
- `-s --special-registers`: choose special registers by name of registers.
- `-b --binary`: output binary instead of text (required to set logic of output).
- `-l --binary-logics`: logic of binary output, detemining size of chunks and where to put special registers value on it.
- `-e --error-format`: set format of errors of application (runtime and pre-compilation time).
- `-i --ignore-repository`: do not use any repository.
- `-d --ignore-dependecies`: do not use any dependecy for libraries.
- `-V --version`: print version of _Gama-X_.
- `-c --config`: use config file instead of passing arguments and flags via command line.
- `-h --help`: prints help text.

## Manually Setting Registers

it is possible and to functional to change registers by name, value or length, this makes Gama-X more flexiable; also it is possible to change special registers, because its to functional.

> _Note:_ mean of special registers are registers where they value gonna used each time transpilation happens because of `transpile` command called.

### Format of setting registers manually

starting with defining registers:

```bash
gx -g r0,r1,r2,r3,r4,r5,s0,s1,s2
# this is format of setting names of manual registers where user wants.
```

both special registers and general-purpose registers must be entered.

```bash
gx -v 0,0,0,0,0,0,0,0,0,
# this is format of setting initial value of manual register where user wants.
```

order of values placement must be suitable with order of register names

```bash
gx -s s0,s1,s2
# this is format of choosing which registers are special registers.
```

choosen registers must be enetered pastly as register names, also, _you must just select special registers, not re-defining them!_

#### Example

```bash
gx -g r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r13,r14,x,y
-v 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 -s x,y
```

in this example, `r0 ~ r14` registers set as _general-purpose_ registers, `x` and `y` set as _special registers_, all initial value set to `0`.

## Configuring Binary output

using binary output is one of the most important and effective ways to reduce output size and improve readability for industrial and embedded machines. For this reason, Gama-X provides the ability to generate binary output files.

to achieve binary file output, you must set logic (format) of output binary file by `-l, --binary-logic` format:

```bash
gx ... -s s0,s1,s2 -b -l u64,u64,u32
```

> _Note:_ also its required to use `-b, --binary` flag.

each element in `-l, --binary-logic` argument, must be a logic state, logic states are:

- `u64`: unsigned 64-bits.
- `u32`: unsigned 32-bits.
- `u16`: unsigned 16-bits.
- `u8`: unsigned 8-bits.
- `s64`: signed 64-bits.
- `s32`: signed 32-bits.
- `s16`: signed 16-bits.
- `s8`: signed 8-bits.

**order of `-l, --binary-logic` elements must be suitable to `-s, --special-registers` elements**.

## Error Formatting

Gama-X also allows you to customize the format and appearance of error messages, enabling errors to be displayed according to the user's preferences and requirements.

here is default format:

```txt
Error in <FILE> at line <LINE>: <MESSAGE>
type <TYPE>, cause <CAUSE>
```

- `<FILE>`: path of errored file.
- `<LINE>`: line of error.
- `<MESSAGE>`: message of error.
- `<TYPE>`: type of error.
- `<CAUSE>`: code-based cause of errored.

but it is possible to pass diffrent formats, like:

```txt
my custom error format is best
<FILE> [<LINE>]: <MESSAGE>
(<TYPE>, <CAUSE>)
``
```

but how to set format of error?

```bash
gx -e ./my-error-format.txt
```

> _Note:_ `-e, --error-format` must be a regular text file.

## Libraries and Repositories

_Gama-X_ does not provide direct control over library and local repository management. However, when using libraries, it is recommended—and in some cases required—to specify which repository should be used. Alternatively, you may choose not to use any repository at all, or avoid automatic dependency linking for the selected library.

if you dont want to use any library from any repository, use `-i, --ignore-repository` to avoid using any repository.

or if you want to use library, but not linking used libraries dependecies with linker, `-d, --ignore-dependecies` to avoid any dependecy checking process.

but if you want to use library from a repoistory, use `-r, --repository` argument.

```bash
# Example
gx --repository my-repo
# or
gx --ignore-repository --ignore-dependecies ...
```
