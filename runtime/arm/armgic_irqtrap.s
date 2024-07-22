/* Companion source of s-bbcppr.adb for ARM with GIC interrupt controller */
/* Note: strictly speaking, __gnat_set_fpu_enabled could be part of s-bbcppr,
   but we need to make sure that the object generated here is bundled with
   the user application (so that the base __gnat_irq_trap weak symbol is
   replaced by the below one). Having s-bbcppr dragging this source ensures
   that. */

.text

        .global __gnat_irq_trap
        .global __gnat_set_fpu_enabled

       	.type	__gnat_irq_trap %function
__gnat_irq_trap:
        /* In order to allow nested IRQ support, we need to save the banked
           registers LR and SPSR, and change CPU mode before allowing
           IRQs to interrupt us. Otherwise LR[IRQ] might get corrupted, and
           thus the interrupted handler be corrupted.
           The runtime uses the SVC mode to run the IRQ handlers.
        */
        sub     lr, lr, #4      @ adjust LR to the proper insn to srsfd
        srsfd   sp!, #19        @ use SRS to save LR_irq and SPSR_irq in one
                                @ step onto the SVC stack
        cps     #19             @ switch to SVC mode
        push    {r0-r3, r12}    @ save scratch registers
        and     r1, sp, #4      @ ensure that stack is 8-byte aligned
        sub     sp, sp, r1
        push    {r1, lr}        @ store adjustment and LR_svc to stack
        bl      __gnat_irq_trap_ada
        pop     {r1, lr}        @ restore adjustment and LR_svc
        add     sp, sp, r1      @ unadjust stack
        pop     {r0-r3, r12}    @ restore scratch registers
        rfefd   sp!             @ return using rfe from the irq handler
	.size	__gnat_irq_trap, . - __gnat_irq_trap

        /* R0: 0 to disable fpu, 1 to enable it */
        .type   __gnat_set_fpu_enabled %function
__gnat_set_fpu_enabled:
        lsl     r0, r0, #30
        vmsr    fpexc, r0
        bx      lr
        .size __gnat_set_fpu_enabled, . - __gnat_set_fpu_enabled
