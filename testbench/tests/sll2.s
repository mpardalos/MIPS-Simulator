.text
    li $t1, -85
    sll $t1, $t1, 1

    li $t5, 0x30000000
    sw $t1, 4($t5)

    jr $0
