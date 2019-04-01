( n -- sqrt )
: sqrt 
	>r
	1 1
	repeat
		dup * dup
		r@ =
		if drop 1 else 
			r@ <  
			if 1 + dup 0 else 1 - 1 then
		then
	until 
	r> drop
	cr
;

( n -- 0/1 ) 
: is_prime
	dup 1 <
	if . ."  is not natural\n" exit then
	dup 1 =
	if drop 0 exit then
	dup 2 = 
	if drop 1 exit then

	dup sqrt 1 + 2 
	do
		r@ . ."  count it\n" 
		dup r@ % 
		not
		if 
			r> drop ( destroy old index )
			sqrt 1 + >r ( push last index )
			drop 0 
		then
	loop
	0 = if 0 else drop 1 then
;
