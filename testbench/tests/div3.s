.text
    li $t1, -98 
    li $t2, 0

    .word 0x012a001a # div $t1, $t2

    li $v0, 10

    jr $0
