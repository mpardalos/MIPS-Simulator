    .text
        li $t0, 0x20000004 

        li $t1, 0x823562BF
        sw $t1, ($t0)

        lhu $v0, ($t0)
        srl $v0, $v0, 9
        jr $0