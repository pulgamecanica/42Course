section .text
  global ft_strlen    ; Export the function (non static)

; Original: (man strlen) size_t strlen(const char *s);
;
; ft_strlen:
; Descr: Returns the length of a null-terminated chain of characters (string)
; Arguments:
;   rdi - pointer to the string (char*)
;     if a pointer to something else, different from a string is given
;     the behaviour is undefined
; Return:
;   rax - the length of the string (size_t)
ft_strlen:
  push rdi            ; put the first argument (pointing to the string) onto the stack
                      ; to follow the callee-saved convention (push instead of mov)
  xor rax, rax        ; Clear rax (index), to start at 0 (zero)
.loop:
  cmp byte [rdi], 0   ; Compare current byte with '\0' 
  je  .done           ; 'jump it equal' to the label done
  inc rax             ; increase the index
  inc rdi             ; increase the pointer (string argument)
  jmp .loop           ; 'jump' to the loop label (repeat)
.done:
  pop rdi             ; restore rdi register (callee-saved convention)
  ret                 ; return the string length (rax is value returned)