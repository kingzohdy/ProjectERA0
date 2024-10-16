;;;;;;;;;;;隐秘机关

proc 1

	strvar Ret
	task_stat 1583 1 Ret
	if Ret = 2
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b#c676464<这个神秘的东西看起来像一个机关，你试着寻找一些破解机关的方式，发现其中有个手柄可以旋转>#n	
			option 151 左转
			option 300 #右转
		talk
	endif
endproc 
proc 300
	add_sys_msg_start
	add_sys_msg 机关没有丝毫变化，看来是打开的方式不对！	
	set_sys_msg_mode 3 0 0
	msg_info
endproc 
proc 151
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c676464<你听到了轻微的巨石转动的声响，虽然转瞬即逝，但你确实听到了>#n
		option 152 左转
		option 301 #右转
	talk
endproc
proc 301
	add_sys_msg_start
	add_sys_msg 机关恢复了原貌，看来是打开的方式不对！	
	set_sys_msg_mode 3 0 0
	msg_info
endproc
proc 152
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c676464<机关里传出“卡”的两声响>#n
		option 302 #左转
		option 153 右转
	talk

endproc
proc 302
	add_sys_msg_start
	add_sys_msg 机关恢复了原貌，看来是打开的方式不对！		
	set_sys_msg_mode 3 0 0
	msg_info
endproc 
proc 153
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c676464<又是一阵细微的声响传出，仿佛是巨大的齿轮转动的沉重>#n
		option 302 #左转
		option 154 #右转
	talk
endproc

proc 154
;	task_fini 1583 1
	add_item_num_inpak 5300807 1 1 0
endproc











