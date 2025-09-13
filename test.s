.main main
.end

main:
	mov x,#10
	mov y,#11
	mov r0, #1
	mov r1, #1
	cmp r0,r1
	call test,EQ
	.transpile
	end

test:
	mov v, #1
	end
