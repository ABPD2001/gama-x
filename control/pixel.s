.module
.end

_set_pixel:
	mov r5,r2
	mov r6,r3
	
	shf r5,r0,LEFT
	shf r6,r1,LEFT
	
	log x,r4,ORR
	log y,r5,ORR
	
	mov x,r0
	mov y,r1
	.transpile
	end
