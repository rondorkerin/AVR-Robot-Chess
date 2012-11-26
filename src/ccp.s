#define   _SFR_ASM_COMPAT   1 
#define   _SFR_OFFSET  0 

#include  <avr/io.h> 


#define   CCP_SIGN   0xD8 


.global  ccp_write 

.func  ccp_write 

   ccp_write: 

    movw   r26, r24  // Copy register address into X ptr. 
   ldi  r24, CCP_SIGN 
   out  CCP, r24 
    st   X, r22  // Write value to protected register X_ptr. 
ret 
.endfunc 

// Must use registers shown, BUT you can use Y ( must preserve these registers ) or Z ptr. instead of X .