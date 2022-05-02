	lw	2	2	test
	sw	0	2	label
loop	beq	0	4	array
	lw	2	2	five
array	beq	0	1	loop
	jalr	4	3
	noop
test	halt
label	.fill	12
five	.fill	-1
