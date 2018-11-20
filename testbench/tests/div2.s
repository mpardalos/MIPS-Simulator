.text
    li $t1, -98 
    li $t2, 4

    .word 0x012a001a # div $t1, $t2
    mflo $v0

    jr $0
