.text
    addi $t1, $t1, 20
    addi $t2, $t2, 15
    beq $t1, $t2, equal
    addi $2, $2, 10
    jr $0
    
equal: 
    addi $2, $2, 5
    jr $0
