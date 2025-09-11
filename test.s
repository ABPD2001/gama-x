.main test
.end

test:
mov r0,#10
call test2
.transpile
end
test2:
mov x,r0
mov y,x
end
