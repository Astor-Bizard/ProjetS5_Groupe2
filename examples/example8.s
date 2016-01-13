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
	.file	"example8.c"
	.text
	.align	2
	.global	ArbreVide
	.type	ArbreVide, %function
ArbreVide:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	mov	r3, #0
	mov	r0, r3
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	ArbreVide, .-ArbreVide
	.align	2
	.global	NouveauNoeud
	.type	NouveauNoeud, %function
NouveauNoeud:
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #24
	str	r0, [fp, #-16]
	mov	r3, r1
	str	r2, [fp, #-24]
	strb	r3, [fp, #-17]
	mov	r0, #12
	bl	malloc
	mov	r3, r0
	str	r3, [fp, #-8]
	ldr	r3, [fp, #-8]
	ldrb	r2, [fp, #-17]
	strb	r2, [r3, #0]
	ldr	r3, [fp, #-8]
	ldr	r2, [fp, #-16]
	str	r2, [r3, #8]
	ldr	r3, [fp, #-8]
	ldr	r2, [fp, #-24]
	str	r2, [r3, #4]
	ldr	r3, [fp, #-8]
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	NouveauNoeud, .-NouveauNoeud
	.align	2
	.global	EstVide
	.type	EstVide, %function
EstVide:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	str	r0, [fp, #-8]
	ldr	r3, [fp, #-8]
	cmp	r3, #0
	movne	r3, #0
	moveq	r3, #1
	mov	r0, r3
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	EstVide, .-EstVide
	.align	2
	.global	Etiq
	.type	Etiq, %function
Etiq:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	str	r0, [fp, #-8]
	ldr	r3, [fp, #-8]
	ldrb	r3, [r3, #0]	@ zero_extendqisi2
	mov	r0, r3
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	Etiq, .-Etiq
	.align	2
	.global	FilsGauche
	.type	FilsGauche, %function
FilsGauche:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	str	r0, [fp, #-8]
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #8]
	mov	r0, r3
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	FilsGauche, .-FilsGauche
	.align	2
	.global	FilsDroit
	.type	FilsDroit, %function
FilsDroit:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	sub	sp, sp, #12
	str	r0, [fp, #-8]
	ldr	r3, [fp, #-8]
	ldr	r3, [r3, #4]
	mov	r0, r3
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	FilsDroit, .-FilsDroit
	.align	2
	.global	LibererArbre
	.type	LibererArbre, %function
LibererArbre:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	ldr	r0, [fp, #-8]
	bl	EstVide
	mov	r3, r0
	cmp	r3, #0
	bne	.L7
	ldr	r0, [fp, #-8]
	bl	FilsGauche
	mov	r3, r0
	mov	r0, r3
	bl	LibererArbre
	ldr	r0, [fp, #-8]
	bl	FilsDroit
	mov	r3, r0
	mov	r0, r3
	bl	LibererArbre
	ldr	r0, [fp, #-8]
	bl	free
.L7:
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	LibererArbre, .-LibererArbre
	.comm	cpt_noeud,4,4
	.align	2
	.global	EcrireArbreRec
	.type	EcrireArbreRec, %function
EcrireArbreRec:
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #24
	str	r0, [fp, #-24]
	str	r1, [fp, #-28]
	ldr	r0, [fp, #-28]
	bl	EstVide
	mov	r3, r0
	cmp	r3, #0
	beq	.L10
	mov	r3, #0
	b	.L11
.L10:
	ldr	r0, [fp, #-28]
	bl	FilsGauche
	mov	r3, r0
	ldr	r0, [fp, #-24]
	mov	r1, r3
	bl	EcrireArbreRec
	mov	r3, r0
	str	r3, [fp, #-8]
	ldr	r0, [fp, #-28]
	bl	FilsDroit
	mov	r3, r0
	ldr	r0, [fp, #-24]
	mov	r1, r3
	bl	EcrireArbreRec
	mov	r3, r0
	str	r3, [fp, #-12]
	ldr	r3, [fp, #-8]
	cmp	r3, #0
	bne	.L12
	ldr	r0, [fp, #-28]
	bl	Etiq
	mov	r3, r0
	str	r3, [fp, #-16]
	b	.L13
.L12:
	ldr	r3, .L14
	ldr	r3, [r3, #0]
	str	r3, [fp, #-16]
	ldr	r3, .L14
	ldr	r3, [r3, #0]
	add	r2, r3, #1
	ldr	r3, .L14
	str	r2, [r3, #0]
.L13:
	sub	r3, fp, #16
	mov	r0, r3
	mov	r1, #4
	mov	r2, #1
	ldr	r3, [fp, #-24]
	bl	fwrite
	sub	r3, fp, #8
	mov	r0, r3
	mov	r1, #4
	mov	r2, #1
	ldr	r3, [fp, #-24]
	bl	fwrite
	sub	r3, fp, #12
	mov	r0, r3
	mov	r1, #4
	mov	r2, #1
	ldr	r3, [fp, #-24]
	bl	fwrite
	ldr	r3, [fp, #-16]
.L11:
	mov	r0, r3
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
.L15:
	.align	2
.L14:
	.word	cpt_noeud
	.size	EcrireArbreRec, .-EcrireArbreRec
	.align	2
	.global	EcrireArbre
	.type	EcrireArbre, %function
EcrireArbre:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	str	r0, [fp, #-16]
	str	r1, [fp, #-20]
	mov	r3, #0
	str	r3, [fp, #-8]
	ldr	r3, .L17
	mov	r2, #256
	str	r2, [r3, #0]
	ldr	r0, [fp, #-16]
	ldr	r1, [fp, #-20]
	bl	EcrireArbreRec
	sub	r3, fp, #8
	mov	r0, r3
	mov	r1, #4
	mov	r2, #1
	ldr	r3, [fp, #-16]
	bl	fwrite
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
.L18:
	.align	2
.L17:
	.word	cpt_noeud
	.size	EcrireArbre, .-EcrireArbre
	.align	2
	.global	LireArbre
	.type	LireArbre, %function
LireArbre:
	@ args = 0, pretend = 0, frame = 2080
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {r4, fp, lr}
	add	fp, sp, #8
	sub	sp, sp, #2080
	sub	sp, sp, #4
	str	r0, [fp, #-2088]
	bl	ArbreVide
	mov	r3, r0
	str	r3, [fp, #-2068]
	mov	r3, #0
	str	r3, [fp, #-16]
	sub	r3, fp, #2048
	sub	r3, r3, #12
	sub	r3, r3, #12
	mov	r0, r3
	mov	r1, #4
	mov	r2, #1
	ldr	r3, [fp, #-2088]
	bl	fread
	b	.L20
.L23:
	sub	r3, fp, #2064
	sub	r3, r3, #12
	mov	r0, r3
	mov	r1, #4
	mov	r2, #1
	ldr	r3, [fp, #-2088]
	bl	fread
	sub	r3, fp, #2064
	sub	r3, r3, #12
	sub	r3, r3, #4
	mov	r0, r3
	mov	r1, #4
	mov	r2, #1
	ldr	r3, [fp, #-2088]
	bl	fread
	ldr	r3, [fp, #-2076]
	cmp	r3, #0
	bne	.L21
	ldr	r3, [fp, #-2072]
	strb	r3, [fp, #-17]
	b	.L22
.L21:
	mov	r3, #97
	strb	r3, [fp, #-17]
.L22:
	ldr	r4, [fp, #-2072]
	ldr	r2, [fp, #-2076]
	mvn	r3, #2048
	sub	r3, r3, #7
	mov	r2, r2, asl #2
	sub	r0, fp, #12
	add	r2, r0, r2
	add	r3, r2, r3
	ldr	r1, [r3, #0]
	ldr	r2, [fp, #-2080]
	mvn	r3, #2048
	sub	r3, r3, #7
	mov	r2, r2, asl #2
	sub	r0, fp, #12
	add	r2, r0, r2
	add	r3, r2, r3
	ldr	r3, [r3, #0]
	ldrb	r2, [fp, #-17]	@ zero_extendqisi2
	mov	r0, r1
	mov	r1, r2
	mov	r2, r3
	bl	NouveauNoeud
	mov	r2, r0
	mvn	r3, #2048
	sub	r3, r3, #7
	mov	r1, r4, asl #2
	sub	r0, fp, #12
	add	r1, r0, r1
	add	r3, r1, r3
	str	r2, [r3, #0]
	ldr	r3, [fp, #-2072]
	str	r3, [fp, #-16]
	sub	r3, fp, #2048
	sub	r3, r3, #12
	sub	r3, r3, #12
	mov	r0, r3
	mov	r1, #4
	mov	r2, #1
	ldr	r3, [fp, #-2088]
	bl	fread
.L20:
	ldr	r3, [fp, #-2072]
	cmp	r3, #0
	bne	.L23
	ldr	r2, [fp, #-16]
	mvn	r3, #2048
	sub	r3, r3, #7
	mov	r2, r2, asl #2
	sub	r1, fp, #12
	add	r2, r1, r2
	add	r3, r2, r3
	ldr	r3, [r3, #0]
	mov	r0, r3
	sub	sp, fp, #8
	ldmfd	sp!, {r4, fp, pc}
	.size	LireArbre, .-LireArbre
	.section	.rodata
	.align	2
.LC0:
	.ascii	"%c\012\000"
	.text
	.align	2
	.global	AfficherArbreRec
	.type	AfficherArbreRec, %function
AfficherArbreRec:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	str	r0, [fp, #-16]
	str	r1, [fp, #-20]
	ldr	r0, [fp, #-16]
	bl	EstVide
	mov	r3, r0
	cmp	r3, #0
	bne	.L24
	mov	r3, #0
	str	r3, [fp, #-8]
	b	.L26
.L27:
	mov	r0, #32
	bl	putchar
	ldr	r3, [fp, #-8]
	add	r3, r3, #1
	str	r3, [fp, #-8]
.L26:
	ldr	r2, [fp, #-8]
	ldr	r3, [fp, #-20]
	cmp	r2, r3
	blt	.L27
	ldr	r0, [fp, #-16]
	bl	Etiq
	mov	r3, r0
	ldr	r0, .L28
	mov	r1, r3
	bl	printf
	ldr	r0, [fp, #-16]
	bl	FilsGauche
	mov	r2, r0
	ldr	r3, [fp, #-20]
	add	r3, r3, #2
	mov	r0, r2
	mov	r1, r3
	bl	AfficherArbreRec
	ldr	r0, [fp, #-16]
	bl	FilsDroit
	mov	r2, r0
	ldr	r3, [fp, #-20]
	add	r3, r3, #2
	mov	r0, r2
	mov	r1, r3
	bl	AfficherArbreRec
.L24:
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
.L29:
	.align	2
.L28:
	.word	.LC0
	.size	AfficherArbreRec, .-AfficherArbreRec
	.align	2
	.global	AfficherArbre
	.type	AfficherArbre, %function
AfficherArbre:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	ldr	r0, [fp, #-8]
	mov	r1, #0
	bl	AfficherArbreRec
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	AfficherArbre, .-AfficherArbre
	.align	2
	.global	AfficherArbreFichRec
	.type	AfficherArbreFichRec, %function
AfficherArbreFichRec:
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #24
	str	r0, [fp, #-16]
	str	r1, [fp, #-20]
	str	r2, [fp, #-24]
	ldr	r0, [fp, #-16]
	bl	EstVide
	mov	r3, r0
	cmp	r3, #0
	bne	.L31
	mov	r3, #0
	str	r3, [fp, #-8]
	b	.L33
.L34:
	mov	r3, #32
	strb	r3, [fp, #-9]
	ldrb	r3, [fp, #-9]	@ zero_extendqisi2
	mov	r0, r3
	ldr	r1, [fp, #-24]
	bl	fputc
	ldr	r3, [fp, #-8]
	add	r3, r3, #1
	str	r3, [fp, #-8]
.L33:
	ldr	r2, [fp, #-8]
	ldr	r3, [fp, #-20]
	cmp	r2, r3
	blt	.L34
	ldr	r0, [fp, #-16]
	bl	Etiq
	mov	r3, r0
	mov	r0, r3
	ldr	r1, [fp, #-24]
	bl	fputc
	mov	r3, #10
	strb	r3, [fp, #-9]
	ldrb	r3, [fp, #-9]	@ zero_extendqisi2
	mov	r0, r3
	ldr	r1, [fp, #-24]
	bl	fputc
	ldr	r0, [fp, #-16]
	bl	FilsGauche
	mov	r2, r0
	ldr	r3, [fp, #-20]
	add	r3, r3, #2
	mov	r0, r2
	mov	r1, r3
	ldr	r2, [fp, #-24]
	bl	AfficherArbreFichRec
	ldr	r0, [fp, #-16]
	bl	FilsDroit
	mov	r2, r0
	ldr	r3, [fp, #-20]
	add	r3, r3, #2
	mov	r0, r2
	mov	r1, r3
	ldr	r2, [fp, #-24]
	bl	AfficherArbreFichRec
.L31:
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	AfficherArbreFichRec, .-AfficherArbreFichRec
	.align	2
	.global	AfficherArbreFich
	.type	AfficherArbreFich, %function
AfficherArbreFich:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
	str	r0, [fp, #-8]
	str	r1, [fp, #-12]
	ldr	r0, [fp, #-8]
	mov	r1, #0
	ldr	r2, [fp, #-12]
	bl	AfficherArbreFichRec
	sub	sp, fp, #4
	ldmfd	sp!, {fp, pc}
	.size	AfficherArbreFich, .-AfficherArbreFich
	.ident	"GCC: (GNU) 4.5.3"
