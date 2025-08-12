; A file name and a text (defined in the data segment) are given. The text contains lowercase letters, uppercase letters, digits and special characters. Replace all the special characters from the given text with the character 'X'. Create a file with the given name and write the generated text to file.
bits 32

global start        

extern exit, fopen, fprintf, fclose            
import exit msvcrt.dll
import fopen msvcrt.dll
import fprintf msvcrt.dll
import fclose msvcrt.dll   

segment data use32 class=data
    ; ...
    file_name db "samplefile.txt", 0
    access_mode db "w", 0
    file_descriptor dd -1 
    
    text db "-S4mpl3T3xt p0pc0rn!", 0
    output_text resb 256
    newline db 0xA, 0
    
    x_char db "X", 0

; our code starts here
segment code use32 class=code
    start:
        ; ...
        mov esi, text
        mov edi, output_text
        
        repeat:
        lodsb
        cmp al, 0  
        je end_of_repeat
        cmp al, 'a'
        jge check1
        jmp continue1
        
        continue1:
        cmp al, 'A'
        jge check2  
        jmp continue2
        
        continue2:
        cmp al, '0'
        jge check3
        jmp is_special_char

        is_special_char:
        mov al, [x_char]
        jmp continue_loop
              
        check1:
        cmp al, 'z'
        jle not_special_char
        jmp continue1
        
        check2:
        cmp al, 'Z'
        jle not_special_char
        jmp continue2
        
        check3:
        cmp al, '9'
        jle not_special_char
        jmp is_special_char

        not_special_char:
        jmp continue_loop
        
        continue_loop:
        stosb
        jmp repeat
        
        
        end_of_repeat:
        push dword access_mode
        push dword file_name
        call [fopen]
        add esp, 4*2
        
        mov [file_descriptor], eax
        cmp eax, 0
        je end
        
        push dword output_text
        push dword [file_descriptor]
        call [fprintf]
        add esp, 4*2
        
        push dword newline
        push dword [file_descriptor]
        call [fprintf]
        add esp, 4*2
        
        
        push dword [file_descriptor]
        call [fclose]
        add esp, 4*1    
        
        end:
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
