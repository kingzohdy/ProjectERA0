proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b真正的强大不是来自你的四肢，而是你内心是否坚定，进入试炼场便没有回头路了，做好准备。
	
			
		task_add_option 3141 1 101
		task_add_option 3142 1 102
		task_add_option 3143 1 103
		task_add_option 3144 1 104
		
		
		task_add_option 3147 1 105
		task_add_option 3148 1 106
		task_add_option 3149 1 107
		task_add_option 3150 1 108
		
		task_add_option 3152 1 109
		task_add_option 3162 1 110
		task_add_option 3154 1 111
		task_add_option 3155 1 112
		
		task_add_option 3157 1 113
		task_add_option 3158 1 114
		task_add_option 3159 1 115
		task_add_option 3160 1 116
		
		task_add_option 3161 1 117
		task_add_option 3145 1 118

		TASK_STAT 3143 1 20
		if 20 = 2
			option 200 #进入战士试炼之崖
		endif
		TASK_STAT 3148 1 20
		if 20 = 2
			option 201 #进入祭祀试炼之崖
		endif
		
		TASK_STAT 3161 1 20
		TASK_STAT 3162 1 21
		if 20 = 2 or 20 = 3 or 20 = 1
		
			if 21 != 1 and 21 != 3
				option 202 #进入枪手试炼之台
			endif
		endif
		
		TASK_STAT 3158 1 20
		if 20 = 2
			option 203 #进入法师试炼之台
		endif		

	talk
endproc

proc 200
		ENTER_PWORLD 505 0 0
		strvar Loc1
		get_task_var 3143 8 8 1 Loc1
		var Loc1 1
		set_task_var 3143 8 8 @Loc1 1
endproc 
proc 201
		strvar Ret
		ENTER_PWORLD 507 0 Ret
		
		strvar Loc1
		get_task_var 3148 8 8 1 Loc1
		var Loc1 1
		set_task_var 3148 8 8 @Loc1 1
endproc
proc 202
		ENTER_PWORLD 506 0 0
		
		strvar Loc1 Loc2
		get_task_var 3161 8 8 1 Loc1
		var Loc1 1
		set_task_var 3161 8 8 @Loc1 1
		
		get_task_var 3162 8 8 1 Loc2
		var Loc2 1
		set_task_var 3162 8 8 @Loc2 1
		
endproc 
proc 203
		strvar Ret
		ENTER_PWORLD 508 0 Ret 
		
		strvar Loc1
		get_task_var 3158 8 8 1 Loc1
		var Loc1 1
		set_task_var 3158 8 8 @Loc1 1
endproc

PROC 101
	task_do_option 3141 1
ENDPROC
PROC 102
	task_do_option 3142 1
ENDPROC
PROC 103
	task_do_option 3143 1
ENDPROC
PROC 104
	task_do_option 3144 1
ENDPROC

PROC 105
	task_do_option 3147 1
ENDPROC
PROC 106
	task_do_option 3148 1
ENDPROC
PROC 107
	task_do_option 3149 1
ENDPROC
PROC 108
	task_do_option 3150 1
ENDPROC

PROC 109
	task_do_option 3152 1
ENDPROC
PROC 110
	task_do_option 3162 1
ENDPROC
PROC 111
	task_do_option 3154 1
ENDPROC
PROC 112
	task_do_option 3155 1
ENDPROC

PROC 113
	task_do_option 3157 1
ENDPROC
PROC 114
	task_do_option 3158 1
ENDPROC
PROC 115
	task_do_option 3159 1
ENDPROC
PROC 116
	task_do_option 3160 1
ENDPROC
PROC 117
	task_do_option 3161 1
ENDPROC
PROC 118
	task_do_option 3145 1
ENDPROC

proc 2
	get_trig_task 1 2

	if 2 = 3144
		ADD_ITEM_NUM_INPAK 5300499 1 1 0
		ADD_ITEM_NUM_INPAK 5300513 1 1 0
		ADD_ITEM_NUM_INPAK 5300514 1 1 0
	endif
	if 2 = 3145
		
		task_fini 3145 1
	endif
	if 2 = 3150
		task_fini 3150 1
	
	endif
	if 2 = 3155
		task_fini 3155 1
	
	endif
	if 2 = 3160
		task_fini 3160 1
	
	endif
	if 2 = 3161
		task_fini 3161 1
	
	endif	
	if 2 = 3143
		dialog 1
		add_talk $_npc$:
		add_talk #b#b真正的强大不是来自你的四肢，而是你内心是否坚定，进入试炼场便没有回头路了，做好准备。
			option 200 #进入战士试炼之崖
			
		talk
	endif 
	if 2 = 3148
		dialog 1
		add_talk $_npc$:
		add_talk #b#b真正的强大不是来自你的四肢，而是你内心是否坚定，进入试炼场便没有回头路了，做好准备。
			option 201 #进入祭祀试炼之崖
			
		talk
	endif 
	if 2 = 3161
		dialog 1
		add_talk $_npc$:
		add_talk #b#b真正的强大不是来自你的四肢，而是你内心是否坚定，进入试炼场便没有回头路了，做好准备。
			option 202 #进入枪手试炼之台
			
		talk
	endif 
	if 2 = 3158
		dialog 1
		add_talk $_npc$:
		add_talk #b#b真正的强大不是来自你的四肢，而是你内心是否坚定，进入试炼场便没有回头路了，做好准备。
			option 203 #进入法师试炼之台
			
		talk
	endif 
endproc







