;玛雅藏宝洞

PROC 1

	new_npc 1 333003 18805 3310 0 zhu30out.mac
	new_npc 1 333025 19236 7171 0 zhu30out.mac
	CIRCLE_NEW_MON 200028 40 18800 7600 2000 1 40 0 0 0 0 1 2



		new_mon_by_dir 294008 1 18800 7053 0 0 0 90 1 2
		new_mon_by_dir 294008 1 18800 8085 0 0 0 0 1 2
		new_mon_by_dir 294008 1 19350 7577 0 0 0 45 1 2
		new_mon_by_dir 294008 1 18331 7577 0 0 0 45 1 2
		new_npc 1 293171 17800 6700 0 xianjishi.mac
		new_npc 1 293142 17800 8400 0 xianjishi.mac
		new_npc 1 293172 19600 6700 0 xianjishi.mac
		new_npc 1 293173 19600 8400 0 xianjishi.mac


		GET_PLAYER_BASE_ATT 0 40 41 42
		if @41 >= 20 and @41 < 30
		CALL_MON 1 293121 100 600 18819 7577 1
		endif
		if @41 >= 30 and @41 < 40
		CALL_MON 1 293122 100 600 18819 7577 1
		endif
		if @41 >= 40 and @41 < 50
		CALL_MON 1 293131 100 600 18819 7577 1
		endif
		if @41 >= 50 and @41 < 60
		CALL_MON 1 293132 100 600 18819 7577 1
		endif
		if @41 >= 60 and @41 < 70
		CALL_MON 1 293133 100 600 18819 7577 1
		endif
		if @41 >= 70 and @41 < 80
		CALL_MON 1 293134 100 600 18819 7577 1
		endif
		if @41 >= 80 and @41 < 90
		CALL_MON 1 293135 100 600 18819 7577 1
		endif
		if @41 >= 90 and @41 < 100
		CALL_MON 1 293136 100 600 18819 7577 1
		endif
		if @41 >= 100 and @41 < 110
		CALL_MON 1 293137 100 600 18819 7577 1
		endif
		if @41 >= 110 and @41 <= 120
		CALL_MON 1 293138 100 600 18819 7577 1
		endif

	var 60 0
	set_pworld_var 0 32 @60 1

ENDPROC

PROC 3
		get_pworld_var 0 32 1 60
			add_sys_msg_start
			add_sys_msg 60=$@60$
			set_sys_msg_mode 4 0 0
			msg_info
			
		if @60 = 4
			add_sys_msg_start
			add_sys_msg 献祭石的封印全被解除，通往祭坛的路已被打开
			;神灵收到新鲜的祭品，解除了中央祭台的封印
			set_sys_msg_mode 4 0 0
			MAP_MSG_INFO
			delete_mon_by_defid 294008 1
			var 60 5
			set_pworld_var 0 32 @60 1
		endif
ENDPROC


PROC 4
	
	
ENDPROC

proc 5
	effect 2 0 playEffct(18790,5127,"effect\\entity\\9598_16.ent")
	ADD_SYS_MSG_START
	ADD_SYS_MSG 收集邪灵祭司身上的“人类徽记”解除献祭石的封印，即可通往祭坛取得宝藏
	;一群玛雅人正在举行古怪的祭祀活动，也许他们的头颅就是最好的祭品
	SET_SYS_MSG_MODE 7 0 0
	MSG_INFO
endproc


proc 6

endproc
