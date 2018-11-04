.set noat
.text
    ADDI $1, $1, 30 
    ADDI $2, $2, 4
    DIV $1, $1, $2
    MFLO $3 
    JR $0