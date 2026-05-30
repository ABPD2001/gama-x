.main _start
.argular "./arg.txt"
.end

test:
    mov y,#10
    transpile

_start:
    reset
    transpile
    mov x,#10
    transpile
    call test
    argint x,kamali
    transpile