readint n

an=n+1
bn=n+0
new pole an
zaciatokcyklu: if bn pokracujvcykle
               jump jumpend
               pokracujvcykle: push bn
                               call faktorial
	                       pole2=pole+bn
	                       top @pole2
			       pop
			       bn=bn-1
			       jump zaciatokcyklu
jumpend: jump end 

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

end: bn=n+0
     zaccyklus2: if bn pokr
                 jump totalend
		 pokr: pole2=pole+bn
		       writeint bn
		       writechar 32
		       writeint @pole2
		       writechar 10
		       bn=bn-1
		       jump zaccyklus2

totalend: delete pole
     

	
