section .text
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
  ret