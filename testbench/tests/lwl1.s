.text
    li $t0, 0x20000004

    li $t1, 0x12345678
    sw $t1, ($t0)

    li $v0, 0x87654321
    lwl $v0, ($t0)

    jr $0