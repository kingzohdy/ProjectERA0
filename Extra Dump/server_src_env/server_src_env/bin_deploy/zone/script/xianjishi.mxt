
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b只有怀着谦卑的心，献上五颗人类徽记作为祭品，方可解除献祭石的封印，打开通往祭坛的道路。
	;凡人，怀着一颗谦卑的心，献上你的祭品，然后便可收获神灵的恩赐

	option 300 #献祭人类徽记
	;向神灵献上五颗人类徽记

	talk
endproc


PROC 300
	GET_ITEM_NUM_INPAK 5300584 0 30
	if 30 >= 5
		DEC_ITEM_NUM_INPAK 5300584 5 1
		DEL_NPC 0 1
		get_pworld_var 0 32 1 60
		add 60 1
		add_sys_msg_start
		add_sys_msg 解除献祭石封印 $@60$/4
		;献祭神灵 $@60$/4
		set_sys_msg_mode 3 0 0
		msg_info
		set_pworld_var 0 32 @60 1
	else
		add_sys_msg_start
		add_sys_msg 献祭需要5颗人类徽记
		;没有足够的徽记
		set_sys_msg_mode 4 0 0
		msg_info
	endif

ENDPROC
