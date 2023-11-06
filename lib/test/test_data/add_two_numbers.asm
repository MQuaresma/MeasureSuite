SECTION .text
%ifidn __OUTPUT_FORMAT__, macho64
GLOBAL _add_two_numbers
_add_two_numbers:
%elifidn __OUTPUT_FORMAT__, elf64
GLOBAL add_two_numbers
add_two_numbers:
%endif
; [rdi]= [rsi] + [rdx]
mov rax, [rsi]
add rax, [rdx]
mov [rdi], rax
ret
