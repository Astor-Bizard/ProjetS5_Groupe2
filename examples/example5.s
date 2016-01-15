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
	.file	"example5.c"
	.global	coucou
	.data
	.align	2
	.type	coucou, %object
	.size	coucou, 4
coucou:
	.word	1
	.text
	.align	2
	.global	main
	.type	main, %function
main:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	mov	r3, #9
	str	r3, [fp, #-8]
	ldr	r3, [fp, #-8]
	mov	r3, r3, asl #1
	str	r3, [fp, #-12]
	ldr	r3, .L2
	mov	r2, #2
	str	r2, [r3, #0]
	mov	r3, #0
	mov	r0, r3
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
.L3:
	.align	2
.L2:
	.word	coucou
	.size	main, .-main
	.ident	"GCC: (GNU) 4.5.3"
