.set getc, 0x30000000
.set putc, 0x30000004
.set target, 89 # ascii 'X'

.text
__start:
    j game

game:
    # print(prompt)
    li $a0, dprompt
    bal print 

    bal read_char # $v0 = getchar()
    li $a1, target # $a1 = target

    blt $v0, target, higher
    beq $v0, 0, correct
    bgt $v0, target, lower

    b exit

correct: 
    li $a0, dwin
    bal print
    jr $0

lower: 
    li $a0, dlower
    bal print
    b game 

higher: 
    li $a0, dhigher
    bal print
    b game 


# # Compare $a0 and $a1. 
# # $v0 is: -1 if $a0 is greater, 
# #          0 if they are equal
# #          1 if $a1 is greater
# cmp:
#     sub $a0, $a0, $a1
#     break
#     jr $0
#     bgt $a0, cmp_fst
#     blt $a0, cmp_snd
#     beq $a0, $0, cmp_eq

# cmp_fst: 
#     li $v0, 1
#     b return
# cmp_snd:
#     li $v0, 1
#     b return
# cmp_eq:
#     li $v0, 0
#     b return

read_char:
    andi $v0, $v0, 0
    li $v0, 0x30000000
    lw $v0, 0($v0)

    li $t1, 0xFFFFFFFF # EOF
    beq $v0, $t1, exit

    jr $ra

# Print the character in the .text-relative memory location until a NULL is
# encountered
print:
    and $t0, $t0, $0
    li $t0, 0x10000000
    addu $a0, $a0, $t0

print_i:
    lb $t1, ($a0)
    beq $t1, $0, return

    andi $at, $at, 0
    sw $t1, putc
    add $a0, $a0, 1
    j print_i


return: jr $ra
exit: jr $0

dprompt: .asciz "Guess> "
dwin: .asciz "You win!\n"
dhigher: .asciz "Go higher\n"
dlower: .asciz "Go lower\n"
