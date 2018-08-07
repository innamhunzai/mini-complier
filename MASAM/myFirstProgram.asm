.MODEL SMALL
.STACK 250H
.DATA
   N1 dw ?
   N2 dw ?

.CODE

 START: 
        MOV AX, @DATA
        MOV DS, AX
        
        MOV AX,20
        MOV N1,AX
        MOV AX,50
        MOV N2, AX

        ADD AX, N1
 
       MOV AX, 4C00H
       INT 21H
    
 END START         
