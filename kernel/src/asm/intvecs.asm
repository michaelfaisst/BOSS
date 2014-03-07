; *********************************************************************
; * Interrupt Vector Table for BOSS
; *
; *********************************************************************

	.global _reset_handler_asm
	.global undefInstrHandler
	.global _swi_handler_asm
	.global prefetchAbortHandler
	.global dataAbortHandler
	.global irqHandler
	.global fiqHandler

	.sect ".intvecs"
	.word 0					; reset interrupt
	B undefInstrHandler 	; undefined instruction interrupt
	B _swi_handler_asm 		; software interrupt
	B prefetchAbortHandler	; abort (prefetch) interrupt
	B dataAbortHandler		; abort (data) interrupt
	.word 0 				; reserved
	B irqHandler			; IRQ interrupt NORMAL INTERRUPT: will save context registers (r8-r16)
	B fiqHandler			; FIQ interrupt	FAST INTERRUPT REQUEST: won't save context registers (r8-r16)
