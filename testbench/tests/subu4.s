.set noat
.text
    ADDI $1, $1, -7 
    ADDI $2, $2, -6
    SLL $2, $2, 28
    SLL $1, $1, 28
    SUBU $1, $1, $2 
    JR $0
