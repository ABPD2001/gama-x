.module
.end

_set_vert_line:
	mov r5,r3
	mov r6,r4
	
	shf r5,r0,LEFT
	log x,r5,ORR	
	mov y,#4294967296
	
	.transpile
	end
