.set noat
.text
    addi $t1, $t1, 50
    addi $t2, $t2, 15
    add $t1, $t1, $t2
    li $t5, 0x30000000
    sw $t1, 4($t5)
    jr $0
