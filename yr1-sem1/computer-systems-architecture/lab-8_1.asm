bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit       
extern printf
extern scanf

import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
import printf msvcrt.dll
import scanf msvcrt.dll

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    ; ...
    message db 'Enter number a: ', 0

    message2 db 'Enter number b: ', 0

    result_msg db 'Result in hexadecimal: %x', 0
    
    format_input db '%d', 0
    
    a resd 1
    b resd 1
    sum dd 0

; our code starts here
segment code use32 class=code
    start:
        ; ...
        push dword message
        call [printf]
        add esp, 4*1
        
        push dword a
        push dword format_input
        call [scanf]
        add esp, 4*2
        
        push dword message2
        call [printf]
        add esp, 4*1
        
        push dword b
        push dword format_input
        call [scanf]
        add esp, 4*2
        
        mov eax, [a]
        add eax, [b]
        mov [sum], eax
        
        push dword [sum]
        push dword result_msg
        call [printf]
        add esp, 4*3
        
    
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
