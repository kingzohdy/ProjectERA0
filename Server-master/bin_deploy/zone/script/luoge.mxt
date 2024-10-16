proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b飞翔在天际之间,没有束缚的追寻自己存在的意义,这就是姬玛的愿望。
	option 11 #带我去下面吧!
;	option 13 #再见
	talk
endproc

proc 11
	GET_PWORLD_VAR 0 32 1 10
	if @10 >= 5
	autofly 105 1
	;get_pworld_var 16 32 1 14
	;var 14 5
	;set_pworld_var 16 32 @14 1
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG 周围还有很多风行者，挑战哥布林王之前先拿它们练练手吧
		set_sys_msg_mode 4 0 0
		MAP_MSG_INFO
	endif
endproc 

;proc 13
;	return
;endproc
