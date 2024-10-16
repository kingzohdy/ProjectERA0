
;一小包金币

proc 1
	strvar ret pmid
	DEC_ITEM_NUM_INPAK 5052859 1 ret
	if ret = 0
		GET_PLAYER_ID 1 pmid
		add_money @pmid 10 61 1
	endif
endproc 