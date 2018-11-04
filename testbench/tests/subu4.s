.set noat
.text
    ADDI $1, $1, -7 
    ADDI $2, $2, -6
    LSL $2, $2, 28
    LSL $1, $1, 28
    SUBU $1, $1, $2 
    JR $0