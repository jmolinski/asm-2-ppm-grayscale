.data
R:  .word 0
.balign 4
G:  .word 0
.balign 4
B:  .word 0
.balign 4

.global R, G, B

.text

.global to_grayscale

to_grayscale:
    @ arguments:
    @ r0 - pointer to 1d array of 3*rows*columns ints {RGB}
    @ r1 - rows number
    @ r2 - columns number

    @prologue
    str lr, [sp,#-4]!   @ store lr on the stack (decreasing sp by 4 bytes before load)
    stmdb sp!, {r4-r8}  @ preserve registers on the stack

    mul r3, r1, r2      @ r3 = number of pixels to process (controlling the loop)
    mov r2, #3
    mul r1, r2, r3
    @ r0: uint[], r1: number of pixels * 3

    @ if cells = 0, goto epilogue
    subs r2, r1, #0
    beq .finish

    @ load R, G, B coefficients from memory
    ldr r2, =R
    ldr r6, [r2]
    ldr r2, =G
    ldr r7, [r2]
    ldr r2, =B
    ldr r8, [r2]

    mov	r2, #0	@ i
    @ r0: uint32_t[] matrix, r1: number of pixels * 3, r2: i, r6: R, r7: G, r8: B
.loop:
    ldr	r3, [r0, #0]    @ load pixel's red value
    mul	r4, r3, r6	    @ r4 = red * R

    ldr	r3, [r0, #4]    @ load pixel's green value
    mla r5, r3, r7, r4  @ r5 = red * R + green * G

    ldr	r3, [r0, #8]    @ load pixel's blue value
    mla r4, r3, r8, r5 @ r4 = red * R + green * G + blue * B

    mov r4, r4, lsr #8  @ r4 = (red * R + green * G + blue * B) // 256 = grayscale

    str r4, [r0]        @ save grayscale value to matrix - only overwrite red, other fields should be discarded
    add r0, #12

    add	r2, r2, #3	    @ i += 3
    cmp	r1, r2	        @ if i < cells then loop
    bhi	.loop

.finish:
    @ epilogue
    ldmia sp!, {r4-r8} @ restore preserved registers from the stack
    ldr lr, [sp], #4 @ load return address from the stack & reset stack
    bx lr @ jump to return address
