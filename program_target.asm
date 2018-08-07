; CODE IS GENRATED BY MINI COMPILER.
; Developed By: Innam Hunzai.
; 19 May, 2013.
; COMSATS University of Science and technolgy Islamabad.
;------------------------------------------------------------------

.MODEL SMALL
extrn INDEC:far,OUTDEC:far,INBIN:far,OUTBIN:far,INOCT:far,OUTOCT:far,INHEX:far,OUTHEX:far,NEWLINE:far,MESSAGE:far
.STACK 250H
.DATA
   flg  dw  ?
   in  dw  ?
   c  dw  ?
   r  dw  ?


.CODE

START:
	MOV AX, @DATA
	MOV DS, AX

	MOV AX, 0
	MOV r, AX
	MOV AX, 1
	MOV flg, AX
  LB01:
	MOV AX,flg
	CMP AX, 1
	JNE LB01
	MOV AX, c
	CMP AX, 0
	JNE LB01
	MOV AX, 0
	MOV flg, AX
  LB01:
	MOV AX, c
	SUB AX, 1
	MOV c, AX
	JMP LB01
  LB01:
	MOV AX, 4C00H
	INT 21H

END START