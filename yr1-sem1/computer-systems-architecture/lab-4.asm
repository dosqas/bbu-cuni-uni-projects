bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    ; ...
    a dw 10
    b dw 15
    c dd 0

; our code starts here
segment code use32 class=code
    start:
        ; ...
        mov eax, [a]
        mov ebx, [b]
        
        ; Manipulate bits according to the specified rules
        ; bits 0-2 of C have the value 0
        ; bits 3-5 of C have the value 1
        ; bits 6-9 of C are the same as bits 11-14 of A
        ; bits 10-15 of C are the same as bits 1-6 of B
        ; bits 16-31 of C have the value 1
        
        bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    ; ...

; our code starts here
segment code use32 class=code
    start:
        ; ...
    
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack

        or dword [c], 0b00000000000000000000000000000111
        shr eax, 11
        and eax, 0b00000000000000000000000000001111
        shl eax, 6   
        or dword [c], eax
        
        shr ebx, 1         
        and ebx, 0b00000000000000000000000000111111 
        shl ebx, 10        
        or dword [c], ebx 
        
        or dword [c], 0b11111111111111110000000000000000
        
        
    
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
