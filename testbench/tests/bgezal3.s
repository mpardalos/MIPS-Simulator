.text
    addi $2, $2, -4
    bgezal $2, .greater
    addi $2, $2, 6
    jr $0

.rt:
    li $2, 5
    jr $0

.greater:
    j $ra
    jr $0