; //
; //  mandelbrot.asm
; //  asmManbrt
; //
; //  Created by Emil Bahnsen on 06-08-15.
; //  Copyright © 2015 Emil Bahnsen. All rights reserved.
; //

; https://github.com/yasm/yasm/wiki/NasmSyntax
struc point
	.x:	resq 1
	.y:	resq 1
endstruc

struc size
	.height:	resq 1
	.width:	resq 1
endstruc

struc rect
	.origin:	resb point_size
	.size:	resb size_size
endstruc

extern _sprintf

segment .data
	unboundCutoff	dq		2.0
	_2					dq		2.0
	_3					dq		3.0
	mdlItters		db		255
	colourUpscale	dd		14.09		; 255 / sqrt(255)

; Number of max itterrations over the manderbort quality
%define	mdlItter	255
%define	mdlItterOneRhird	85
%define	mdlItterTwoThird	127	; 127.5
%define	n2ColourRatio		3
mappedColour:
%assign n	0
%rep mdlItter
	%assign r	0
	%assign g	0
	%assign b	0
	%if n > 0 && n < mdlItterOneRhird					; first third
		%assign v	n
		%assign r	-n2ColourRatio*v + 255
		%assign g	n2ColourRatio*v
	%endif
	%if n > mdlItterOneRhird && n < mdlItterTwoThird	; second third
		%assign v	n - mdlItterOneRhird
		%assign g	-n2ColourRatio*v + 255
		%assign b	n2ColourRatio*v
	%endif
	%if n > mdlItterTwoThird && n < mdlItter		; thind third
		%assign v	n - mdlItterTwoThird
		%assign b	-n2ColourRatio*v + 255
		%assign r	n2ColourRatio*v
	%endif
	; Store the values
	db	r
	db	g
	db	b
	%assign n	n+1
%endrep

segment .text

; C-structs to parse to the function
; typedef struct _Point {
; 	double x,y;
; } Point;
;
; typedef struct _Size {
; 	double height, width;
; } Size;
;
; typedef struct _Rect {
; 	Point origin;
; 	Size size;
; } Rect;

; int mandelbrotb(Rect* rect, Size* outSize, const int8_t* outBuf)
%define				rect rdi
%define								outSize rsi
%define													outBuf rdx
global _mandelbrotb
_mandelbrotb:
; R0  R1  R2  R3  R4  R5  R6  R7  R8  R9  R10  R11  R12  R13  R14  R15
; RAX RCX RDX RBX RSP RBP RSI RDI
; xmm0 .. 15
; xmm0 .. 7 (128 bit), .. 15 (64 bit) <– that's perhaps not the case
; (the calle-save registers) are: rbp, rbx, r12, r13, r14, r15
; Z_n+1 = Z_n^2 + c
; Itterrations over this complex plane contaning the coordinates c
; will, as long as these values stay bounded, be the mandelbrot set

; moves a double-FP values from memory into
; both row and high of an xmm-register
; %macro movhl	2
; 	movsd		%1, %2
; 	movhpd	%1, %2
; %endmacro
; OBS: This is done by movddup

; Don't be a rudeman
push rbp
push rbx
push rsp
push r12
push r13
push r14
push r15

; (a + bi)^2 + c
; a^2 + bi^2 + 2abi + c, c = d + ei
; a^2 - b^2 + d  +  2abi + ei
%macro mdl	4	; a, bi, d, ei [c]
	; Real part, is stored back into a (%1)
	movapd	xmmt1, %1	; a
	mulpd		%1, %1		; a^2
	movapd	xmmt2, %2	; b
	mulpd		xmmt2, %2	; b^2
	subpd		%1, xmmt2	; a^2 - b^2
	addpd		%1, %3		; a^2 - b^2 + d
	; Imaginary part, is stored back into b (%2)
	mulpd		%2, xmmt1	; bi·a
	mulpd		%2, xVal2	; bi·a·2
	addpd		%2, %4		; bi·a·2 + ei
%endmacro

; (a + bi)^3 + c, c = d + ei
;	1	 2					 3	  4	
%macro mdl3	4
	; Real part
	movapd	xmmt1, %1		; a
	mulpd		%1, %1			; a^3
	mulpd		%1, %1			;
	movapd	xmmt2, %2		; b
	mulpd		xmmt2, xmmt1	; b·a
	mulpd		xmmt2, xVal3	; b·a·3
	subpd		%1, xmmt2		; a^3 - 3·a·b
	addpd		%1, %3			; a^3 - 3·a·b + d
	; Imaginary part
	mulpd		xmmt1, xmmt1	; a^2
	mulpd		xmmt1, %2		; a^2·bi
	mulpd		xmmt1, xVal3	; a^2·bi·3
	mulpd		%2, %2			; b^3 i
	mulpd		%2, %2			;
	subpd		xmmt1, %2		; 3·a^3·b - b^3 i
	movapd	%2, xmmt1		; move to dest
	addpd		%2, %4			; 3·a^3·b - b^3 i + ei
