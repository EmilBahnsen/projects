; //
; //  mandelbrot.asm
; //  asmManbrt
; //
; //  Created by Emil Bahnsen on 06-08-15.
; //  Copyright © 2015 Emil Bahnsen. All rights reserved.
; //

; http://www.opensource.apple.com/source/xnu/xnu-1504.3.12/bsd/kern/syscalls.master
; System calls
%define	sys_exit		0x2000001
%define	sys_write	0x2000004	; user_ssize_t write(int fd, user_addr_t cbuf, user_size_t nbyte);
%define	sys_open		0x2000005
%define	sys_close	0x2000006
%define	sys_unlink	0x2000010

%define	sys_stdin	0
%define	sys_stdout	1
%define	sys_stderr	2

; File read write flags
%define	O_RDONLY		0x0000		; open for reading only
%define	O_WRONLY		0x0001		; open for writing only
%define	O_RDWR		0x0002		; open for reading and writing
%define	O_NONBLOCK	0x0004		; do not block on open or for data to become available
%define	O_APPEND		0x0008		; append on each write
%define	O_CREAT		0x0200		; create file if it does not exist
%define	O_TRUNC		0x0400		; truncate size to 0
%define	O_EXCL		0x0800		; error if O_CREAT and the file exists
%define	O_SHLOCK		0x0010		; atomically obtain a shared lock
%define	O_EXLOCK		0x0020		; atomically obtain an exclusive lock
%define	O_NOFOLLOW  0x0100      ; don't follow symlinks
%define	O_SYMLINK	0x200000		; allow open of symlinks
%define	O_EVTONLY	0x8000		; descriptor requested for event notifications only
; O_CLOEXEC ? Not found in <fcntl.h> ; mark as close-on-exec

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

; segment .data
; defaultRect istruc rect
; 	at origin

extern _sprintf

segment .bss
;align   16
	renderRect	resb	rect_size
	pOutFileStr	resq	1
	fdFileOut	resq	1
	outSize		resb	size_size
	writeBuffer	resb	2400			; 255 * 3 (RGB) = 24-bits -> *100 pixels
	;_2				resq	1

segment .data
;align 16
	;headerFormat	db		'P6', 10, '%i %i', 10, '255', 10, 0
	unboundCutoff	dq		2.0
	_2					dq		2.0
	colourUpscale	dd		14.09
	; bug in nasm, resulting in the data to be referered to incorrectly, i.e. wrong rip-offset
	; good news: it was fixed...

segment .text

; int mandelbrot(Rect* rect, double scale, const char* outFile);
; Renderes the manderbort set i an given rect
; @param rect: 	The rect in with to render the set
; @param outFile:	A file name to which the set will be written as a Portable pixmap (.ppm)
global _mandelbrotf

; rdi points the the file name
_mandelbrotf:
	; Store the rect in renderRect
	lea	rax, [rel renderRect]		; load effctive address of the local variable renderRect
	mov	ecx, 4							; itterrate 3 times
	.LRect:
		mov	r8, [rdi + rcx * 8 - 8]	; eath 8 byte = 64-bit (double-FP) contains the values of the rect
		mov	[rax + rcx * 8 - 8], r8	; load this into the value of the address contained in rax
		loop	.LRect						; loop according to rcx (ecx)
	
	; Inverte the y-axis, i.e. flip the sign
	; to start at the top left corner
	movsd	xmm0, [rax + rect.origin + point.y]
	xorpd	xmm1, xmm1
	subsd	xmm1, xmm0
	movsd	[rax + rect.origin + point.y], xmm1
	
	
	; Store output size of image into local variable (same as before)
	lea	rax, [rel outSize]
	mov	ecx, 2
	.LoutSize:
		mov	r8, [rsi + rcx * 8 - 8]
		mov	[rax + rcx * 8 - 8], r8
		loop	.LoutSize
	
	lea	rax, [rel pOutFileStr]
	mov	[rax], rdx		; store the output file name pointer into pOutFile
	
	call	.openFile
	call	.writeHeader
	call	_mandelbrotb
	call	.closeFile
	
	;cvtsd2si	rax, [rel outSize]
	mov	rax, [rel pOutFileStr]
	ret
	
	.openFile:
		; Overwrite the file if it already exist
		mov	rax, sys_unlink
		mov	rdi, [rel pOutFileStr]
		syscall

		; Open file: int open(user_addr_t path, int flags, int mode);
		mov	rax, sys_open
		mov	rdi, [rel pOutFileStr]					; pointer to file name
		mov	rsi, O_WRONLY | O_APPEND | O_CREAT	; write only, append on write, and create if not exsistant
		mov	edx, 110100100b							; mode according to chmod (but in binary ... apparently)
		syscall
		mov	[rel fdFileOut], rax			; store the file discriber locally
		ret
		
	.closeFile:
		mov	rax, sys_close
		lea	rdi, [rel fdFileOut]
		syscall
		ret
		
	.writeHeader:
		; load the effective address af the write buffer
		lea	rax, [rel writeBuffer]
		
		; use sprintf to append the size of the image
		; int
	   ; sprintf(char * restrict str, const char * restrict format, ...);
	   ; sprintf(buf, "%i %i\n", size.width, size.height);
		mov		rdi, rax
		; lea		rsi, [rel headerFormat]
		;lea		rsi, [rel headerFormat]
		cvtsd2si	rdx, [rel outSize + size.width]		; convent the two sizes to integers
		cvtsd2si	rcx, [rel outSize + size.height]
		call		_sprintf
		
		mov	rdx, rax		; sprintf return the number of written bytes
		; user_ssize_t write(int fd, user_addr_t cbuf, user_size_t nbyte);
		mov	rax, sys_write
		mov	rdi, [rel fdFileOut]
		lea	rsi, [rel writeBuffer]
		;  mov	rdx, bytes to write		; look above
		syscall
		
		ret

