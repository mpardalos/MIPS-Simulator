.text 
    li $t1, 34
    li $t2, -5

    mult $t2, $t1
    mflo $t1

    li $t5, 0x30000000
    sw $t1, 4($t5)

    jr $0 


