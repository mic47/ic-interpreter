new a 1
@a = 0+1 
push a
new a 1
@a = 0+ 2
push a
new a 1
@a = 0+ 3
push a
new a 1
@a = 0+ 4
push a
new a 1
@a = 0+ 5
push a
new a 1
@a = 0+ 6
push a
new a 1
@a = 0+ 7
push a
new a 1
@a = 0+ 8
push a
new a 1
@a = 0+ 9
push a
new a 1
@a = 0+ 10
push a
new a 1
@a = 0+ 11
push a
new a 1
@a = 0+ 12
push a
new a 1
@a = 0+ 13
push a
new a 1
@a = 0+ 14
push a
new a 1
@a = 0+ 15
push a
new a 1
@a = 0+ 16
push a
new a 1
@a = 0+ 17
push a
new a 1
@a = 0+ 18
push a
new a 1
@a = 0+ 19
push a
new a 1
@a = 0+ 20
push a
new a 1
@a = 0+ 21
push a
new a 1
@a = 0+ 22
push a
new a 1
@a = 0+ 23
push a
new a 1
@a = 0+ 24
push a
new a 1
@a = 0+ 25
push a

call WriteStack

@^0 = @^1 + @^2

writeint @^0
writechar 10

@^20 = @^24 + @^17

writeint @^20
writechar 10

new @^4 @^21

writeint @^4
writechar 10

call WriteStack

jump end
WriteStack:


#toto je komentar medzi definicou


	writeint @^0
	writechar 32
	writeint @^1
	writechar 32
	writeint @^2
	writechar 32
	writeint @^3
	writechar 32
	writeint @^4
	writechar 32
	writeint @^5
	writechar 32
	writeint @^6
	writechar 32
	writeint @^7
	writechar 32
	writeint @^8
	writechar 32
	writeint @^9
	writechar 32
	writeint @^10
	writechar 32
	writeint @^11
	writechar 32
	writeint @^12
	writechar 32
	writeint @^13
	writechar 32
	writeint @^14
	writechar 32
	writeint @^15
	writechar 32
	writeint @^16
	writechar 32
	writeint @^17
	writechar 32
	writeint @^18
	writechar 32
	writeint @^19
	writechar 32
	writeint @^20
	writechar 32
	writeint @^21
	writechar 32
	writeint @^22
	writechar 32
	writeint @^23
	writechar 32
	writeint @^24
	writechar 10
	return

end:
push 234
writeint @^25
writechar 10
writeint ^0
writechar 10
writeint @^0
writechar 10

