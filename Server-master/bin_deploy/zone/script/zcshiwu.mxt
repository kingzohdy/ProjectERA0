;食物


proc 1

	DIALOG 1

	add_talk $_npc$:
	add_talk #b#b欢迎光临！这里都是最新鲜出炉的食物，请随便挑选！
	add_talk #b#b放心哦，我们店使用的全是安全食材，诚信可是经商者必备的品德。嘿嘿！
	add_talk #b#b但是浪费粮食可是万万不可的哦！亲！

	
;	task_add_option 1081 1 100
		
	option 120 #(买卖)打开商店
	strvar RetTsk
	task_stat 3128 1 RetTsk
	if RetTsk = 2
		option 200 这是圣诞老公公给你的礼物
	endif
	
	strvar Ret
	task_stat 3674 1 Ret
	if Ret = 2
		option 250 我替公会送来了你要的生产物资
	endif

	
	task_stat 4147 1 50
	if 50 = 2
		option 300 #我替大马丁送来了你订购的物资
	endif
	
	talk
endproc 	


;PROC 100
;	task_do_option 1081 1
;ENDPROC


;proc 2
;	get_trig_task 1 2
;	if 2 = 3110
;		task_fini 3110 1
;	
;		NEW_OWNER_TMP_ITEM 1460071 27100 24900 0 0 0 311 1095 1 3600 1
;	endif 
;endproc 


proc 120
	OPEN_NPC_SHOP 
endproc 
proc 200 
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b 谢谢你，$_player$！谢谢你，圣诞老公公，亲！
		option 210 #交出圣诞礼物
	talk
endproc 
proc 210
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
			task_fini 3128 1
		endif
	endif
endproc 
	
	
proc 250
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b谢谢你，明天制作蛋糕的材料总算有着落了。
		option 251 #交出生产物资
	talk
endproc 
proc 251
	GET_ITEM_NUM_INPAK 5300871 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品！
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
			
		dec_item_num_inpak 5300871 1 30
		if 30 != -1
			task_fini 3674 1
		endif
	endif
endproc


proc 300
	GET_ITEM_NUM_INPAK 5301003 0 11
	if 11 < 1
	
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品！
		set_sys_msg_mode 1 0 0
		msg_info
	else
	
		dec_item_num_inpak 5301003 1 30
		if 30 != -1
			task_fini 4147 1
		endif
	endif
endproc

