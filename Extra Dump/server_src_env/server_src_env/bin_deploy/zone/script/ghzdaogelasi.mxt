

proc 1
	DIALOG 1

	add_talk $_npc$:
	add_talk #b#b最好的防御就是进攻！城防建设的关键之处就是增加城内的进攻能力，以我浸淫此道数十年的经验，搞定这里实在是太轻松了。信我的没错的。
	
	strvar Ret
	IS_WORLD_CITY_OWNER Ret
	if Ret = 1
		option 200 #城防建设
		option 202 #回时间城
	endif
	option 201 #关闭
	
	talk
endproc 
proc 200
	open_ui 59
endproc
proc 201
	return
endproc
proc 202
	TRANSMIT 1000 26600	28100 0

endproc
