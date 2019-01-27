
GetManorMultipler PROTO
.data
exitJmp QWORD 7D12E6h
.code
ManorMultiple proc
;.text:00000000007D12E1 mov     eax, [rbx+1Ch]
;.text:00000000007D12E4 inc     eax
	call GetManorMultipler
	add eax, [rbx+1Ch]
	jmp exitJmp
ManorMultiple endp
end