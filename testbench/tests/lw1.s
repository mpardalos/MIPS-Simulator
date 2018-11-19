    .text
        li $t0, 0x20000004 

        li $t1, 0x823562BF
        sw $t1, ($t0)
        li $t1, 0x12345678
        sw $t1, 4($t0)

        lw $v0, ($t0)
        jr $0