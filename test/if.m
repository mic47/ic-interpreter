start:
	readint a
	if a G
L0:	ifg a GG
L1:	ifeq a EQ
L2:	ifl a L
L3:	ifgeq a GEQ
L4:	ifleq a LEQ

	jump start

G:
	writechar 103
	writechar 114
	writechar 101
	writechar 97
	writechar 116
	writechar 101
	writechar 114
	writechar 10
	jump L0

GG:
	writechar 103
	writechar 114
	writechar 101
	writechar 97
	writechar 116
	writechar 101
	writechar 114
	writechar 10
	jump L1


EQ:
	writechar 101
	writechar 113
	writechar 117
	writechar 97
	writechar 108
	writechar 10
	jump L2
L:
	writechar 108
	writechar 111
	writechar 119
	writechar 101
	writechar 114
	writechar 10

	jump L3
GEQ:
	writechar 103
	writechar 114
	writechar 101
	writechar 97
	writechar 116
	writechar 101
	writechar 114
	writechar 79
	writechar 114
	writechar 69
	writechar 113
	writechar 117
	writechar 97
	writechar 108
	writechar 10
	jump L4
LEQ:
	writechar 108
	writechar 111
	writechar 119
	writechar 101
	writechar 114
	writechar 79
	writechar 114
	writechar 69
	writechar 113
	writechar 117
	writechar 97
	writechar 108
	writechar 10
	jump start
