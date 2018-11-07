.text 
    # Result should have 0x6c in the lowest 8 bits,
    # which is 'l'
    li $t1, 0xFFFFFF32
    li $t2, 0xFFFFFF26

    multu $t2, $t1
    mflo $t1

    li $t5, 0x30000000
    sw $t1, 4($t5)

    jr $0 


