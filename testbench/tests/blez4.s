.text
    li $t1, -5
    .word 0x1920FFFC
    nop

    add $v0, $v0, $t1
    jr $0