
;小娟
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b别看我个子小，我已经有几千岁了呢！
	add_talk #b#b你看我的眼睛、鼻子、耳朵、嘴巴，哪一点像小朋友了？我看你是老眼昏花了……
	
	strvar ret
	task_stat 3316 1 ret
	if ret = 2
		option 150 #我们来玩躲猫猫吧，我来躲，你来找，如何？
	endif
	talk
endproc

proc 150
	strvar tmpid ret
	GET_ROLE_TMP_ITEM 0 tmpid
	HAS_STATUS 3171 ret
	if ret = 0
;	if tmpid = 1460083

	
		get_player_id 1 10
		TARGET_ADD_STATUS 1 @10 3171 1 0 0
	;	DEL_ROLE_TMP_ITEM  0 0
		task_fini 3316 1
	else
		add_sys_msg_start
		add_sys_msg 你不是小玲，我才不要跟你玩！
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc 