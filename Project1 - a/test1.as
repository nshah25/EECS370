	sw      2       9       label
	sw      1       2       3
add     2       2       2
label   beq     0       5       2
noop
incorrect beq     0       5       2
	lw	4	6	nonexistant
	.fill   99
	.fill   0
keyboard .fill   -123