.set noat 
.text 
    ADDI $1, $1, 5
    SLL $1, $1, 29 
    ADDI $2, $2, -10
    SLL $2, $2, 28
    MULT $2, $1
    JR $0 


