

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk  #b#b应该有更好的方式开始新的一天，而不是千篇一律的在每个上午都醒来。应该有更好的方式开始新的生活，也许我应该好好考虑到时间城里去摆个小摊……


	TASK_STAT 12058 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (环次)学院长让我来了解一下情况
	endif

	TASK_STAT 12123 1 20
	if 1 = 0 and 20 = 2
	  	option 151 (环次)这是你要的武器
	endif
	TASK_STAT 12124 1 20
	if 1 = 0 and 20 = 2
	  	option 152 (环次)这是你要的武器
	endif
	TASK_STAT 12125 1 20
	if 1 = 0 and 20 = 2
	  	option 153 (环次)这是你要的武器
	endif
	TASK_STAT 12126 1 20
	if 1 = 0 and 20 = 2
	  	option 154 (环次)这是你要的武器
	endif

	TASK_STAT 12258 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (环次)这是炼金需要的一些材料，你愿意帮忙收集一些么？
	endif

	TASK_STAT 12323 1 20
	if 1 = 0 and 20 = 2
	  	option 161 (环次)这些物资送给你，试一下能不能找回些什么？
	endif
	TASK_STAT 12324 1 20
	if 1 = 0 and 20 = 2
	  	option 162 (环次)这些物资送给你，试一下能不能找回些什么？
	endif
	TASK_STAT 12325 1 20
	if 1 = 0 and 20 = 2
	  	option 163 (环次)这些物资送给你，试一下能不能找回些什么？
	endif
	TASK_STAT 12326 1 20
	if 1 = 0 and 20 = 2
	  	option 164 (环次)这些物资送给你，试一下能不能找回些什么？
	endif
	OPTION 110 #(买卖)打开商店
	talk
endproc 
PROC 110
	OPEN_NPC_SHOP
ENDPROC
PROC 150
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b灾难已经降临到这片土地，恐怖的事情一件接着一件发生。

	OPTION 250 #(环次)知道了！
	TALK
ENDPROC
PROC 250
	GET_ITEM_NUM_INPAK 5300509 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300509 1 0
	task_fini 12058 1
	endif
ENDPROC

PROC 151

    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b非常感谢你带来的这些物资，我想我们又可以继续坚持下去了。

	OPTION 251 #(环次)不用客气！
	TALK
	
ENDPROC
PROC 251
	GET_ITEM_NUM_INPAK 1019906 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 1019906 1 0
	task_fini 12123 1
	endif
ENDPROC

PROC 152

    	DIALOG 1
   	ADD_TALK $_npc$:      
    	ADD_TALK  #b#b非常感谢你带来的这些物资，我想我们又可以继续坚持下去了。

	OPTION 252 #(环次)不用客气！
	TALK
	
ENDPROC
PROC 252
	GET_ITEM_NUM_INPAK 1109906 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 1109906 1 0
	task_fini 12124 1
	endif
ENDPROC


PROC 153

    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b非常感谢你带来的这些物资，我想我们又可以继续坚持下去了。

	OPTION 253 #(环次)不用客气！
	TALK
	
ENDPROC
PROC 253
	GET_ITEM_NUM_INPAK 1209906 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 1209906 1 0
	task_fini 12125 1
	endif
ENDPROC

PROC 154

    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b非常感谢你带来的这些物资，我想我们又可以继续坚持下去了。

	OPTION 254 #(环次)不用客气！
	TALK
	
ENDPROC
PROC 254
	GET_ITEM_NUM_INPAK 1509906 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 1509906 1 0
	task_fini 12126 1
	endif
ENDPROC

PROC 160
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b在危险的丛林之中游走，一直都不会去注意有什么东西，但是如果是炼金术师要求的话，我想我会考虑一下。

	OPTION 260 #(环次)谢谢你了！
	TALK
ENDPROC
PROC 260
	GET_ITEM_NUM_INPAK 5300519 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300519 1 0
	task_fini 12258 1
	endif
ENDPROC

PROC 161

    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK #b#b谢谢你送来的物资，我想这些东西足够我们研究很久。

	OPTION 261 #(环次)谢谢你了！
	TALK
	
ENDPROC

PROC 261
	GET_ITEM_NUM_INPAK 2019906 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 2019906 1 0
	task_fini 12323 1
	endif
ENDPROC

PROC 162

    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK #b#b谢谢你送来的物资，我想这些东西足够我们研究很久。

	OPTION 262 #(环次)谢谢你了！
	TALK
	
ENDPROC
PROC 262
	GET_ITEM_NUM_INPAK 2029906 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 2029906 1 0
	task_fini 12324 1
	endif
ENDPROC

PROC 163

    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK #b#b谢谢你送来的物资，我想这些东西足够我们研究很久。

	OPTION 263 #(环次)谢谢你了！
	TALK
	
ENDPROC
PROC 263 
	GET_ITEM_NUM_INPAK 2039906 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 2039906 1 0
	task_fini 12325 1
	endif
ENDPROC

PROC 164

    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK #b#b谢谢你送来的物资，我想这些东西足够我们研究很久。

	OPTION 264 #(环次)谢谢你了！
	TALK
	
ENDPROC
PROC 264
	GET_ITEM_NUM_INPAK 2049906 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 2049906 1 0
	task_fini 12326 1
	endif
ENDPROC
