colon "bye", bye
    dq xt_lit, 60
    TIMES 6 dq xt_lit, 0
    dq xt_syscall

%define sys_read_no 0
%define sys_write_no 1

colon "sys-read", sys_read
TIMES 3 dq xt_push_to_r
dq xt_lit, sys_read_no
TIMES 3 dq xt_pop_from_r
TIMES 3 dq xt_lit, 0
dq xt_syscall , xt_drop, xt_exit

colon "sys-write", sys_write
TIMES 3 dq xt_push_to_r
dq xt_lit, sys_write_no
TIMES 3 dq xt_pop_from_r
TIMES 3 dq xt_lit, 0
dq xt_syscall , xt_drop, xt_exit


colon  "stack-cell", stack_cell
  dq xt_lit, 0, xt_forth_sp, xt_exit

; ( fd - char 1) or ( - 0 )
colon "file-read-char", file_read_char
  dq xt_push_to_r, xt_stack_cell, xt_pop_from_r, xt_swap, xt_lit, 1, xt_sys_read
	branch0 .fail
  dq xt_lit, 1
.fail :
	dq xt_exit

  ; ( fd char - )
colon "file-write-char", file_write_char
  dq xt_swap, xt_push_to_r, xt_push_to_r
  dq xt_stack_cell, xt_dup, xt_pop_from_r, xt_swap, xt_store_char
  dq xt_pop_from_r, xt_swap, xt_lit, 1, xt_sys_write, xt_drop, xt_drop, xt_exit

 ;( char - )
colon "print-char", print_char
	dq xt_lit, 1, xt_swap, xt_file_write_char, xt_exit

colon "digit-to-char", digit_to_char
	dq xt_lit, 0x30, xt_plus, xt_exit

colon "print-uint", print_uint
	dq xt_push_to_r
	dq xt_forth_sp
	dq xt_pop_from_r, xt_swap
	dq xt_push_to_r, xt_push_to_r
.loop:
	dq xt_fetch_r
   	dq xt_lit, 10, xt_div_rem
	dq xt_pop_from_r
	dq xt_lit, 10, xt_div
	dq xt_dup
	branch0 .print
	dq xt_push_to_r
	branch .loop
.print:
	dq xt_drop
.print_loop:
	dq xt_forth_sp, xt_fetch_r, xt_less
	branch0 .exit
	dq xt_digit_to_char, xt_print_char
	branch .print_loop
.exit:
	dq xt_pop_from_r, xt_drop, xt_exit

colon "string-length", string_length
      dq xt_dup
.loop:
      dq xt_dup, xt_fetch_char
      branch0 .exit
      dq xt_inc
      branch .loop
.exit:
  dq xt_swap, xt_sub, xt_exit

colon "print-cr", print_cr
  dq xt_lit, 1, xt_lit, 10, xt_file_write_char, xt_exit

;  ( str count - )
colon "print-string-counted", print_string_counted
  dq xt_push_to_r, xt_push_to_r, xt_lit, 1, xt_pop_from_r, xt_pop_from_r, xt_sys_write, xt_drop, xt_exit

; ( str -- )
colon "print-string", print_string
  dq xt_dup, xt_string_length, xt_print_string_counted, xt_exit

colon "char-is-space", char_is_space
  dq xt_push_to_r
  dq xt_fetch_r, xt_lit,  ' ', xt_equal
  dq xt_fetch_r, xt_lit, 10, xt_equal, xt_lor
  dq xt_fetch_r, xt_lit, 13, xt_equal, xt_lor
  dq xt_fetch_r, xt_lit, 9, xt_equal, xt_lor
  dq xt_pop_from_r,  xt_lit, '\t', xt_equal, xt_lor, xt_exit

  ; ( addr - )
colon "string-terminate", string_terminate
  dq xt_lit, 0, xt_swap, xt_store_char, xt_exit

;  ( fd buffer -  )
colon "file-read-word", file_read_word
  dq xt_dup, xt_push_to_r, xt_push_to_r

.read_space:
  dq xt_dup, xt_file_read_char
  branch0 .eof

  dq xt_dup, xt_char_is_space
  branch0 .commit

  dq xt_drop
  branch .read_space

.read:
  dq xt_dup, xt_file_read_char
  branch0 .eof

  dq xt_dup, xt_char_is_space, xt_bit_not
  branch0 .end

.commit:
  dq xt_fetch_r, xt_store_char
  dq xt_pop_from_r, xt_inc, xt_push_to_r
  branch .read

.end:
  dq xt_drop
.eof:
  dq xt_drop, xt_fetch_r, xt_string_terminate
  dq xt_pop_from_r, xt_pop_from_r, xt_sub, xt_exit



