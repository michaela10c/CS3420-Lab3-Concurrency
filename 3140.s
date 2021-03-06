		AREA myData, DATA, READWRITE	
;global variable in assembly			
OrigStackPointer DCD 0x00
		
		AREA myProg, CODE, READONLY
;export assembly functions			
		EXPORT process_terminated
		EXPORT process_begin
		EXPORT process_blocked
		EXPORT PIT0_IRQHandler
		EXPORT SVC_Handler
;import C functions
		IMPORT process_select

		PRESERVE8
				
TFLG     EQU 0x4003710C ; TFLG address
CTRL     EQU 0x40037108 ; Ctrl address
SHCSR    EQU 0xE000ED20
	
SVC_Handler
	LDR  R1, [SP,#24] ; Read PC from SVC exception 
	LDRB R0, [R1,#-2] ; Get #N from SVC instruction (PC is SVC addr + 2)
	ADR  R1, SVC_Table
	LDR  PC, [R1,R0,LSL #2] ; Branch to Nth SVC routine

SVC_Table
	DCD SVC0_begin
	DCD SVC1_terminate
	DCD PIT0_IRQHandler ; Use system tick as SVC2 handler

SVC0_begin
				PUSH {R4-R11,LR}
				;******* Store Original Stack Pointer ********
				LDR R1, =OrigStackPointer
				STR SP, [R1]
				;********************************************
SVC1_terminate				
				MOVS R0, #0
				B do_process_select


process_terminated
				CPSIE i ; Enable global interrupts, just in case   
				SVC #1 ; SVC1 = process terminated
				; This SVC shouldn't ever return, as it would mean the process was scheduled again

process_begin
				CPSIE i ; Enable global interrupts (for SVC)
				SVC #0 ; Syscall into scheduler
				BX LR
				
process_blocked
				CPSIE i ; Enable global interrupts, just in case   
				SVC #2 ; SVC2 = process blocked        
				BX LR
				
PIT0_IRQHandler ; Timer Interrupt
			  CPSID i 			; Disable all interrupts 
			  PUSH {R4-R11,LR} 	; save registers
			  ;----store scheduling timer state----
			  LDR R1, =CTRL
			  LDR R0, [R1]
			  PUSH {R0}

			  ;---clear the interrupt flag----
			  LDR  R4, =TFLG
			  MOVS R1, #1	
			  STR  R1, [R4]
			  ;-------------------------------
				
			  ;move sp to r0 to prepare for process_select 
			   MOV R0, SP 
				
do_process_select
				;******* Load Original Stack Pointer ********
				; We want the process select function to run on the "main" stack
				; This helps reduce funkiness when a process stack is too small and process_select overwrites other memory
				LDR R1, =OrigStackPointer
				LDR SP, [R1]
				;********************************************
				BL process_select	;Process_select returns 0 if there are no processes left
				CMP R0, #0
				BNE resume_process	;take branch if there are more processes
				
				; Disable scheduling timer before returning to initial caller
				LDR R1, =CTRL
				MOVS R0, #0
				STR R0, [R1]
				
				POP {R4-R11,PC} ; Restore calle-save state and return
				
resume_process 
				MOV SP, R0    ;switch stacks
				;---- restore scheduling timer state
				POP {R0}
			    LDR R1, =CTRL
			    STR R0, [R1]
				
				CPSIE I ; Enable global interrupts before returning from handler
				POP {R4-R11,PC} ; Restore registers that aren't saved by interrupt, and return from interrupt
				END
