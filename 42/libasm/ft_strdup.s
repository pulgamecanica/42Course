section .text
  global ft_strdup    ; Export the function (non-static)
  extern ft_strlen    ; Enable use of ft_strlen
  extern ft_strcpy    ; Enable use of ft_strcpy
  extern malloc       ; Enable use of malloc

ft_strdup:
  push rdi            ; Save the original rdi (the string to duplicate)
  call ft_strlen      ; Call ft_strlen(rdi) to get the length of the string
  inc rax             ; Add 1 to the length for the null terminator
  mov rdi, rax        ; Allocate memory with malloc (rax contains the size)
  call malloc
  pop rsi             ; Restore the original rdi (source string)
  test rax, rax       ; Check if malloc failed (rax = NULL)
  je .malloc_failed
  ; rdi (destination) is now the result of malloc (in rax)
  ; rsi (source) is the original string (restored from push/pop)
  ; Call ft_strcpy(dest = rax, src = rsi)
  mov rdi, rax
  call ft_strcpy      ; Fortunately strcpy is already setting rax to pointer rdi :D
  ret

.malloc_failed:
  ; If malloc failed, return NULL (rax is already 0)
  ret
