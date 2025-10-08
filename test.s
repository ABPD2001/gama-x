.main _main
.end
_main:
	mov r0, #1
	mov r1, #2
	mov x,#10
	mov y,#11
	.transpile
	cmp r0,r1
	call test,EQ
	end

test:
	mov x,#99
	.transpile
	end
