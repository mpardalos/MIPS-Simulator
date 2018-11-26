.text
    li $t1, 13
    li $s0, 13
    .word 0x1130FFFC
    nop

    add $v0, $v0, $t1
    jr $0