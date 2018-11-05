.set noat
.text
    addi $1, $1, 8
    sll $1, $1, 28
    addi $2, $2, 8
    sll $1, $1, 28
    addu $1, $2, $1
    jr $0
