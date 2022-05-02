	lw	0	1	five
	add	4	1	3
done	nor	0	0	0
	sw	4	3	1
	beq	0	1	done
	jalr	1	1	1
five	beq	0	2	3
	noop
	halt
	.fill	1
	.fill	-1
	.fill	0
	