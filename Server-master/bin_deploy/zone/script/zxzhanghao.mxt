
;安全账号

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b$_player$，你认为在零纪元，最重要的是什么？
	add_talk #b#b当然是账号安全了，如果你的账号安全得不到保障，那么等级、装备、金币什么的都是浮云啊！
	add_talk #b#b说不定那些不法份子还会以你的名义去欺骗你的好友，让你名利皆失啊！

	task_add_option 2260 1 100
	task_add_option 2261 1 101
	
	
;	player_need_new_name 1
;	if 1 > 0
;		option 190 #修改角色名
;	endif
	clan_need_new_name 1
	if 1 > 0
		option 191 #修改公会名
	endif
	
	talk
	
endproc 

proc 190
	OPEN_UI 17
endproc

proc 191
	open_ui 53
endproc


PROC 100
	task_do_option 2260 1
ENDPROC

PROC 101
	task_do_option 2261 1
ENDPROC

proc 2
	
	get_trig_task 1 2
	if 2 = 2260
		IS_UIN_FLAG_SET 30 0 64
		if 30 = 1
			task_fini 2260 1
		endif
	endif
	
	if 2 = 2261
		IS_UIN_FLAG_SET 30 0 32
		if 30 = 1
			task_fini 2261 1
		endif
	endif
endproc 















