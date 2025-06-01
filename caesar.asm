section .data
    ; User messages
    current_msg db "Current message: ", 0
    current_msg_len equ $ - current_msg

    edited_msg db "Edited message: ", 0
    edited_msg_len equ $ - edited_msg

section .bss
    ; Buffers for input/output
    input_buffer resb 100          ; Input string
    shift_value resd 1             ; Shift value
    output_buffer resb 100         ; Encrypted message

section .text
    global encrypt_caesar          ; Function for Caesar cipher encryption

encrypt_caesar:
    push rbx                       ; Save rbx register
    mov rbx, rdi                   ; Input string pointer
    mov rdi, rsi                   ; Output buffer pointer
    mov ecx, edx                   ; Shift value

caesar_loop:
    mov al, byte [rbx]             ; Load current character
    cmp al, 0                      ; Check for null terminator
    je caesar_done                 ; End loop if string ends

    ; Check uppercase range
    cmp al, 'A'
    jl copy_non_alpha              ; Non-alphabetic character
    cmp al, 'Z'
    jle uppercase_encrypt          ; Encrypt uppercase letters

    ; Check lowercase range
    cmp al, 'a'
    jl copy_non_alpha
    cmp al, 'z'
    jle lowercase_encrypt          ; Encrypt lowercase letters

copy_non_alpha:
    mov byte [rdi], al             ; Copy non-alphabetic as is
    jmp next_char

uppercase_encrypt:
    sub al, 'A'                    ; Normalize to 0-25 range
    add al, cl                     ; Apply shift
    cmp al, 0
    jge wrap_upper
    add al, 26                     ; Handle negative shift wrap

wrap_upper:
    cmp al, 26
    jl store_uppercase
    sub al, 26                     ; Handle overflow wrap

store_uppercase:
    add al, 'A'                    ; Convert back to ASCII
    jmp store_char

lowercase_encrypt:
    sub al, 'a'                    ; Normalize to 0-25 range
    add al, cl                     ; Apply shift
    cmp al, 0
    jge wrap_lower
    add al, 26                     ; Handle negative shift wrap

wrap_lower:
    cmp al, 26
    jl store_lowercase
    sub al, 26                     ; Handle overflow wrap

store_lowercase:
    add al, 'a'                    ; Convert back to ASCII

store_char:
    mov byte [rdi], al             ; Store encrypted character

next_char:
    inc rbx                        ; Advance input pointer
    inc rdi                        ; Advance output pointer
    jmp caesar_loop

caesar_done:
    mov byte [rdi], 0              ; Null-terminate output string
    pop rbx                        ; Restore rbx register
    ret                            ; Return to caller
