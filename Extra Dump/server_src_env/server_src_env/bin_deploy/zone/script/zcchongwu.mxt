

proc 1
		open_ui 73
;	DIALOG 1
;	add_talk $_npc$:
;	add_talk #b#b哪儿的宠物我都有！你说我没有那个？呸，是价钱太贵，你呀！买不起！我就不拿出来让你烧心了！

; 	task_add_option 2026 1 103
 ;	task_add_option 2027 1 104
 ;	task_add_option 4213 1 105

;50级

 ;	task_add_option 2017 1 117
;	task_add_option 3010 1 121
	strvar Ret num1
;	GET_ITEM_NUM_INPAK 5300769 1 num1
;	if num1 > 0
;		TASK_STAT 3304 1 Ret
;		if Ret = 2
;			option 500 曲别针换别墅
;		endif
;	endif


;	OPTION 149 #宠物整合
;	OPTION 170 #宠物幻化
;	OPTION 171 #宠物强化
	
	
	
;	OPTION 172 #宠物增强
;	OPTION 173 #宠物技能学习
;	option 178 #宠物技能重置
;	OPTION 174 #宠物技能升级
;	OPTION 175 #宠物技能格扩充
;	OPTION 176 #宠物技能格改造

;	talk
endproc 

PROC 103
	task_do_option 2026 1
ENDPROC
PROC 104 
	task_do_option 2027 1
ENDPROC
PROC 105
	task_do_option 4213 1
ENDPROC

PROC 149
	
;	TASK_STAT 3009 1 22
;	if 22 = 2
;		task_fini 3009 1
;	endif

;	OPEN_NPC_SHOP
	open_ui 73
ENDPROC

PROC 117
	task_do_option 2017 1
ENDPROC

;proc 121
;	task_do_option 3010 1
;endproc



PROC 170
	open_ui 27
ENDPROC

PROC 171
	open_ui 28
ENDPROC

PROC 172
	open_ui 29
ENDPROC

PROC 173
	open_ui 30
ENDPROC

PROC 174
	open_ui 31
ENDPROC

PROC 175
	open_ui 32
ENDPROC

PROC 176
	open_ui 33
ENDPROC
proc 178
	open_ui 65
endproc



proc 2
	get_trig_task 1 2

	if 2 = 2027 
		task_fini 2027 1
	endif  

	if 2 = 4324 
		task_fini 4324 1
	endif  
	if 2 = 2017
		task_fini 2017 1
	endif 

	if 2 = 1031
		task_fini 1031 1
	endif 

endproc

proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b如果你现在不和我换，以后不要再来找我…

	option 501 #交换

	talk
endproc
	
proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300769 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300769 1 1
			
			rand Rad 100
			if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300770 1 0 0
			endif
			if @Rad >= 80 and @Rad < 95
				ADD_ITEM_NUM_INPAK 5300771 1 0 0
			endif
			if @Rad >= 95 and @Rad < 99
				ADD_ITEM_NUM_INPAK 5300772 1 0 0
			endif
			if @Rad = 99
				ADD_ITEM_NUM_INPAK 5300773 1 0 0
			endif
		else
			add_sys_msg_start
			add_sys_msg 背包空间不足！
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你身上没有可以交换的物品！
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc 
	



