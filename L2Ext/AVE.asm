 Public AVEHook
 
 .data
 sStealth dw 'a','v','e','_','s','t','e','a','l','t','h',0
 sMeditation dw 'a','v','e','_','m','e','d','i','t','a','t','i','o','n',0
 sFirerootStun dw 'a','v','e','_','f','i','r','e','r','o','o','t','_','s','t','u','n',0
 sDanceStun dw 'a','v','e','_','s','t','u','n','_','d','a','n','c','e',0
 sImprisioning dw 'a','v','e','_','i','m','p','r','i','s','i','o','n','i','n','g',0
 sImprisioning2 dw 'a','v','e','_','i','m','p','r','i','s','i','o','n','i','n','g','2',0
 sSoe dw 'a','v','e','_','s','o','e',0
 .code
  
 AVEHook proc
 ;.text:00000000008F0E36 mov     rax, [rbp+0F0h]
 
	 lea     rdi, sStealth		; name of new ave in skilldata in unicode (ave_stealth example)
	 mov     rsi, rdx			; skilldata name buffer
	 mov     ecx, 0Ch			; length of abnormal name including null (12 for ave_stealth)
	 repe cmpsw				; compare words
	 jnz     short lbMeditation			; jump to next ave check
	 mov     rax, [rbp+0F0h]
	 mov     dword ptr [rax+0D4h], 100000h	; move ave id
	 mov     rax, 8F7386h
	 jmp     rax				; jump to return of skilldata function
	 lbMeditation:
		 lea     rdi, sMeditation
		 mov     rsi, rdx
		 mov     ecx, 0fh
		 repe cmpsw
		 jnz     short lbStunDance
		 mov     rax, [rbp+0F0h]
		 mov     dword ptr [rax+0D4h], 20000h
		 mov     rax, 8F7386h
		 jmp     rax
	 	 
	 lbStunDance:
		 lea     rdi, sDanceStun
		 mov     rsi, rdx
		 mov     ecx, 0fh
		 repe cmpsw
		 jnz     short lbFirerootStun
		 mov     rax, [rbp+0F0h]
		 mov     dword ptr [rax+0D4h], 40000h
		 mov     rax, 8F7386h
		 jmp     rax
		 
	 lbFirerootStun:
		 lea     rdi, sFirerootStun
		 mov     rsi, rdx
		 mov     ecx, 0fh
		 repe cmpsw
		 jnz     short lbImprisioning
		 mov     rax, [rbp+0F0h]
		 mov     dword ptr [rax+0D4h], 80000h
		 mov     rax, 8F7386h
		 jmp     rax
	 lbImprisioning:
		 lea     rdi, sImprisioning
		 mov     rsi, rdx
		 mov     ecx, 11h
		 repe cmpsw
		 jnz     short lbImprisioning2
		 mov     rax, [rbp+0F0h]
		 mov     dword ptr [rax+0D4h], 200000h
		 mov     rax, 8F7386h
		 jmp     rax
	 lbImprisioning2:
		 lea     rdi, sImprisioning2
		 mov     rsi, rdx
		 mov     ecx, 12h
		 repe cmpsw
		 jnz     short lbSoe
		 mov     rax, [rbp+0F0h]
		 mov     dword ptr [rax+0D4h], 400000h
		 mov     rax, 8F7386h
		 jmp     rax
		 
	 lbSoe:
		 lea     rdi, sSoe
		 mov     rsi, rdx
		 mov     ecx, 8h
		 repe cmpsw
		 jnz     short lbNotAVE
		 mov     rax, [rbp+0F0h]
		 mov     dword ptr [rax+0D4h], 800000h
		 mov     rax, 8F7386h
		 jmp     rax
		 		 
	 lbNotAVE:
		 mov     rax, [rbp+0F0h]		; replace overwritten code
		 mov     rbx, 8F0E3Dh
		 jmp     rbx	
 
 AVEHook endp
 
 end