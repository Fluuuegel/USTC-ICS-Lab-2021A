.ORIG x3000
		ADD R1, R1, #1
		ADD R3, R0, #-2
		BRnz LABEL
		JSR JUDGE
LABEL		HALT
				
JUDGE   AND R3, R3, #0
		ADD R3, R3, #2	;R3 = i = 2
			
    	ADD R4, R3, #-1	;R4 = R3 - 1
RECUR	ADD R5, R5, R3
		ADD R4, R4, #-1
		BRp RECUR	;求i方 存到R5
		
		NOT R2, R0
		ADD R2, R2, #1	;R0取反加1 -> R2
		ADD R5, R5, R2
		BRp RET1	;若R5小于R0 跳出循环
				
		ADD R4, R0, #0
        NOT R6, R3	;i 取反+1
 		ADD R6, R6, #1
J2 		ADD R4, R4, R6;R4 = R4 - i判断是否为0
 		BRz RET0
 		BRp J2
 		
 		ADD R3, R3, #1 
 		ADD R4, R3, #-1
 		BRnzp RECUR
				
RET0 	AND R1, R1, #0	;R1 = 0
		RET
				
RET1 	RET
	.END