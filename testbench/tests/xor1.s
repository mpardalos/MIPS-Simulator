.text
    li $t1, 0xBE # Negated 'A'
    li $t2, 0xFF

    xor $t1, $t1, $t2

    li $t5, 0x30000000
    sw $t1, 4($t5)

    jr $0