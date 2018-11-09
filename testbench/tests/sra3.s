.text
	addi $t1, $t1, 0xFF00
	sll $t1, $t1, 16
	sra $2, $t1, 28
	jr $0
