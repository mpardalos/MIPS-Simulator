.text
    li $t1, -85

    li $t2, 1
    sllv $t1, $t1, $t2 
    
    li $t5, 0x30000000
    sw $t1, 4($t5)

    jr $0
