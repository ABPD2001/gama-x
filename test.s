.main _main
.end

_main:
	.reset
	mov x,#10
	.transpile
	mov r0,#1
	shf r0,#1,LEFT
	log r0,#4,ORR
	cmp r0,#7
	call test,EQ
	end

test:
	mov x,#1
	mov y,#1
	.transpile
	end
