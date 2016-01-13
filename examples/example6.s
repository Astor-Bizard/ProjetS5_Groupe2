	.cpu arm7tdmi
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 18, 4
	.file	"example6.c"
	.text
	.align	2
	.global	truc
	.type	truc, %function
truc:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #20
	str	r0, [fp, #-8]
	mov	r3, r1
	str	r2, [fp, #-16]
	strb	r3, [fp, #-9]
	ldrb	r3, [fp, #-9]	@ zero_extendqisi2
	ldr	r2, [fp, #-8]
	mul	r2, r3, r2
	ldr	r3, [fp, #-16]
	add	r3, r2, r3
	mov	r0, r3
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	truc, .-truc
	.section	.rodata
	.align	2
.LC0:
	.ascii	"%d %d cou%ccou\000"
	.align	2
.LC1:
	.ascii	"%d\000"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	mov	r3, #79
	strb	r3, [fp, #-5]
	mov	r3, #9
	str	r3, [fp, #-12]
	ldr	r3, [fp, #-12]
	mov	r3, r3, asl #1
	str	r3, [fp, #-16]
	ldrb	r3, [fp, #-5]	@ zero_extendqisi2
	ldr	r0, .L3
	ldr	r1, [fp, #-12]
	ldr	r2, [fp, #-16]
	bl	printf
	ldrb	r3, [fp, #-5]	@ zero_extendqisi2
	ldr	r0, [fp, #-12]
	mov	r1, r3
	ldr	r2, [fp, #-16]
	bl	truc
	mov	r3, r0
	ldr	r0, .L3+4
	mov	r1, r3
	bl	printf
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
.L4:
	.align	2
.L3:
	.word	.LC0
	.word	.LC1
	.size	main, .-main
	.ident	"GCC: (GNU) 4.5.3"