; int mandelbrotb(Rect* rect, Size* outSize, const int8_t* outBuf)
;						rdi			rsi				rdx
%define	rect		rdi
%define	outSize	rsi
%define	outBuf	rdx
global _mandelbrotb
_mandelbrotb:
; R0  R1  R2  R3  R4  R5  R6  R7  R8  R9  R10  R11  R12  R13  R14  R15
; RAX RCX RDX RBX RSP RBP RSI RDI
; xmm0 .. 15
; xmm0 .. 7 (128 bit), .. 15 (64 bit) <– that's perhaps not the case
; (the calle-save registers) are: rbp, rbx, r12, r13, r14, r15
; Z_n+1 = Z_n^2 + c
; Itterrations over this imaginary plane contaning the coordinates c
; will, as long as these values stay bounded, be the mandelbrot set

; moves a double-FP values from memory into
; both row and high of an xmm-register
; %macro movhl	2
; 	movsd		%1, %2
; 	movhpd	%1, %2
; %endmacro
; OBS: This is done by movddup

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
%define	rUbS1	r11b	; Unbound stamp 1
%define	rUbS2	r12b	; Unbound stamp 2
%define	xVal2	xmm8	; the value 2 (as two Double-FP), used by the mdl macro
movddup	xVal2, [rel _2]
%define	xUps	xmm9
movss		xUps, [rel colourUpscale]

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

; Number of max itterrations over the manderbort quality
%define	mdlItter	1000
; Written to the pixels and meant to role over when it reathes 255
%define	rorIdx	bx

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
; Advance the second (low) value to contain the next pixel
addsd		xmmds, xmmdx
subsd		xmmei, xmmdy

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
		mov	rUbS1, -1		;
		mov	rUbS2, -1		;
		mov	rorIdx, 0
		; Assume one (simple) itterration
		; Z = 0^2 + c = c => d + ei = a + bi
		movapd	xmma, xmmd
		movapd	xmmbi, xmmei
		; or perhaps not...?
		; xorpd	xmma, xmma
		; xorpd	xmmbi, xmmbi
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
			cmp	rUbS1, -1		; skip the next instictions if the unbound time stamp has allready been set, i.e. above 0
			jg		LUb1Skip			;
			mov	rax, rdx			; copy the signmask-extract
			and	rax, 0b01		; and it with this pixel
			cmp	rax, 0			; compare if it excist
			cmova	rUbS1, rorIdx	; store the time stamp of "becomming" unbound on the condition of rax being the number above
			LUb1Skip:
			
			cmp	rUbS2, -1		; the same as above, but with the next pixel
			jg		LUb2Skip
			and	rdx, 0b10
			cmp	rdx, 0
			cmova	rUbS2, rorIdx
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
			inc	rorIdx
			dec	ecx
			jnz	Lmdl
			LmdlSkip:
			
			%macro	sqrtInt	1			; newValue = sqrt(value) * (255/sqrt(255))		to boost the contrast a bit
				cvtsi2ss	xmmt1, %1
				sqrtss	xmmt1, xmmt1
				mulss		xmmt1, xUps
				cvtss2si	%1, xmmt1
			%endmacro
			
			; Write the unbound time stamps to memory
			sqrtInt	rUbS1
			mov	eax, 255
			sub	eax, rUbS1
			mov	[outBuf], eax
			inc	outBuf
			
			sqrtInt	rUbS2
			mov	eax, 255
			sub	eax, rUbS2
			mov	[outBuf], eax
			inc 	outBuf
			
			
		addpd	xmmd, xmmdx		; increment the x coordinate
		dec	rXit
		cmp	rXit, 0
		jnz	Lx					; jump if not zero
		
	
	subpd	xmmei, xmmdy	; increment the y coordinate, i.e. decrement as we're starting in top left
	dec	rYit
	cmp	rYit, 0
	jnz	Ly					; jump if not zero

mov rax, 0	; sucess
ret
		
__exit:
	mov	rax, sys_exit
	mov	rdi, 0
	syscall
