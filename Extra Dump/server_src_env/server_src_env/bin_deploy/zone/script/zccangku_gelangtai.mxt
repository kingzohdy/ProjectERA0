 

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b欢迎光临“联盟银行”，我是负责仓库管理的机器人$_npc$，提供与物品、钱币相关的存储服务。
	add_talk #b#b#c186506为了您的账号财产安全，请启用背包【快捷键 B 】安全锁。#n


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
