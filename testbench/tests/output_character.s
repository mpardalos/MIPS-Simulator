# Outputs the character 'A' to the console
.set noat
.text
    ori $11, $11, 3
    sll $11, $11, 28
    ori $1, $1, 65
    sw  $1, 4($11)
    jr  $0
