.text
    addi $t1, $t1, -6
    bltzal $t1, .t
.r:
    addi $2, $2, -1
    jr $0

.t:
    jr $ra