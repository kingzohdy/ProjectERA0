

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b据太阳神殿的侦查员回报，黑暗议会似乎又在蓄谋着某种神秘仪式，我们必须赶在他们成功前阻止这起阴谋！
	add_talk #b#b但现在时间紧迫，所以自治联盟绝定召集各路勇者协助，只要有足够的实力与团结队伍，便可找我传送到太阳神殿中。
	add_talk #b#b#c186506[太阳神殿]进入条件#n	
	add_talk #b#b#c186506人物等级：75级#n	
;	add_talk #b#b通往太阳神殿的传送门已经损坏,正在修复中……
	option 110 #进入太阳神殿
	
	talk
endproc 
proc 20 
	return
endproc

proc 110
;	ENTER_PWORLD 100 0 0
;
;		
;		TEAM_PWORLD_INFO 300 100 0 0
;		if 300 = 0
;			ENTER_PWORLD 100 0 0
;		OPEN_PWORLD_UI 100
		OPEN_PWORLD_UI 11
;		else
;	
;			IS_CAPTAIN 15 0
;			if 15 = 0 
;				open_pworld_ui 100
;			else
;
;				add_sys_msg_start
;;				add_sys_msg 你不是队长！此副本需要队长操作进入！
	;			set_sys_msg_mode 4 0 0
;				msg_info
;
;			endif
;		endif
endproc