%endmacro

; Modulus of complex number
; a + bi
; sqrt(a^2 + b^2)
%macro zLen	2
	mulpd		%1, %1			; a^2
	movapd	xmmt1, %2		; copy b
	mulpd		xmmt1, xmmt1	; b^2
	addpd		%1, xmmt1		; a^2 + b^2
	sqrtpd	%1, %1			; sqrt(a^2 + b^2)
%endmacro

; 
%macro zLenGe2 2
	mulpd		%1, %1			; a^2
	movapd	xmmt1, %2		; copy b
	mulpd		xmmt1, xmmt1	; b^2
	addpd		%1, xmmt1		; a^2 + b^2
	sqrtpd	%1, %1			; sqrt(a^2 + b^2)
%endmacro

; Static registers used to speed tings up
%define	xmma	xmm0	; a
%define	xmmbi	xmm1	; bi
%define	xmmd	xmm2	; d, c = d + ei
%define	xmmds	xmm6	; Static d for resetting
%define	xmmei	xmm3	; ei
%define	xmmdx	xmm4	; ∆x-incremental value
%define	xmmdy	xmm5	; ∆y-incremental value
%define	rYit	r8		; y-axis itterrations
%define	rXit	r9		; x-axis itterrations
%define	rXits	r10	; Static x-axis itterrations for resitting
%define	xmmct	xmm7	; Unbound cutoff value
%define	rUbS1	r11d	; Unbound stamp 1
%define	rUbS2	r12d	; Unbound stamp 2
%define	xVal2	xmm8	; the value 2 (as two Double-FP), used by the mdl macro
movddup	xVal2, [rel _2]
%define	xVal3	xmm14	; the value 2 (as two Double-FP), used by the mdl macro
movddup	xVal3, [rel _3]
%define	xUps	xmm9
movss		xUps, [rel colourUpscale]
%define	aMap	r13
lea		aMap, [rel mappedColour]
; Written to the pixels and meant to role over when it reathes 255
%define	rorIdx	bx

; These will be used to store values temporarily
%define	xmmt1	xmm10	; these will be used by macros
%define	xmmt2	xmm11	;
%define	xmmT1	xmm12	; and these will be used by anyone exclusively
%define	xmmT2	xmm13	;

; Map the tree input pointers to more convinient registres
mov		r13, rect
mov		r14, outSize
mov		r15, outBuf
%define	rect		r13
%define	outSize	r14
%define	outBuf	r15

; Invert y-axis on input rect
movsd	xmmT1, [rect + rect.origin + point.y]		; y_i = h + y
movsd	xmmT2, [rect + rect.size + size.height]
addsd	xmmT1, xmmT2
movsd	[rect + rect.origin + point.y], xmmT1

; xmmdx and xmmdy will hold the step size in each direction
; xmmdx (x_step: two double-FP value)
; xmmdy (y_step: two double-FP value)
movddup	xmmdy, [rect + rect.size + size.height]
movddup	xmmdx, [rect + rect.size + size.width]
movddup	xmmT1, [outSize + size.height]	; These two will be used leater
movddup	xmmT2, [outSize + size.width]
divpd		xmmdy, xmmT1			; Rect.size / outSize.size = step.size
divpd		xmmdx, xmmT2
movddup	xmmT1, [rel _2]		; as two x-values are calculated at a time
mulpd		xmmdx, xmmT1			;

; xmmd and xmmei are d and ei respectively in c = d + ei
movddup	xmmds, [rect + rect.origin + point.x]		; xmmd will hold the dynamic value (done at begining op loop)
movddup	xmmei, [rect + rect.origin + point.y]		;
; Advance the second (low) value to contain the next pixel (x-value)
movsd		xmmT1, xmmdx		; only the half though, as the dx is double for two pixels
divsd		xmmT1, [rel _2]	;
addsd		xmmds, xmmT1

; Calculate how many itterrations there are in eath direction, i.e. pixels
movsd		xmmT1, [outSize + size.height]		; allready loaded above
movsd		xmmT2, [outSize + size.width]
cvtsd2si	rYit, xmmT1
cvtsd2si	rax, xmmT2
xor		rdx, rdx		; Must be zero before divition, for some reason (remainder is stored here)
mov		rcx, 2		; divide by 2 as two x-values are calculated at a time
div		rcx			;
mov		rXits, rax	; save this to reset the loop counter quickly (rXit)

