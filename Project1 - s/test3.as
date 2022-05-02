	sw	0	7	label1
	sw	0	7	label2
	sw	0	7	label3
	lw	0	7	label4
	nor	1	1	4
	noop
	noop
	noop
label5	beq	4	3	label6
	nor	1	1	0
label6	add	4	3	2
	noop
	add	2	2	1
	beq	0	5	3	label5
	halt
label1	.fill	0
label2	.fill	65536
label3	.fill	-65536
label4	.fill	2
