; 格姆夏


proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b不想当将军的厨子就不是好裁缝！
	add_talk #b#b泥马，凭什么麦克尔一句话，我就要豁出性命？凭什么当官的就能吹着小风、喝着小酒、吃着小肉，而我活该被撕成碎片？
	add_talk #b#b泥马，有了功劳是他们的，犯了错是我的，老子凭什么任他们如此践踏，老子也是条铁铮铮的汉子！

	task_add_option 2115 1 100
		task_add_option 2116 1 101
		task_add_option 2117 1 102
		task_add_option 2118 1 103
		
		strvar Ret
		task_stat 2117 1 Ret
		if Ret = 2
			option 150 #打败格姆夏

		endif
	talk
endproc

PROC 100
	task_do_option 2115 1
ENDPROC
PROC 101
	task_do_option 2116 1
ENDPROC
PROC 102
	task_do_option 2117 1
ENDPROC
PROC 103
	task_do_option 2118 1
ENDPROC


proc 2
	get_trig_task 1 2
	if 2 = 2117
		dialog 1
		add_talk $_npc$:
		add_talk #b#b不想当将军的厨子就不是好裁缝！
		add_talk #b#b泥马，凭什么麦克尔一句话，我就要豁出性命？凭什么当官的就能吹着小风、喝着小酒、吃着小肉，而我活该被撕成碎片？
		add_talk #b#b泥马，有了功劳是他们的，犯了错是我的，老子凭什么任他们如此践踏，老子也是条铁铮铮的汉子！
		
			option 150 #打败格姆夏
		talk
	endif
	if 2 = 2118
		task_fini 2118 1
	endif

endproc
proc 150
	strvar x y
	GET_PLAYER_MAPINFO 0 0 0 x y
	CALL_MON 1 254021 500 120 @x @y 1
	return
endproc 





