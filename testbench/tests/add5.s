.text
    addi $t1, $t1, -100
    addi $t2, $t2, -70
    add $t1, $t1, $t2

    li $t5, 0x30000000
    sw $t1, 4($t5)

    jr $0
