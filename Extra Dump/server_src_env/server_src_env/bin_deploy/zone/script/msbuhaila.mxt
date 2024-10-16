;布海拉



proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你好啊，$_player$，我们正需要你的帮助。
	
	
	task_add_option 2010 1 100
	task_add_option 2011 1 101
	task_add_option 2012 1 102
	task_add_option 2013 1 103
	task_add_option 2014 1 104
	task_add_option 2015 1 105
	task_add_option 2016 1 106
	task_add_option 2017 1 107
	strvar Ret Retstatus
	task_stat 2012 1 Ret	
	HAS_STATUS 1018 Retstatus
	if Ret = 2 and Retstatus = -1
		option 150 #请无影之镜赐给我力量
	endif
	strvar Ret1
	task_stat 2017 1 Ret1
	if Ret1 = 2
		option 155 #我不小心把车弄丢了，再给我一次机会吧
	endif
	
	talk
	
endproc 

PROC 100
	task_do_option 2010 1
ENDPROC
PROC 101
	task_do_option 2011 1
ENDPROC
PROC 102
	task_do_option 2012 1
ENDPROC
PROC 103
	task_do_option 2013 1
ENDPROC
PROC 104
	task_do_option 2014 1
ENDPROC
PROC 105
	task_do_option 2015 1
ENDPROC

PROC 106
	task_do_option 2016 1
ENDPROC

PROC 107
	task_do_option 2017 1
ENDPROC

proc 150

	GET_PLAYER_ID 1 Mid
	TARGET_ADD_STATUS 1 @Mid 1018 3 120 1

endproc 
proc 155
	strvar Mid
	GET_PLAYER_ID 1 Mid
	TARGET_ADD_STATUS 1 @Mid 1033 1 300000 1
	NEW_OWNER_TMP_ITEM 1460045 29100 23600 0 0 0 183 1095 1 1000 19		
endproc

proc 2
	
	get_trig_task 1 2 
;	if 2 = 2001
;		task_fini 2001 1
;	endif
	if 2 = 2012
		strvar Mid
		GET_PLAYER_ID 1 Mid
		TARGET_ADD_STATUS 1 @Mid 1018 3 120 1
	endif
	if 2 = 2017
		call 155
	endif
	
endproc

