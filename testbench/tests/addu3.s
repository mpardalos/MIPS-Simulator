.set noat
.text
    addi $t1, $t1, 8
    sll $t1, $t1, 28
    addi $t2, $t2, 8
    sll $t1, $t1, 28
    addu $t1, $t2, $t1
    jr $0
