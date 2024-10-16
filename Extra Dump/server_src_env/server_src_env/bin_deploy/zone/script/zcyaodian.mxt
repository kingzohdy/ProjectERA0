

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b其实开始我不打算卖食物的，因为我不能说卖就卖。
	add_talk #b#b第一我要试一试，我不愿意夸大其词欺骗消费者，明明是地沟油，却说是花生油，这样生意一定没法继续做。
	add_talk #b#b后来经过我亲自证实，吃了大概3天左右，明显感觉英俊了，身体强壮了，身高一路飙升！
	add_talk #b#b所以我强烈推荐各位冒险途中的勇士，我这出售的食物，吃了后保证你美丽动人，强壮挺拔！
	add_talk #b#b#c186506出售回复“生命值”与“魔法值”的食物#n	
	
	;五一活动
	task_stat 4434 1 51
	if 51 = 2
		option 510 节日慰问——送上礼物
	endif
	
	task_add_option 1092 1 101
	task_add_option 1093 1 102
	task_add_option 1301 1 103
	task_add_option 1081 1 104
;	task_add_option 3004 1 105
	
;	task_add_option 1086 1 106
;	task_add_option 1087 1 107
;	task_add_option 1088 1 108
	
		OPTION 110 #(买卖)打开商店

	TASK_STAT 3004 1 20
	if 20 = 2
;		option 300 时间城太大了，能否送我一程？
	endif
	talk
endproc 

PROC 101
	task_do_option 1092 1
ENDPROC
PROC 102
	task_do_option 1093 1
ENDPROC
PROC 103
	task_do_option 1301 1
ENDPROC
PROC 104
	task_do_option 1081 1
ENDPROC
;PROC 105
;	task_do_option 3004 1
;ENDPROC

PROC 106
	task_do_option 1086 1
ENDPROC
PROC 107
	task_do_option 1087 1
ENDPROC
PROC 108
	task_do_option 1088 1
ENDPROC



PROC 110
	OPEN_NPC_SHOP
ENDPROC

proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b当然可以，您可是个大人物呢。
	add_talk #b#b不过，我是直接将您送到目的地，中途不会停的哟！
	add_talk #b#b您确定要乘坐直达班车吗？
		option 301 #请直接将我送到目的地吧！
		option 302 #不用啦，我自己过去，还能逛逛时间城！
	talk
endproc
proc 301
;	NEW_OWNER_TMP_ITEM 1460071 27100 24900 0 0 0 311 1095 1 3600 1	
	AUTO_MOVE 0 1000 29400 26700 1460071 0 3081 1 3600
endproc 
proc 302
	return
endproc 
proc 2
	get_trig_task 1 2
	if 2 = 1093
		task_fini 1093 1
	endif 
;

	if 2 = 1301
		task_fini 1301 1
	endif 
	if 2 = 3110
;		DIALOG 1
;		add_talk $_npc$:
;		add_talk #b#b其实开始我不打算卖食物的，因为我不能说卖就卖。
;		add_talk #b#b第一我要试一试，我不愿意夸大其词欺骗消费者，明明是地沟油，却说是花生油，这样生意一定没法继续做。
;		add_talk #b#b后来经过我亲自证实，吃了大概3天左右，明显感觉英俊了，身体强壮了，身高一路飙升！
;		add_talk #b#b所以我强烈推荐各位冒险途中的勇士，我这出售的食物，吃了后保证你美丽动人，强壮挺拔！
;		add_talk #b#b#c186506出售回复“生命值”与“魔法值”的食物#n	
;				
;			task_fini 3110 1
;			option 300 时间城太大了，能否送我一程？
		talk

	endif 
	
	
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
			task_fini 4434 1
			
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b谢谢你，$_player$，请将我的祝福带给艾劳，顺便替我谢谢她！
			add_talk #b#b我会买很多的材料，挥动快乐的炒勺，唱着幸福的歌谣，带着美好的欢笑，做出一桌美味佳肴，亲爱的，一起庆祝节日吧！
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