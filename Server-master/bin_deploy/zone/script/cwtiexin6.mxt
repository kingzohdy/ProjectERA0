;英勇

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我在这里等候了很久，终于等到一个看起来稍微有点顺眼的家伙了，怎么样，要交个朋友吗？我是说那种愿意为你上刀山下油锅的铁哥们！

	option 100 我要挑战

	talk
endproc

proc 100
	dialog 1
	add_talk $_npc$:
	add_talk #b#b虽然你很对我胃口，但是，我们松鼠猎手一族只臣服于强者。如果你能展现出足够让我信服的能力，我自然也会对你一生一世忠诚。
	add_talk #b#b但如果你不能战胜我，那么，很抱歉，我没办法容忍一名比自己弱的主人。
	add_talk #b#b$_player$，你准备好接受挑战了吗？
	
	option 110 #开始挑战
	option 111 #放弃挑战

	talk

endproc 

proc 110
	del_npc 352005 0
	call_mon 1 237005 0 0 6806 5935 0
	get_pworld_var 12 32 1 31
	var 31 237005
	set_pworld_var 12 32 @31 1
	GET_MAP_MON_ID 237005 55 0 
	mon_speech @55 0 1 0 先打败我再说！
	
	add_sys_msg_start
	add_sys_msg 打败“松鼠枪手”！
	set_sys_msg_mode 3 0 0
	msg_info
			
	strvar Num
	GET_ITEM_NUM_INPAK 5233705 1 Num
	if Num = 0
		ADD_ITEM_NUM_INPAK 5233705 1 1 0	
	endif
	return	

endproc

proc 111
	return
endproc
