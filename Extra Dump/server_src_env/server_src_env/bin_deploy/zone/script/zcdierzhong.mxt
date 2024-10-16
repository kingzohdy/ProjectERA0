proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b不知道这个方向看不看得到细语丘陵。
	add_talk #b#b你去过细语丘陵吗，在战鼓村那里有一个我的爱人，为了生计他只能出去卖东西赚钱，我已经很久没见过他了。
	add_talk #b#b如果你要去细语丘陵，麻烦你帮我看看他好不好，只要他好我也好。
	
	;五一活动
	task_stat 4438 1 51
	if 51 = 2
		option 510 节日慰问——送上礼物
	endif
	
	
	strvar RetTsk
	task_stat 3123 1 RetTsk
	if RetTsk = 2
		option 200 这是圣诞老公公给你的礼物
	endif
	talk
	
endproc
proc 200 
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b请代我感谢圣诞老公公，让我有一个难忘的圣诞，还有你，$_player$，谢谢你不辞辛苦！
		option 201 #交出圣诞礼物
	talk
endproc 
proc 201
	GET_ITEM_NUM_INPAK 5300844 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品！
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
			
		dec_item_num_inpak 5300844 1 30
		if 30 != -1
			task_fini 3123 1
		endif
	endif
endproc 

;五一活动
proc 510
	GET_ITEM_NUM_INPAK 5052308 1 10
	STRVAR flag
	var flag 0
	if 10 > 0
		DEC_ITEM_NUM_INPAK 5052308 1 11
		if 11 = 0
			var flag 1
			task_fini 4438 1
			
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b我一直以为这个世界已经将我遗忘！没想到还有人记得我！
			option 511 #打开心扉，阳光就会照进来！
			talk
		endif
	endif
	
	if flag = 0
		add_sys_msg_start
		add_sys_msg 您的背包里没有“慰问物资”。
		set_sys_msg_mode 1 0 0
		msg_info
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc