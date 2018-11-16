.text
    addi $t1, $t1, 6
    bltzal $t1, .t
    nop
    addi $2, $2, 2
    jr $0

.r:
    addi $2, $2, -1
    jr $0

.t:
    jr $ra