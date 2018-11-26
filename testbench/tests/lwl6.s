.text
    li $t2, 0x23FF0000
    li $t1, 0x12345678
    sw $t1, 65532($t2)

    li $v0, 0x87654321
    lwl $v0, 65532($t2)

    jr  $0