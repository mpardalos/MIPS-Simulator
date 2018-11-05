# .set noat
.text
    addi $2, $2, 30 
    addi $3, $3, -4
    div $2, $3
    mfhi $4 
    jr $0
