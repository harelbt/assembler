;comment to ignore

.string "label:aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
.string "label:"
.data +7,-0, -3
LABEL1:	mov	#7,	r1	
LABEL2:	mov	#-9	,r2
	mov	r3	,r2	
	mov	r1	,r2	
	cmp	#-9	,r2	
	cmp	#-9	,X		
	cmp	#-9,	#8
	cmp	#9,	#8	
	cmp	r1  ,	r2	
	cmp	r1     ,	#7	
	cmp	#9   	,	r2	
	add	#7	,r1	
	add	LABEL1	,r1	
	add	#-9	,r2					
X:	add	r3	,r2	
	add	r1	,r2	
	sub	#7,	r1	
	sub	#-9	,r2	
	sub	r3	,r2	
	sub	r1	,r2	
	lea	X ,r3
	lea	DATA	,X	  
	clr	DATA		
	clr	X		
	clr	r1		
	clr	LABEL2		
	not	DATA		
	not	X		
	not	r1		
	not	LABEL2				
	inc	DATA		
	inc	X		
	inc	r1		
	inc	LABEL2		
	dec	DATA		
	dec	X		
	dec	r1		
	dec	LABEL2		
	jmp	DATA		
	jmp	X		
	jmp	LABEL2		
	jmp	&LABEL2		
	jmp	&X	 		
	bne	DATA		
	bne	X
	bne	LABEL2		
	bne	&LABEL2		
	bne	&X	 
	jsr	DATA		
	jsr	X
.data 6,  7, -7
.string "khskhbkd"
.string "jdcbjcn""
.data 6,  7, -7
.data -3
	jsr	LABEL2		
	jsr	&LABEL2		
	jsr	&X	 	
	red	DATA		
	red	X		
	red	r1		
	red	LABEL2			
	prn	r2		
	prn	X				
	prn	#8				
	prn	#8
	prn	r2		
	prn	#7		
LABEL1:	prn	r2					
rts			
stop			
DATA:	.data	-6		
stri: .string "certy"
str:	.string	"abcdef"		
