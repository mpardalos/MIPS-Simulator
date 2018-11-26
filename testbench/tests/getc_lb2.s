.text
    li $t0, 0x30000000

    lb $t1, 3($t0)

    li $t2, -1

    beq $t1, $t2, ok
    nop

nok: 
    li $v0, 1
    jr $0

ok: 
    li $v0, 10
    jr $0
