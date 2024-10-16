proc 1
	dialog 1

	add_talk $_npc$:
	add_talk #b#b族人的血不能白流，我要为他们报仇雪恨！
;	task_add_option 1335 1 101
	task_add_option 1336 1 102	
	task_add_option 1312 1 103
	task_add_option 1337 1 104
	
	strvar Ret
	task_stat 1337 1 Ret
	if Ret = 2
;		option 300 是否需要我送您一程？
	endif
	
	TASK_STAT 1318 0 50	
	if 50 = 2
		GET_ITEM_NUM_INPAK 5300415 0 83
	    GET_ITEM_NUM_INPAK 5300416 0 84
		if 83 = 0 and 84 = 0
;			option 199 领取审讯工具
	endif
	endif
	talk
endproc

;proc 101
;    task_do_option 1335 1
;endproc
proc 102
    task_do_option 1336 1
endproc
proc 103
    task_do_option 1312 1
endproc
proc 104
    task_do_option 1337 1
endproc
proc 199
		dialog 1
		add_talk $_npc$:
		add_talk #b#b我这里有一些审问犯人可能会用到的东西，你就随便挑一两样吧。我们一定要从他的嘴里得到有价值的东西！所以，你可以根据情况决定使用什么刑具。
		add_talk #b#b由我来问问题，你从旁协助，等你挑选好合适的刑具之后，咱们就开始审讯！
		option 200 #领取鞭子
;		option 201 #领取巴掌
		
		talk
endproc 
proc 200
		GET_ITEM_NUM_INPAK 5300415 0 80
		if 80 < 1
			ADD_ITEM_NUM_INPAK 5300415 1 1 0
			add_sys_msg_start
			add_sys_msg 你获得了一个鞭子，关键时候使用它协助祖宾审讯
			set_sys_msg_mode 3 0 0
			msg_info
		endif
endproc 
proc 201
		get_task_var 1318 3 8 1 13
		var 13 1
		set_task_var 1318 3 8 @13 1
	    GET_ITEM_NUM_INPAK 5300416 0 81
		if 81 < 1
			ADD_ITEM_NUM_INPAK 5300416 1 1 0
			add_sys_msg_start
			add_sys_msg 你获得了一个巴掌，关键时候使用它协助祖宾审讯
			set_sys_msg_mode 3 0 0
			msg_info
		endif	
endproc 

proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我可以直接将您送到目的地，但是中途不会停哟！
	
		option 301 #请直接将我送到目的地吧！
		option 302 #不用啦，我自己过去！
	talk

endproc 
proc 301
	NEW_OWNER_TMP_ITEM 1460061 13000 13300 0 0 0 302 1095 1 3600 1
endproc 
proc 302
	return
endproc 

proc 2
	get_trig_task 1 2
	if 2 = 1337
;		dialog 1
;		add_talk $_npc$:
;		add_talk #b#b族人的血不能白流，我要为他们报仇雪恨！
		task_fini 1337 1
;		option 300 是否需要我送您一程？
;		talk
		;;;协助
	endif

endproc 
