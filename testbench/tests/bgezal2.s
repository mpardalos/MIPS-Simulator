.set noreorder
.text
    addi $2, $2, 1
    bgezal $2, .greater

.rt:
    li $2, 5
    jr $0

.greater:
    j $ra
    jr $0