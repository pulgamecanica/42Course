section .text
  global ft_strcpy        ; Export the function (non static)

; Original: (man strcpy) char *strcpy(char *restrict dst, const char *restrict src);
;
; ft_strcpy
; Descr: Copy a string
; Arguments:
;   rdi - pointer to the destiny string
;   rsi - pointer to the source string
;     if a pointer to something else, different from a string is given
;     the behaviour is undefined 
; Return:
;   rax - the pointer to the destiny string (copied)

ft_strcpy:
  push rdi            ; Save the original destination pointer
.loop:
  mov al, [rsi]       ; Load the byte from the source string (using al to imply only 8 bits, thus 1 byte, one char)
  mov [rdi], al       ; Store it in the destination string
  inc rsi             ; Increment source pointer
  inc rdi             ; Increment destination pointer
  test al, al         ; Check if we reached the null terminator
  jne .loop           ; If not, continue copying
.done:
  pop rax             ; Restore the original destination pointer
  ret