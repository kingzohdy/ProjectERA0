;鲁莽

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b要成为我的主人可不是件容易的事，先拿出点让我信服的能耐来，我也会给主人展现让他满意的能力。
	add_talk #b#b我最擅长爆头，这可是出奇制胜的关键！你最拿手的又是什么？
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
	del_npc 352001 0
	call_mon 1 237001 0 0 6806 5935 0
	get_pworld_var 12 32 1 31
	var 31 237001
	set_pworld_var 12 32 @31 1
	GET_MAP_MON_ID 237001 51 0 
	mon_speech @51 0 1 0 先打败我再说！
	
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
