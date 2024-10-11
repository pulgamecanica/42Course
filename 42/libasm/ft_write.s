section .text
  extern __errno_location; Export the error
  global ft_write     ; Export the function (non static)

; Original: (man 2 write) ssize_t write(int fd, const void buf[.count], size_t count);
;
; write:
; Descr: Returns the number of bytes written
; Arguments:
;   rdi - the fd
;   rsi - the string to be written
;   rdx - the number of bytes to write
; Return:
;   rax - the number of bytes written (ssize_t)
ft_write:
  mov rax, 1
  syscall             ; rax is 1 which corresponds to write syscall
  test rax, rax       ; check rax (ret val of system) for any errors
  js  .error          ; jump to error if any errors founded
  ret

.error:
  neg rax             ; negate rax to get a possive error code (errno)
  mov rdi, rax        ; save rax to rdi
  call __errno_location wrt ..plt ; call error to get the address of errno
  mov [rax], rdi
  mov rax, -1         ; system write would return an error number (negative), not -1
  ret