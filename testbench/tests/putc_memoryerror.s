.text
    li $t0, 0x30000000

    lhu $t1, 4($t0)

    ori $t2, 0xFFFF

    beq $t1, $t2, ok
    nop

nok: 
    li $v0, 1
    jr $0

ok: 
    li $v0, 10
    jr $0
