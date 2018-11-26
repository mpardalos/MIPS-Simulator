.text
    li $t1, -7
    .word 0x530FFFC
    nop

    add $v0, $v0, $t1
    jr $0