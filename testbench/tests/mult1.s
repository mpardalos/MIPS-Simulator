.text 
    li $t1, 22 
    li $t2, 3

    mult $t1, $t2
    mflo $t1

    li $t5, 0x30000000
    sw $t1, 4($t5)

    jr $0 
