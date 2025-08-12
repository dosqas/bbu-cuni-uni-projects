bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit   
extern printf            ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll 
import printf msvcrt.dll   ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    ; ...
    a dw 143
    b dw 6
    c dw 37
    d dw 12
    rez dd 0
    mesaj db "Rezultatul calculului (a-b+c)-(d+d) este: %d",0

; our code starts here
segment code use32 class=code
    start:
        ; ...
        mov ax, word [a] ; ax = a
        sub ax, word [b] ; ax = a-b
        add ax, word [c] ; ax = a-b+c
        mov bx, word [d] ; bx = d
        add bx, word [d] ; bx = d+d
        sub ax, bx ; ax = (a-b+c)-(d+d)
        mov [rez], ax ; rez = (a-b+c)-(d+d)
        push dword [rez]
        push mesaj
        call[printf]
        
    
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