; Load the cutoff threshold of 2 DFP
movddup	xmmct, [rel unboundCutoff]

Ly:
	mov		rXit, rXits	; reset the x-loop counter
	movapd	xmmd, xmmds	; reset the x coordinate
	
	Lx:
	
		mov	ecx, mdlItter	; reset loop counter and unbound stamps
		mov	rUbS1, -1			;
		mov	rUbS2, -1			;
		; Assume one (simple) itterration
		; Z = 0^2 + c = c => d + ei = a + bi
		movapd	xmma, xmmd
		movapd	xmmbi, xmmei
		; or perhaps not...?
		; xorpd	xmma, xmma
		; xorpd	xmmbi, xmmbi
		; int 3
		; nop
		Lmdl:
			mdl		xmma, xmmbi, xmmd, xmmei
			
			; calculate the modulus of the complex value
			movapd	xmmT1, xmma
			zLen		xmmT1, xmmbi
			
			; compare this to the threshold
			movapd	xmmT2, xmmct			; Do not override the cutoffvalue
			cmppd		xmmT2, xmmT1, 001B	; 001B-predicate -> A < B, will change the sign mask according to the result
			movmskpd	rdx, xmmT2				; extract the sign mask
			
			; if the first Double-FP is true note ecx in rUbS1
			cmp	rUbS1, -1		; skip the next instuctions if the unbound time stamp has allready been set, i.e. above 0
			jg		LUb1Skip			;
			mov	rax, rdx			; copy the signmask-extract
			and	rax, 0b01		; and it with this pixel
			cmp	rax, 0			; compare if it excist
			cmova	rUbS1, ecx	; store the time stamp of "becomming" unbound on the condition of rax being the number above
			LUb1Skip:
			
			cmp	rUbS2, -1		; the same as above, but with the next pixel
			jg		LUb2Skip
			and	rdx, 0b10
			cmp	rdx, 0
			cmova	rUbS2, ecx
			LUb2Skip:
			
			; if rUbS1 and rUbS2 both not are -1, then break the loop
			; avoiding any retundant itterrations
			cmp	rUbS1, -1
			je		LoutSkip
			cmp	rUbS2, -1
			jne	LmdlSkip
			LoutSkip:
			;cmovg	ecx, eax		;
			
			; loop	Lmdl		: hmm... this thing becomes out of range, more than 127 bytes from label
			dec	ecx
			jnz	Lmdl
			LmdlSkip:
			
			%macro	sqrtInt	1			; newValue = sqrt(value) * (255/sqrt(255))		to boost the contrast a bit
				cvtsi2ss	xmmt1, %1
				sqrtss	xmmt1, xmmt1
				mulss		xmmt1, xUps
				cvtss2si	%1, xmmt1
			%endmacro

			%macro	sqrtInt2	1			; newValue = sqrt(value * 255)		to boost the contrast a bit
				cvtsi2ss	xmmt1, %1
				addss		xmmt1, 0xFF
				sqrtss	xmmt1, xmmt1
				cvtss2si	%1, xmmt1
			%endmacro
			
			; Write the unbound time stamps to memory
			sqrtInt	rUbS1
			; mov	eax, mdlItter
			; sub	eax, rUbS1
			mov	[outBuf], rUbS1
			inc	outBuf
			
			sqrtInt	rUbS2
			; mov	eax, mdlItter
			; sub	eax, rUbS2
			mov	[outBuf], rUbS2
			inc 	outBuf

			; %macro writeBuf	1
			; 	mov	rax, aMap
			; 	imul	%1, 3
			; 	add	rax, %1
			; 	mov	al, [rax]
			; 	mov	[outBuf], al
			; 	inc	rax
			; 	mov	al, [rax]
			; 	mov	[outBuf+1], al
			; 	inc	rax
			; 	mov	al, [rax]
			; 	mov	[outBuf+2], rax
			; 	add	outBuf, 3
			; %endmacro

			; writeBuf	r11
			; writeBuf	r12
			
			
			
		addpd	xmmd, xmmdx		; increment the x coordinate
		dec	rXit
		cmp	rXit, 0
		jne	Lx					; jump if not zero
		
	
	subpd	xmmei, xmmdy	; increment the y coordinate, i.e. decrement as we're starting in top left
	dec	rYit
	cmp	rYit, 0
	jne	Ly					; jump if not zero

mov rax, 0	; sucess

pop r15
pop r14
pop r13
pop r12
pop rsp
pop rbx
pop rbp

ret
