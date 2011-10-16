push 1
push 2
push 3
push 4
push 5
push 6
push 7
push 8
push 9
push 10
push 11
push 12
push 13
push 14
push 15
push 16
push 17
push 18
push 19
push 20
push 21
push 22
push 23
push 24
push 25

call WriteStack

^0 = ^1 + ^2

writeint ^0
writechar 10

^20 = ^24 + ^17

writeint ^20
writechar 10

new ^4 ^21

writeint ^4
writechar 10

call WriteStack

jump end
WriteStack:
	writeint ^0
	writechar 32
	writeint ^1
	writechar 32
	writeint ^2
	writechar 32
	writeint ^3
	writechar 32
	writeint ^4
	writechar 32
	writeint ^5
	writechar 32
	writeint ^6
	writechar 32
	writeint ^7
	writechar 32
	writeint ^8
	writechar 32
	writeint ^9
	writechar 32
	writeint ^10
	writechar 32
	writeint ^11
	writechar 32
	writeint ^12
	writechar 32
	writeint ^13
	writechar 32
	writeint ^14
	writechar 32
	writeint ^15
	writechar 32
	writeint ^16
	writechar 32
	writeint ^17
	writechar 32
	writeint ^18
	writechar 32
	writeint ^19
	writechar 32
	writeint ^20
	writechar 32
	writeint ^21
	writechar 32
	writeint ^22
	writechar 32
	writeint ^23
	writechar 32
	writeint ^24
	writechar 10
	return

end:
writeint ^25
