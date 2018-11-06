# emit the division instruction directly 
# because gas also emits division by zero checking code
.set noreorder
.text
    addi $t1, $t1, -98 
    addi $t2, $t2, 4
    .word 0x12a001b # t1 / t2
    jr $0
