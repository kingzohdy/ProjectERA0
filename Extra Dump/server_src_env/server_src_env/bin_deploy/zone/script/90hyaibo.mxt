; 琳达


proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b谁说男人不能拥有人间凶器！只是我现在还能算是个男人咩？
	add_talk #b#b真希望有一天，我也能成为世界名人、联盟红人，这样我心爱的小甜甜就会主动投入我的怀抱！
	
		task_add_option 2132 1 100
		
		strvar Ret
		task_stat 2127 1 Ret
		if Ret = 2
			option 150 艾波先生，您吃了吗？
		endif
		
		strvar Ret1
		task_stat 2109 1 Ret1
		if Ret1 = 2
			option 180 请问您听说过啃脸男吗？

		endif
		
	talk
endproc

PROC 100
	task_do_option 2132 1
ENDPROC


proc 2
;	get_trig_task 1 2
;	if 2 = 2128
;		dialog 1
;		add_talk $_npc$:
;		add_talk #b#b
;			option 150 让我选吃哪种？
;		talk
;	endif
endproc

proc 150
	dialog 1
	add_talk $_npc$:
	add_talk #b#b嗨，$_player$，还没吃呢，你有什么事吗？

		option 151 琳达小姐让我来问问紫薯…

	talk

endproc 
proc 151
	dialog 1
	add_talk $_npc$:
	add_talk #b#b哦，这不刚刚煮好嘛，我正想给她送去几块呢，不过……

		option 152 不过什么？
	talk

endproc 
proc 152
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我煮出来的紫薯居然是蓝色的！整锅的水都是蓝色的，就像是倒进了一瓶蓝墨水一样！我现在正在纠结呢，到底是吃呢，还是吃呢，还是吃呢？

		option 153 这个……
	talk

endproc 
proc 153
	dialog 1
	add_talk $_npc$:
	add_talk #b#b哎，算了，不说这些了，说了也没用，来，拿好，带回去给琳达吧。

		option 154 #多谢
	talk

endproc 
proc 154
	ADD_ITEM_NUM_INPAK 5300903 1 1 0
	return
endproc 

proc 180
	dialog 1
	add_talk $_npc$:
	add_talk #b#b那哥们，真是太……彪悍了……，想我艾波，什么没吃过！可惜还真没吃过人肉，而且还是活生生的人！

		option 181 呃……

	talk
endproc 
proc 181
	dialog 1
	add_talk $_npc$:
	add_talk #b#b虽然大家都说这是谣传，但是我觉得这肯定是真事，你看看……
	add_talk #b#b<艾波从怀里拿出一本已经发黄甚至隐隐散发出体味的画报，无论男女都穿的非常凉爽>
	add_talk #b#b你看看这些图片，多么真实啊……
		option 182 您的画报能借我看看吗？

	talk
endproc 

proc 182
	dialog 1
	add_talk $_npc$:
	add_talk #b#b哎哟，还害羞了哟，这光天化日的，还怕他们从画里跳出来不成，若能真跳出来，我可爱的小甜甜，你可知道我做梦都想和你一起……

		option 183 #我会尽快归还的！

	talk
endproc 
proc 183
	
	ADD_ITEM_NUM_INPAK 5300896 1 1 0
endproc 