;伊米

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b两个人在一起，人家就要造谣言，正如两根树枝接近，蜘蛛就要挂网。
	
		task_add_option 1910 1 100
		task_add_option 1911 1 101
		task_add_option 1912 1 102
		task_add_option 1913 1 103
		task_add_option 1914 1 104
		task_add_option 1915 1 105
		task_add_option 1916 1 106
		task_add_option 1917 1 107
		task_add_option 1918 1 108
		strvar Ret
		task_stat 1919 0 Ret
		if Ret = 2
			option 130 询问伊米
		endif
		
		strvar Ret num1
		GET_ITEM_NUM_INPAK 5300933 1 num1
		if num1 > 0
			TASK_STAT 3305 1 Ret
			if Ret = 2
				option 700 曲别针换别墅
			endif	
		endif
	
	talk
endproc
proc 130
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我也不知道他在说啥，难道是想再来一瓶？不过看他的样子似乎不是酒鬼也不像智障儿童。
	add_talk #b#b我觉得他好像还有点迷糊，他的头一定还晕着。我看我还是留在这里照顾他吧。
	add_talk #b#b不如你替我跑一趟，把这些事情告诉姐姐，她或许有办法！
		task_fini 1919 1
		option 150 #我知道了
	talk 
endproc
proc 150
	return
endproc 

PROC 100
	task_do_option 1910 1
ENDPROC
PROC 101
	task_do_option 1911 1
ENDPROC
PROC 102
	task_do_option 1912 1
ENDPROC
PROC 103
	task_do_option 1913 1
ENDPROC
PROC 104
	task_do_option 1914 1
ENDPROC
PROC 105
	task_do_option 1915 1
ENDPROC
PROC 106
	task_do_option 1916 1
ENDPROC
PROC 107
	task_do_option 1917 1
ENDPROC
PROC 108
	task_do_option 1918 1
ENDPROC
;proc 2
;	get_trig_task 1 2
;	if 2 = 1910
;		task_fini 1910 1
;	endif
;
;endproc


proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b人是人他妈生的，妖是妖他妈生的，那人妖……啊，不是人妖，是妖人，啊哦~~什么是妖人？这真是个值得研究的课题。
		option 701 #交换

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300933 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300933 1 1
			
			ADD_ITEM_NUM_INPAK 5300931 1 0 0

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


