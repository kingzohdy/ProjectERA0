 

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b欢迎光临“联盟银行”，我是负责仓库管理的机器人$_npc$，提供与物品、钱币相关的存储服务。
	add_talk #b#b#c186506为了您的账号财产安全，请启用背包【快捷键 B 】安全锁。#n

	;五一活动
	task_stat 4431 1 51
	if 51 = 2
		option 510 节日慰问——送上礼物
	endif
	
	OPTION 104 #打开仓库
;	option 200 #联邦金币交易所
	option 130 #修改安全锁密码
	option 131 #取消安全锁密码
	talk
endproc 

PROC 104
	OPEN_UI 0
ENDPROC

proc 130
	OPEN_UI 55
endproc
proc 131
	open_ui 56
endproc

;五一活动
proc 510
	GET_ITEM_NUM_INPAK 5301004 1 10
	STRVAR flag
	var flag 0
	if 10 > 0
		DEC_ITEM_NUM_INPAK 5301004 1 11
		if 11 = 0
			var flag 1
			task_fini 4431 1
			
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b谢谢你，$_player$，请将我的祝福带给艾劳，顺便替我谢谢她！
			add_talk #b#b迈开步，踏上快乐的旅程；
			add_talk #b#b睁开眼，欣赏如画的风景；
			add_talk #b#b按下键，保存永远的幸福；
			add_talk #b#b谢谢你，传递快乐和满足。
			option 511 #没问题
			talk
		endif
	endif
	
	if flag = 0
		add_sys_msg_start
		add_sys_msg 您的背包里没有“礼品盒”。
		set_sys_msg_mode 1 0 0
		msg_info
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc