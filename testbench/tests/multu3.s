.text 
    addi $t1, $t1, 5
    sll $t1, $t1, 29 
    addi $t2, $t2, -10
    sll $t2, $t2, 28
    mult $t2, $t1
	mflo $2
    jr $0 


