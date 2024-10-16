;新手教程副本

PROC 1
	NEW_MON 204006 2 37615 19488 150 0 0 1 2
	ADD_SYS_MSG_START
	ADD_SYS_MSG 进入试炼场副本！
    SET_SYS_MSG_MODE 4 0 0
	MAP_MSG_INFO
	
	VAR 10 0 				
    SET_PWORLD_VAR 0 32 @10 1
	VAR 11 0 				
    SET_PWORLD_VAR 4 32 @11 1
	VAR 12 0 				
    SET_PWORLD_VAR 8 32 @12 1

	effect 0 0 playEffct(36900,19550,180,"effect\\entity\\9570_16.ent")
ENDPROC

PROC 3
	GET_PWORLD_VAR 4 32 1 11
	GET_PWORLD_VAR 8 32 1 12
	
	GET_PWORLD_VAR 0 32 1 10
		IF 10 = 2 AND 12 = 0
			add 11 1
				IF 11 = 2
					EFFECT 0 0 playEffct(37505,18868,"effect\\entity\\5500002_15.ent")
				endif
				if 11 = 3 
					EFFECT 0 0 playEffct(37505,20165,"effect\\entity\\5500002_15.ent")
				ENDIF
				IF 11 = 4
					NEW_MON 261001 5 37505 18868 100 0 0 1 2
					NEW_MON 261001 5 37505 20165 100 0 0 1 2
					VAR 12 1
				ENDIF
			SET_PWORLD_VAR 4 32 @11 1
			SET_PWORLD_VAR 8 32 @12 1
		ENDIF
		IF 10 = 12 AND 12 = 1
			add 11 1
				if 11 = 5
					new_npc 1 333003 38529 19425 0 zhu30out.mac
					ADD_SYS_MSG_START
					ADD_SYS_MSG 副本已完成，可从传送门离开，3秒后副本关闭
					set_sys_msg_mode 4 0 0
					MAP_MSG_INFO
				endif
				if 11 = 9
					COMPEL_CLOSE_PWORLD 1
					VAR 12 3
					; ADD_SYS_MSG_START
					; ADD_SYS_MSG 副本即将关闭，20秒后传送出副本。
					; set_sys_msg_mode 4 0 0
					; MAP_MSG_INFO	
				endif
				; if 11 = 15
					; ADD_SYS_MSG_START
					; ADD_SYS_MSG 副本即将关闭，10秒后传送出副本。
					; set_sys_msg_mode 4 0 0
					; MAP_MSG_INFO	
				; endif
				; if 11 = 20
				; endif
			SET_PWORLD_VAR 4 32 @11 1
			SET_PWORLD_VAR 8 32 @12 1
		endif
		
ENDPROC


PROC 4
	GET_PWORLD_VAR 0 32 1 10
	add 10 1
	SET_PWORLD_VAR 0 32 @10 1
	If 10 = 2
		NEW_TMP_ITEM 1420013 37598 19485 0 0 0 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG 竞技场中央出现一把火焰刀，左键点击拾取它可以得到威力巨大的技能
		set_sys_msg_mode 4 0 0
		MAP_MSG_INFO
	endif
	
ENDPROC
