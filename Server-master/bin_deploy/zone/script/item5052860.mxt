
;Ò»Ð¡¹Þ½ð±Ò

proc 1
	strvar ret pmid
	DEC_ITEM_NUM_INPAK 5052860 1 ret
	if ret = 0
		GET_PLAYER_ID 1 pmid
		add_money @pmid 20 61 1
	endif
endproc 