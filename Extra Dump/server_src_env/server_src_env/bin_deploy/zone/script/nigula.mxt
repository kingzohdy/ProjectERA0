
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b有什么可以帮你的吗。。。。。

	
	
;	OPTION 110 #(买卖)打开商店 

	TASK_STAT 12012 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (环次)东西送来了，学院长希望得到你的帮助
	endif

	TASK_STAT 12212 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (环次)这是带给你的地图
	endif


	talk
endproc 

proc 110
    OPEN_NPC_SHOP
endproc


PROC 150	
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b看上去如果我们都一起努力的话，应该可以找到变异人的消息。好吧，告诉学院长，我知道了。

	OPTION 250 #(环次)好的！
	TALK
ENDPROC
proc 250
	GET_ITEM_NUM_INPAK 5300501 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300501 1 0
    	task_fini 12012 1
	endif
endproc

PROC 160
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b商路的拓展，如果可以的话，我希望时间城的各位都能够享受到它带来的好处。

	OPTION 260 #(环次)谢谢你的支持！
	TALK
ENDPROC
proc 260
	GET_ITEM_NUM_INPAK 5300511 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg 你身上没有这件物品
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300511 1 0
    	task_fini 12212 1
	ENDIF

endproc
