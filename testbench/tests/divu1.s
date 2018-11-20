.text
    li $t1, 28 
    li $t2, 4

    .word 0x012a001b # divu $t1, $t2
    mflo $v0

    jr $0
