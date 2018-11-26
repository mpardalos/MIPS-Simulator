.text
    li $t1, 13
    .word 0x1D20FFFC
    nop

    add $v0, $v0, $t1
    jr $0