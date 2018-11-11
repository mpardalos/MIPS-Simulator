.text 
    addiu $t1, $t1, 5
    sll $t1, $t1, 28 
    addiu $t2, $t2, -10
    sll $t2, $t2, 28
    mult $t2, $t1
	mfhi $2
    srl $2, $2, 24
    jr $0 


