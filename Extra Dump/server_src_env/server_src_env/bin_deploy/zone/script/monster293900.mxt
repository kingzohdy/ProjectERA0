
proc 2
	ADD_SYS_MSG_START
	rand 20 100
	; 幻化果实
	if @20 >= 0 and @20 <= 59
		ADD_SYS_MSG 开启地心宝箱，获得幻化果实
		ADD_BIND_ITEM_NUM_INPAK 5233000 1 1 1
	; 基因果实
	elseif @20 >= 60 and @20 <= 79
		ADD_SYS_MSG 开启地心宝箱，获得基因果实
		ADD_BIND_ITEM_NUM_INPAK 5233500 1 1 1
	; 遗忘果实
	else
		ADD_SYS_MSG 开启地心宝箱，获得遗忘果实
		ADD_BIND_ITEM_NUM_INPAK 5219600 1 1 1
	endif 
	SET_SYS_MSG_MODE 4 0 0
	MAP_MSG_INFO
	;DESTROY_MON 1
	bct_ui 0 0
endproc 

proc 6
	GET_PACK_SPACE 0 10
	if @10 <= 0
		ADD_SYS_MSG_START
		ADD_SYS_MSG 背包无法容纳更多物品，请整理背包
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
		PLAYER_COLLECT_BREAK
	endif
endproc 