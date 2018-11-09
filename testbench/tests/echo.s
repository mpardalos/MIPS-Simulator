.text
        li $t5, 0x30000000 # Address of getc
        li $t4, 0xFFFFFFFF # EOF
        li $t3, 10 # newline

.read:  lw $t1, 0($t5)
        beq $t1, $t4, .exit
        nop

        sw $t1, 4($t5)
        j .read

.exit:  jr $0

