.text 
    addi $t1, $t1, 5
    sll $t1, $t1, 29 
    addi $t2, $t2, -10
    sll $t2, $t2, 28
    mult $t1, $t2
	mfhi $2	
    srl $2, $2, 24
    jr $0 


