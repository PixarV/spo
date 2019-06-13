; Stack commands
native "drop", drop
    add rsp, 8
    jmp next

native "swap", swap
    pop rax
    xchg [rsp], rax
    push rax
    jmp next

native "dup", dup
    push qword[rsp]
    jmp next

native '.S', show_stack
  mov rcx, rsp
  .loop:
    cmp rcx, [stack_start]
    jae next
    mov rdi, [rcx]
    push rcx
    call print_int
    call print_newline
    pop rcx
    add rcx, 8
    jmp .loop

native '.', dot
  pop rdi
  call print_int
  jmp next