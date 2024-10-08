section .text
  global ft_strcmp    ; Export the function (non static)

; Original (man strcmp) int strcmp(const char *s1, const char *s2);
;
; ft_strcmp:
; Descr: Compare two strings
; Arguments:
;   rdi - Pointer to first string
;   rsi - Pointer to second string
;     if a pointer to something else, different from a string is given
;     the behaviour is undefined
; Return:
;   rax - 0 when strings string are the same, else the difference between first missmatch
ft_strcmp:
  mov al, BYTE [rdi]  ; Load a byte from the first string (rdi)
  mov bl, BYTE [rsi]  ; Load a byte from the second string (rsi)
  test al, al         ; Check if we reached the end of the first string (null terminator)
  je .done            ; If end, jumt to .done
  test bl, bl         ; Check if we reached the end of the second string (null terminator)
  je .done            ; If end, jumt to .done
  cmp al, bl          ; Compare the bytes
  jne .done           ; If different, jump to .done
  inc rdi             ; Increment rdi to move to the next character
  inc rsi             ; Increment rsi to move to the next character
  jmp ft_strcmp       ; Repeat the loop
.done:
  movzx rax , al      ; Zero-extend al to rax (rax now has the value of al)
  movzx rcx, bl       ; Zero-extend bl to rcx (64 bits)
  sub rax, rcx        ; Now rax = al (zero-extended 64 bits) - bl (also zero-extended implicitly rcx)
  ret
