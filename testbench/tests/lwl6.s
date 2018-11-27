.set noat
.text
    li $t2, 0x23FFFFFC
    li $t1, 0x12345678

    sw $t1, ($t2)

    li $v0, 0x87654321

    lwl $v0, ($t2)

    jr  $0