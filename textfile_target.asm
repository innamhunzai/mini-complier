; CODE IS GENRATED BY MINI COMPILER.
; Developed By: Innam Hunzai.
; 19 May, 2013.
; COMSATS University of Science and technolgy Islamabad.
;------------------------------------------------------------------

.MODEL SMALL
extrn INDEC:far,OUTDEC:far,INBIN:far,OUTBIN:far,INOCT:far,OUTOCT:far,INHEX:far,OUTHEX:far,NEWLINE:far,MESSAGE:far
.STACK 250H
.DATA
   mes1  db  0
   k  dw  ?
   i  dw  ?
   b  dw  ?
   a  dw  ?


.CODE

START:
	MOV AX, @DATA
	MOV DS, AX

	MOV AX, k
	MUL 1
	MOV k, AX
	MOV AX, i
	CMP AX, 9
	JGE LB01
	MOV AX, i
	ADD AX, 9
	MOV i, AX
	MOV AX, k
	MUL 1
	MOV k, AX
  LB01:
	MOV AX, i
	SUB AX, 45
	MOV i, AX
  LB01:
	MOV AX, i
	ADD AX, 9
	MUL k
	ADD AX, b
	MOV i, AX
	MOV AX, k
	MUL 1
	MOV k, AX
	LEA DX, "Hello World"
	CALL MESSAGE
	CALL INDEC
	MOV k, AX
	MOV AX, i
	CMP AX, 2
	JGE LB01
  LB01:
	MOV AX,k
	CMP AX, 3
	JLE LB01
	MOV AX, i
	ADD AX, 9
	MOV i, AX
	MOV AX, k
	MUL 1
	MOV k, AX
	JMP LB01
  LB01:
	MOV AX, 4C00H
	INT 21H

END START