.set noat
.text
    addi $1, $1, -8
    sll $1, $1, 28
    add $1, $1, $1
    jr $0
