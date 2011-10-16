readint n
N = n + 0
new pole N
start:
  tmp = pole + N
  tmp = tmp - 1
  new @tmp N
  N = N - 1
  ifneq N start

call VypisPointre

tmp = pole + 0
N = n + 0
start2:
  delete @tmp
  new @tmp N
  tmp = tmp + 1
  N = N - 1
  ifneq N start2

call VypisPointre



jump end
VypisPointre:
	push i
	push tmp
	i = n + 0
	cyk:
	  tmp = pole + i
	  tmp = tmp - 1
	  writeint @tmp
	  writechar 32
	  i = i - 1
	  ifneq i cyk
	writechar 10
	top tmp
	pop
	top i
	pop
	return


end:
	writechar 10
    

	
