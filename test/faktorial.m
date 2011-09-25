readint n

push n
call faktorial
call end

faktorial: top fa
           pop
	   if fa fakt2
	   push 1
	   return
fakt2:     fb = fa-1
	   push fa
	   push fb
	   push fb
	   call faktorial
	   top fc
	   pop
	   top fb
	   pop
	   top fa
	   pop
	   fa=fc*fa
	   push fa
	   return
end: top a
     writeint a
     writechar 10
