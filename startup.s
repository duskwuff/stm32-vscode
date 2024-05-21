.syntax unified

.global Reset_Handler
.weak SystemInit

.section .text.Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    bl      SystemInit

    ldr     r1, =_sidata
    ldr     r2, =_sdata
    ldr     r3, =_edata
    b       .L_init_data_enter

.L_init_data:
    ldmia   r1!, {r0}
    stmia   r2!, {r0}
.L_init_data_enter:
    cmp     r2, r3
    blt     .L_init_data

    b       _start
.size Reset_Handler, .-Reset_Handler

.section .text.SystemInit
.type SystemInit, %function
SystemInit:
    bx	    lr
.size SystemInit, .-SystemInit
