 

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b欢迎光临“联盟银行”，我是负责仓库管理的机器人巴拉贡，提供物品与钱币的存储服务。
	add_talk #b#b不过我还是建议你别把食物放进来，因为我是吃肉不吃油的机器人…
	add_talk #b#b#c186506仓库可存取背包物品#n
	task_add_option 4039 1 103


	OPTION 104 #打开仓库
;	option 200 #联邦金币交易所
	option 130 #修改安全锁密码
	option 131 #取消安全锁密码


	TASK_STAT 12009 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (环次)徽章送来了
	endif
	TASK_STAT 12209 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (环次)这是带给你的地图
	endif
	talk
endproc 

PROC 103
	task_do_option 4039 1
ENDPROC
PROC 104
	OPEN_UI 0
ENDPROC
proc 200
	OPEN_UI 35
endproc 

proc 130
	OPEN_UI 55
endproc
proc 131
	open_ui 56
endproc
PROC 160
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b商路的拓展，如果可以的话，我希望时间城的各位都能够享受到它带来的好处。

	OPTION 260 #(环次)谢谢你的支持！
	TALK
ENDPROC
PROC 260
	GET_ITEM_NUM_INPAK 5300511 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300511 1 0
	task_fini 12209 1
	endif
ENDPROC
proc 2
	get_trig_task 1 2
	if 2 = 1097
		task_fini 1097 1
	endif

endproc
