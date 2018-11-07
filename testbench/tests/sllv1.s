.text
    li $t1, 1

    li $t2, 6
    sllv $t1, $t1, $t2 # t1 = 2^6 = 64 -> ascii @
    
    li $t5, 0x30000000
    sw $t1, 4($t5)

    jr $0