colon ",", comma
  dq xt_here, xt_fetch, xt_store
  dq xt_here, xt_fetch, xt_lit, 8, xt_plus
  dq xt_here, xt_store, xt_exit

colon "c,", char_comma
  dq xt_here, xt_fetch, xt_store_char
  dq xt_here, xt_fetch, xt_inc
  dq xt_here, xt_store, xt_exit

colon "2dup", ddup
  dq xt_push_to_r, xt_dup, xt_fetch_r, xt_swap, xt_pop_from_r, xt_exit

colon "2inc", dinc
  dq xt_push_to_r, xt_inc, xt_pop_from_r, xt_inc, xt_exit

colon "string-equals", string_equals
.start:
  dq xt_ddup, xt_fetch_char, xt_swap, xt_fetch_char, xt_equal
  branch0 .no
  dq xt_dup, xt_fetch_char
  branch0 .yes
  dq xt_dinc
  branch .start
.yes:
  dq xt_drop, xt_drop, xt_lit, 1, xt_exit
.no:
  dq xt_drop, xt_drop, xt_lit, 0, xt_exit

; ( dest source - )
colon "string-copy", string_copy
.loop:
dq xt_ddup, xt_fetch_char, xt_push_to_r, xt_fetch_r, xt_swap, xt_store_char
dq xt_dinc, xt_pop_from_r, xt_bit_not
  branch0 .loop
  dq xt_drop, xt_drop, xt_exit

colon ":", colon
  .restart:
    dq xt_fd, xt_fetch, xt_lit, word_buf, xt_file_read_word
    branch0 .restart
    dq xt_lit, 0, xt_lit, word_buf, xt_forth_create
    dq xt_lit, STATE_COMPILE, xt_state, xt_store
    dq xt_lit, i_docol, xt_comma, xt_exit

colon ";", semicolon, 1
    dq xt_lit, STATE_INTERPRET, xt_state, xt_store
    dq xt_lit, xt_exit, xt_comma, xt_exit

colon "char-is-digit", char_is_digit
  dq xt_push_to_r
  dq xt_fetch_r, xt_lit, '9'+1, xt_less
  dq xt_lit, '0'-1, xt_fetch_r, xt_less, xt_land
  dq xt_pop_from_r, xt_drop, xt_exit

colon "drop0", drop0
  dq xt_dup
  branch0 .is_zero
  dq xt_exit
  .is_zero:
  dq xt_drop, xt_exit

colon "char-to-digit", char_to_digit
  dq xt_lit, '0', xt_sub, xt_exit

; ( buf -- num? len )
colon "string-parse-unsigned-number", string_parse_unsigned_number
  dq xt_dup, xt_push_to_r
  dq xt_lit, 0, xt_push_to_r
.loop:
  dq xt_dup, xt_fetch_char, xt_dup, xt_char_is_digit
  branch0 .end
  dq xt_char_to_digit
  dq xt_lit, 10, xt_pop_from_r, xt_mul, xt_plus, xt_push_to_r
  dq xt_inc
  branch .loop
  .end:
  dq xt_drop, xt_pop_from_r, xt_swap, xt_pop_from_r, xt_sub, xt_drop0, xt_exit

colon "neg", neg
  dq xt_lit, 0, xt_swap, xt_sub, xt_exit

colon "inc", inc
  dq xt_lit, 1, xt_plus, xt_exit

; ( buf -- num? len )
colon "string-parse-number", string_parse_number
  dq xt_dup, xt_fetch_char, xt_dup, xt_char_is_digit, xt_bit_not
  branch0 .unsigned
  dq xt_dup, xt_lit, '-', xt_equal, xt_bit_not
  branch0 .negative

  dq xt_dup, xt_lit, '+', xt_equal, xt_bit_not
  branch0 .positive

.negative:
  dq xt_drop
  dq xt_inc, xt_string_parse_unsigned_number
  dq xt_dup
  branch0 .error
  dq xt_inc
  dq xt_swap, xt_neg, xt_swap, xt_exit
.positive:
  dq xt_drop
  dq xt_inc, xt_string_parse_unsigned_number
  dq xt_dup
  branch0 .error
  dq xt_inc, xt_exit
.unsigned:
  dq xt_drop, xt_string_parse_unsigned_number, xt_exit 
.error:
  dq xt_drop, xt_lit, 0, xt_exit

colon "..", show_stack_s
	dq  xt_forth_sp
	dq xt_push_to_r
.loop:
	dq xt_fetch_r, xt_forth_stack_base, xt_less
	branch0 .exit
	dq xt_fetch_r, xt_fetch, xt_print_uint, xt_print_cr
	dq xt_pop_from_r
	dq xt_lit, 8, xt_plus
	dq xt_push_to_r
	branch .loop
.exit:
	dq xt_pop_from_r, xt_drop, xt_exit
