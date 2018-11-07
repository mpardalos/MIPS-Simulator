.text
    li $t1, 0x01
    li $t2, 0x40 

    or $t3, $t2, $t1 
    
    li $t5, 0x30000000
    sw $t3, 4($t5)

    jr $0
