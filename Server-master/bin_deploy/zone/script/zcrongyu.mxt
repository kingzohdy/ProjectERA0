

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b上帝永远眷顾着付出努力的人，欢迎光临时间城军需指定销售商。
	add_talk #b#b在这混乱时期，为了激励群众的意志，我们承诺，对那些为时间城作出卓越贡献的勇者，我们将提供独一无二的货物。
	add_talk #b#b#c186506出售需要声望购买的物品#n
	
	
	
	
	task_add_option 1121 1 102
	task_add_option 1200 1 103
	task_add_option 4133 1 104

	task_add_option 2000 1 105
	task_add_option 2001 1 106

	task_add_option 4200 1 107
	task_add_option 4201 1 108
	task_add_option 4202 1 109

;50级
	task_add_option 2200 1 110

	task_add_option 2138 1 116
	
;	strvar Ret 
;	task_stat 3676 1 Ret
;	if Ret = 2 
;		option 300 这里有张凭证需要你签名
;	endif

	OPTION 101 #(买卖)声望商店
	
	task_stat 4491 1 20
	if 20 = 2
		option 200 #向时间城声望商人沙丽娜赠送一个粽子
	endif
	talk
endproc 


PROC 101
	OPEN_NPC_SHOP
ENDPROC

PROC 102
	task_do_option 1121 1
ENDPROC
PROC 103
	task_do_option 1200 1
ENDPROC
PROC 104
	task_do_option 4133 1
ENDPROC
PROC 105
	task_do_option 2000 1
ENDPROC
PROC 106
	task_do_option 2001 1
ENDPROC
PROC 107
	task_do_option 4200 1
ENDPROC
PROC 108 
	task_do_option 4201 1
ENDPROC
PROC 109
	task_do_option 4202 1
ENDPROC
PROC 110
	task_do_option 2200 1
ENDPROC

PROC 116
	task_do_option 2138 1
ENDPROC

proc 300
	dialog 1
	add_talk $_npc$:
	add_talk #b#b原来是公会的领取凭证，现在就帮你签上，物资我已经送到亨利那了。
		option 301 #拿出领取凭证让沙丽娜签名

	talk
endproc 

proc 301
	GET_ITEM_NUM_INPAK 5300879 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品！
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
		dec_item_num_inpak 5300879 1 30
		if 30 != -1
			ADD_ITEM_NUM_INPAK 5300880 1 1 0
		endif
	endif
endproc


proc 200
	dialog 3
	add_talk $_npc$:
	add_talk #b#b请在下面的格子中放入#c186506相思粽#n或#c186506情义粽#n
	add_talk #b#b如果你没有粽子的话，就去粽老那儿制作吧！
	
	NPC_COMMIT_OPT 201
	
	talk
endproc 
proc 201
	strvar m_ID m_Pos1 m_Pos2 m_Col Ret RetTask
	GET_COMMIT_ITEM_INFO 1 m_ID
	if m_ID = -1
		add_sys_msg_start
		add_sys_msg 你还没有提交物品！
		set_sys_msg_mode 1 0 0
		msg_info		
		return 
	endif
	
	GET_COMMIT_ITEM_INFO 2 m_Pos1
	GET_COMMIT_ITEM_INFO 3 m_Pos2
	GET_COMMIT_ITEM_INFO 4 m_Col
	task_stat 4491 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4491 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4491 1
					call 502
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg 物品不符任务要求，未能成功上交
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	
	endif
endproc 
proc 502
	add_sys_msg_start
	add_sys_msg 成功上交物品，任务完成
	set_sys_msg_mode 3 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc 


proc 2
	get_trig_task 1 2
	if 2 = 1200
		task_fini 1200 1
	endif 
	if 2 = 2001
		task_fini 2001 1
	endif 

	if 2 = 4201 
		task_fini 4201 1
	endif 
	if 2 = 2200
		task_fini 2200 1
	endif 

endproc
