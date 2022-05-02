        sw      0       2       one
        sw      1       2       two
        nor     2       2       3
	jalr	0	5
	lw	0	1	five
	noop
        add     1       3       9
	lw      0       3       two
	add	1	1	1
	nor	0	0	0
scoop   beq     0       1       three
        lw      0       3       four
three   halt
        add     4       2       4
 	nor     5       5       5
five	beq	1	1
	noop
        sw      0       4       four
        add     1       3       3
        beq     0       0       scoop
one     .fill   -100
two	.fill   32
        .fill   0
        .fill   -1
        .fill   4
four    .fill   2