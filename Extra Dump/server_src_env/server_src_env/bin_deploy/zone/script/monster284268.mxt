;»Î«÷’ﬂ-III
proc 2
	var 10 11
	clan_everyday_pworld_val_chg 0 @10 1
	strvar hp x y
	GET_MON_INFO 0 0 0 hp 0 0 x y
	if hp = 0

			NEW_TMP_ITEM 1460302 @x @y 0 0 60 1

	
	endif
endproc
