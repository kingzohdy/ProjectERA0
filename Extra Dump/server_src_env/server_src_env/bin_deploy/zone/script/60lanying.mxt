

proc 1
	dialog 1
	add_talk $_npc$:
	get_pworld_var 103 8 1 43 

	if 43 = 0
	;普通
		add_talk #b#b让我瞧瞧，这是谁呀？呀！大名鼎鼎的$_player$！
		add_talk #b#b我已经打探好了，要打开囚牢结界，就必须要杀死结界守护者：驭兽者、丛林德鲁伊、牧树人、古武族首领。
		add_talk #b#b我会帮你们破除这一道结界之门，当你们击杀了结界守护者，另一道结界之门就会打开！
		add_talk #b#b你们只需要帮我破除结界就好，救人这项工作就交给我吧！
		add_talk #b#b祝你们好运！
		option 151 #开启结界大门
		
	endif
	if 43 = 1
		;专家
		add_talk #b#b结界的力量真是太强大了，$_player$，我没办法独立完成救人工作，急需你们的帮助！
		add_talk #b#b在我破除这道结界之前，有一些话我必须要说！
		add_talk #b#b我们的时间很紧迫！一旦破除这道结界之门，我们必须用最短的时间救人，否则我们会连同囚牢结界一起毁灭！并且囚牢结界坚固异常，单凭人力根本无法撼动！所以，你们必须要找一些科技机械，只有科技机械才能摧毁囚牢结界，才能救出我们的战友！
		add_talk #b#b$_player$，希望你能理解我刚才所说的话，如果你准备好了，就告诉我！
		
		option 150 #开启结界之门
	endif
	if 43 = 10
		add_talk #b#b你已经帮我解决麻烦了吗？
	endif
	talk
endproc 
proc 150
	get_pworld_var 103 8 1 43 
	if 43 != 1
		return
	endif
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;专家副本倒计时;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	strvar pwid
	GET_ID_PLAYER_INFO 0 20 pwid
	;获取点击npc时角色所在的副本id
	
	strvar time bian
	var bian 1800
	
	strvar pw1 pw2 pw3 pw4 pw5 pw6
	strvar ret play player1 player2 player3 player4 player5 player6 num
	GET_TEAM_PLAYER_ID ret 0 num player1 player2 player3 player4 player5 player6  
	if ret = 0
		GET_ID_PLAYER_INFO @player1 20 pw1
		if pwid = @pw1
			COUNT_DOWN 11 @player1 @bian 拯救蓝鹰队员倒计时：
		endif
		GET_ID_PLAYER_INFO @player2 20 pw2
		if pwid = @pw2
			COUNT_DOWN 11 @player2 @bian 拯救蓝鹰队员倒计时：
		endif
		GET_ID_PLAYER_INFO @player3 20 pw3
		if pwid = @pw3
			COUNT_DOWN 11 @player3 @bian 拯救蓝鹰队员倒计时：
		endif
		GET_ID_PLAYER_INFO @player4 20 pw4
		if pwid = @pw4
			COUNT_DOWN 11 @player4 @bian 拯救蓝鹰队员倒计时：
		endif
		GET_ID_PLAYER_INFO @player5 20 pw5
		if pwid = @pw5
			COUNT_DOWN 11 @player5 @bian 拯救蓝鹰队员倒计时：
		endif
		GET_ID_PLAYER_INFO @player6 20 pw6
		if pwid = @pw6
			COUNT_DOWN 11 @player6 @bian 拯救蓝鹰队员倒计时：
		endif
	else
		get_player_id 0 55
		COUNT_DOWN 11 @55 @bian 拯救蓝鹰队员倒计时：
	endif
	
	;记录倒计时总时间
	var 10 @bian
	set_pworld_var 24 32 @10 1
	
	strvar time 
	;记录从1970到创建副本时多少秒
	GET_TIME_SEC 20
	get_pworld_var 16 32 1 time
	var time @20
	set_pworld_var 16 32 @time 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
	;删除阻挡大门
	get_pworld_var 12 32 1 13 
	DEL_ID_MON @13 1
	;开启专家副本的判断-计时锁
	get_pworld_var 107 8 1 47 
	var 47 1
	set_pworld_var 107 8 @47 1
	
;;;;;;;;;;;;;传送;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	strvar ret play player1 player2 player3 player4 player5 player6 num
;	GET_TEAM_PLAYER_ID ret 0 num player1 player2 player3 player4 player5 player6  
;	if ret = 0
;
;		TRANSMIT_CAMERA @player1 0 11600 20600 30900 0 0
;		TRANSMIT_CAMERA @player2 0 11600 20600 30900 0 0
;		TRANSMIT_CAMERA @player3 0 11600 20600 30900 0 0
;		TRANSMIT_CAMERA @player4 0 11600 20600 30900 0 0
;		TRANSMIT_CAMERA @player5 0 11600 20600 30900 0 0
;		TRANSMIT_CAMERA @player6 0 11600 20600 30900 0 0
;		
;	else
;	
;		GET_PLAYER_ID 1 play
;		TRANSMIT_CAMERA @play 45 11600 20600 30900 135 0
;	endif
	;锁选项
	var 43 10
	set_pworld_var 103 8 @43 1 	
	
	;开锁第一波怪物
	strvar roc
	get_pworld_var 101 8 1 roc
	var roc 1
	set_pworld_var 101 8 @roc 1
	
	add_sys_msg_start
	add_sys_msg 驭兽者：发现异族入侵，所有猛兽向我集结，30秒后发动攻击
	set_sys_msg_mode 4 0 0
	map_msg_info
	
	get_player_mapinfo 1 20 1 1 1 
	if 20 = 11800 or 20 = 11900
		;6.27增加 npc冒泡
		strvar mid
		GET_MAP_NPC_ID 201103 mid 1
		mon_speech @mid 0 2 1 刚刚前面好像有个流浪汉跑过去了,不知道跑来这里做什么
	endif
endproc 



proc 151

	get_pworld_var 103 8 1 43 
	if 43 != 0
		return
	endif
	;锁选项
	var 43 10
	set_pworld_var 103 8 @43 1 	
	
	;删除阻挡大门
	get_pworld_var 12 32 1 13 
	DEL_ID_MON @13 1
	
	
	;开锁第一波怪物
	strvar roc
	get_pworld_var 101 8 1 roc
	var roc 1
	set_pworld_var 101 8 @roc 1
	
	add_sys_msg_start
	add_sys_msg 驭兽者：发现异族入侵，所有猛兽向我集结，30秒后发动攻击
	set_sys_msg_mode 4 0 0
	map_msg_info
	
	
	get_player_mapinfo 1 20 1 1 1 
	if 20 = 11800 or 20 = 11900
		
		;6.27增加 npc冒泡
		strvar mid
		GET_MAP_NPC_ID 201103 mid 1
		mon_speech @mid 0 2 1 刚刚前面好像有个流浪汉跑过去了,不知道跑来这里做什么
	endif
endproc












