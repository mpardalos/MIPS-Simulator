.text
	addi $t1, $t1, 0xFF00
	sll $t1, $t1, 16
	addi $t2, $t2, 28
	sra $2, $t1, $t2
	jr $0
