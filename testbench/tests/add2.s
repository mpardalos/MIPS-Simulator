.text
    addi $t1, $t1, 70
    addi $t2, $t2, -5
    add $t1, $t1, $t2

    li $t5, 0x30000000
    sw $t1, 4($t5)

    jr $0
