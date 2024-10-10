section .text
  global ft_read      ; Export the function (non static)

; Original: (man 2 read) ssize_t read(int fd, void buf[.count], size_t count);
;
; read:
; Descr: Read from a file descriptor
; Arguments:
;   rdi - the fd to read from
;   rsi - the string buffer
;   rdx - the number of bytes to read
; Return:
;   rax - the number of bytes read (ssize_t)
ft_read:
  mov rax, 0
  syscall             ; rax is 1 which corresponds to write syscall
  test rax, rax       ; check rax (ret val of system) for any errors
  js  .error          ; jump to error if any errors founded
  ret

.error:
  call extern __errno_location
  mov rax, -1         ; system write would return an error number (negative), not -1
  ret