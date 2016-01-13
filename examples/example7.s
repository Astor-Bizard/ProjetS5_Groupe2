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
	.file	"example7.c"
	.text
	.align	2
	.global	f1
	.type	f1, %function
f1:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	ldr	r3, [fp, #-8]
	cmp	r3, #5
	ble	.L2
	ldr	r3, [fp, #-8]
	add	r3, r3, #1
	mov	r0, r3
	bl	f2
	mov	r3, r0
	b	.L3
.L2:
	ldr	r3, [fp, #-8]
	mov	r3, r3, asl #1
	mov	r0, r3
	bl	f3
	mov	r3, r0
.L3:
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	f1, .-f1
	.align	2
	.global	f2
	.type	f2, %function
f2:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	ldr	r3, [fp, #-8]
	cmp	r3, #5
	bgt	.L5
	mov	r3, #42
	b	.L6
.L5:
	ldr	r0, [fp, #-8]
	bl	f4
	mov	r3, r0
.L6:
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	f2, .-f2
	.align	2
	.global	f3
	.type	f3, %function
f3:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	str	r0, [fp, #-8]
	ldr	r3, [fp, #-8]
	mov	r3, r3, asl #1
	add	r3, r3, #3
	mov	r2, r3, lsr #31
	add	r3, r2, r3
	mov	r3, r3, asr #1
	mov	r2, r3
	ldr	r3, [fp, #-8]
	ldr	r1, .L8
	smull	r0, r1, r3, r1
	mov	r1, r1, asr #2
	mov	r3, r3, asr #31
	rsb	r3, r3, r1
	add	r3, r2, r3
	mov	r0, r3
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
.L9:
	.align	2
.L8:
	.word	1717986919
	.size	f3, .-f3
	.align	2
	.global	f4
	.type	f4, %function
f4:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	ldr	r3, [fp, #-8]
	cmp	r3, #9
	bgt	.L11
	ldr	r0, [fp, #-8]
	bl	f1
	mov	r3, r0
	b	.L12
.L11:
	ldr	r3, [fp, #-8]
.L12:
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	f4, .-f4
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	str	r0, [fp, #-16]
	str	r1, [fp, #-20]
	ldr	r3, [fp, #-20]
	add	r3, r3, #4
	ldr	r3, [r3, #0]
	ldrb	r3, [r3, #0]	@ zero_extendqisi2
	str	r3, [fp, #-8]
	ldr	r0, [fp, #-8]
	bl	f1
	mov	r3, #0
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	main, .-main
	.ident	"GCC: (GNU) 4.5.3"
