



proc 2
	strvar map
	GET_PLAYER_MAPINFO 0 map 0 0 0
	if map = 30000 or map = 30001 or map = 30002 or map = 30003 or map = 30004 or map = 30005 
		strvar ret
		task_stat 3173 1 ret
		if ret = 2
			strvar num
			GET_ITEM_NUM_INPAK 5300888 1 num
			if num < 10
				ADD_ITEM_ON_MAP 1 0 5300888 1 0 0 0 0
			endif
		endif
	endif
endproc  