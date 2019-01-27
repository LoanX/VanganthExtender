Public ItemDistribution
.data
CParty_GetRoutingType QWORD 00716800h
ItemDistributionExit QWORD 008BF788h
.code

;.text:00000000008BF780 mov     rcx, rax
;.text:00000000008BF783 call    CParty_GetRoutingType
;.text:00000000008BF788 test    eax, eax
;.text:00000000008BF78A jz      loc_8BF827
ItemDistribution proc
	; r14- pItem
	mov rax, [r14+30h] ; ItemInfo
	mov eax, [rax+70h] ;ItemID
	cmp eax, 8190 ; 8190 - Zariche
	je CursedWeapon
	cmp eax, 8689 ; Akamanah
	je CursedWeapon
	cmp eax, 2198h ; 8600 - lowest herb id
	jnb SecondCheck
	jmp NotHerb
	
	SecondCheck:
		cmp eax, 21A6h ; 8614 - highest herb id
		ja NotHerb
		xor rax,rax ;Make RoutingType = 0
		jmp ItemDistributionExit
	
	CursedWeapon:
		xor rax,rax
		jmp ItemDistributionExit
		
	NotHerb:
		call CParty_GetRoutingType
		jmp ItemDistributionExit
	
ItemDistribution endp
end