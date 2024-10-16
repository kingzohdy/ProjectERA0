;怪物死亡的事件
proc 2
	var 10 -7
	clan_everyday_pworld_val_chg 0 @10 1
	
	strvar hp x y
	GET_MON_INFO 0 0 0 hp 0 0 x y
	if hp = 0
		rand 50 100
		if 50 < 6
			NEW_TMP_ITEM 1440100 @x @y 0 0 60 1
		endif
		if 50 >= 6 and 50 < 12
			NEW_TMP_ITEM 1460300 @x @y 0 0 60 1
		endif
		if 50 >= 12 and 50 < 18
			NEW_TMP_ITEM 1460301 @x @y 0 0 60 1
		endif
	endif
endproc