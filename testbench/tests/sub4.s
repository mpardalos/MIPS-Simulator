.text 
    addi $t1, $t1, -7 
    addi $t2, $t2, 6
    sll $t2, $t2, 28
    sll $t1, $t1, 28
    sub $t1, $t1, $t2 
    jr $0 

