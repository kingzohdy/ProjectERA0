; 测试对话脚本指令
;
PROC 1
	GET_PLAYER_BASE_ATT 4 1 2 3
	DIALOG 1
	IF 3 = 1
	ADD_TALK $_npc$：
	ADD_TALK 你好！$_player$,你是位战士,我是这里的仓库管理员.有什么能帮你的吗?
	ELSEIF 3 = 2
	ADD_TALK $_npc$：
	ADD_TALK 你好！$_player$,你是位法师,我是这里的仓库管理员.有什么能帮你的吗?
	ELSE
	ADD_TALK $_npc$：
	ADD_TALK 你好！$_player$,你是位枪手,我是这里的仓库管理员.有什么能帮你的吗?
	ENDIF
   OPTION 22 #进入副本
   OPTION 23 #打开仓库.
   TALK               
ENDPROC

PROC 22
	ENTER_PWORLD 100 0 1
ENDPROC  

PROC 23
	OPEN_STASH	
	RETURN
ENDPROC 


