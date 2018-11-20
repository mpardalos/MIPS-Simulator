.text 
    addi $t1, $t1, 39
    addi $t2, $t2, 2
    .word 0x012a001a # div $t1, $t2
    mfhi $2
    jr $0
