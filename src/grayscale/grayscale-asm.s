.data 
    R_: .word 0
    .balign 4
    G_: .word 0
    .balign 4
    B_: .word 0
    .balign 4

.global R_, G_, B_, grayscale

.text

grayscale:
    @ arguments
    @ r0 - matrix pointer
    @ r1 - number of rows
    @ r2 - number of columns

    str lr, [sp, #-4]!
    stmdb sp!, {r4-r8}

prepare_limits: 
    @ r1 - number of cells (rows * columns)
    mul r3, r1, r2
    mov r1, r3

prepare_constants: 
    @ Red
    ldr r4, =R_
    ldr r6, [r4]

    @ Green
    ldr r4, =G_
    ldr r7, [r4]

    @ Blue
    ldr r4, =B_
    ldr r8, [r4]

for_loop: 
    @ r0 - matrix pointer (at cell)
    @ r1 - number of cells (from 0)
    @ r6 - red
    @ r7 - green
    @ r8 - blue
    
    mov r5, #0

    @ Red
    ldr r2, [r0, #0]
    mla r5, r2, r6, r5

    @ Green
    ldr r3, [r0, #4]
    mla r5, r3, r7, r5

    @ Blue
    ldr r4, [r0, #8]
    mla r5, r4, r8, r5

    @ Shift right by 8 - divide by 256
    lsr r5, #8

    str r5, [r0, #0]
    str r5, [r0, #4]
    str r5, [r0, #8]

    @ Move pointer to next cell
    add r0, #12
    @ Update iterator
    sub r1, #1

    @ Check if there are cells to update
    cmp r1, #0 
    bne for_loop

at_end:
    ldmia sp!, {r4 - r8} 
    ldr lr, [sp], #4
    bx lr
