bits 32

global _concatenateWords

segment code public code use32

_concatenateWords:
    push ebp
    mov ebp, esp

    pushad

    mov byte [ebp + 8], 0  

    mov ecx, [ebp + 12]
    xor esi, esi     

loop_sentences:
    mov edx, [ebp + 12]

    mov ebx, edx        
    xor ebp, ebp  

loop_tokenize:

    mov edx, ' '
    call strtok

    test eax, eax
    jz end_sentence

    cmp ebp, esi
    jne not_matching

    mov edx, eax
    call strcat

    call strcat

    jmp end_sentence

not_matching:
    inc ebp
    jmp loop_tokenize

end_sentence:
    inc esi

    loop loop_sentences

    popad

    mov esp, ebp
    pop ebp

    ret

strtok:
    mov al, [ebx]
find_non_delim:
    cmp al, 0
    je  end_of_string

    cmp al, dl
    je  found_delim

    inc ebx
    mov al, [ebx]
    jmp find_non_delim

found_delim:
    mov byte [ebx], 0

    inc ebx

    mov eax, ebx
    ret

end_of_string:
    xor eax, eax
    ret

strcat:
    mov esi, edi
    xor ecx, ecx
find_end:
    cmp byte [esi], 0
    je  end_find_end
    
    inc esi
    inc ecx
    jmp find_end

end_find_end:
    mov edi, esi
    mov esi, edx
    rep movsb
    
    ret