bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit       
extern printf        ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll 
import printf msvcrt.dll   ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    ; ...
    a db 3
    b db 4
    c db 5
    d db 6
    e dw 8
    f dw 6
    g dw 9
    h dw 10
    rez dd 0
    mesaj db "Rezultatul la f*(e-2)/[3*(d-5)] este: %d", 0

; our code starts here
segment code use32 class=code
    start:
        ; ...
        mov al, byte [d] ; al = d
        sub al, 5 ; al = d-5
        mov bl, 3 ; bl = 3
        mul bl ; ax = 3*(d-5)
        mov cx, ax ; cx = 3*(d-5)
        
        mov ax, word [e] ; ax = e
        sub ax, 2 ; ax = e-2
        mov bx, word [f] ; bx = f
        mul bx ; dx:ax = f*(e-2)
        
        div cx; ax = f*(e-2)/[3*(d-5)]
        mov [rez], ax ; rez = f*(e-2)/[3*(d-5)]
        push dword [rez]
        push mesaj
        call [printf]
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
