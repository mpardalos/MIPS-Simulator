.text
    li $t1, 50
    li $t2, 15
    addu $t1, $t2, $t1

    li $t5, 0x30000000
    sw $t1, 4($t5)

    jr $0
