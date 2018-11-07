.text
    li $t1, 1

    sll $t1, $t1, 6 # t1 = 2^6 = 64 -> ascii @

    li $t5, 0x30000000
    sw $t1, 4($t5)

    jr $0
