.text
    li $t1, -98 
    li $t2, 4

    .word 0x012a001b # divu $t1, $t2
    mfhi $v0

    jr $0
