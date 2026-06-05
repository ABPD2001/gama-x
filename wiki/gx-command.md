# `gx` Binary File

`gx` command transpiles _Gama-X_ application to a raw output where is usable for machines with correct drivers.

> _Note:_ Gamx-X output file isnt executable natively.

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
