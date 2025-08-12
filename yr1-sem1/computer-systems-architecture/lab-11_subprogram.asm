section .text
global loop_func
loop_func:
    xor ebx, ebx   
.loop:
    lodsb          
    cmp al, 'b'
    je .end_loop
    sub al, '0'    
    jmp .process
    
.cont:
    rcl ebx, 1     
    jmp .loop
    
.process:
    cmp al, 0
    jz .setzero
    stc
    jmp .cont
    
.setzero:
    clc
    jmp .cont
    
.end_loop:
    ret            