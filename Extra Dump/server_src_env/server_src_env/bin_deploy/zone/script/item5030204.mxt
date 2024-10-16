;怪物宝箱

	;ADD_SYS_MSG_START
	;ADD_SYS_MSG 
	;SET_SYS_MSG_MODE 0 0 0
	;MSG_INFO
	

proc 1

;	strvar Num RetX
;	GET_PACK_SPACE 0 Num
;	if @Num < 2

;		ADD_SYS_MSG_START
;		ADD_SYS_MSG 开启宝箱会随机获得物品，请预留2个背包空格位置再开启！
;		SET_SYS_MSG_MODE 1 0 0
;		MSG_INFO
;		return
;	endif
	

	ITEM_ADD_ONEDAY 5030204 1
	;使用次数+1
;	GET_ITEM_ONEDAY_INFO 5030204 1 800 901
;	;获得使用次数	
;	if 800 <= 10
;	;使用20次以内  
;		DEC_ITEM_NUM_INPAK 5052800 1 RetX
;	else
;	
;	endif
	
endproc

	