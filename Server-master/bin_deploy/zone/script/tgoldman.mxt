proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b其实从他们身上也能看到我们自己的影子！在我们的眼睛里，他们是敌人，是毁灭我们和平生活的祸首；在他们的眼睛里，我们是敌人，是觊觎他们财富和武力的窃贼……
	add_talk #b#b我曾试着和他们平和相处，我成功了，或者说我曾经成功了！至少在神源出现之前，如此。
;	task_add_option 1312 1 101
	task_add_option 1313 1 102
	task_add_option 1314 1 103
	task_add_option 1315 1 104
	task_add_option 1316 1 105
;	task_add_option 1317 1 106
	task_add_option 1318 1 107
	task_add_option 1319 1 108
	task_add_option 1323 1 109
	task_add_option 1324 1 110
	
	
	task_add_option 1322 1 111
	task_add_option 1337 1 112
	task_add_option 1335 1 113
	
	strvar Ret
	task_stat 1319 1 Ret
	if Ret = 2
;		option 300 是否需要我送您一程？
	endif
	

	talk
endproc

;proc 101
;    task_do_option 1312 1
;endproc

proc 102
    task_do_option 1313 1
endproc
proc 103
    task_do_option 1314 1
endproc
proc 104
    task_do_option 1315 1
endproc
proc 105
    task_do_option 1316 1
endproc

proc 107
    task_do_option 1318 1
endproc

proc 108
    task_do_option 1319 1
endproc
proc 109
    task_do_option 1323 1
endproc

proc 110
    task_do_option 1324 1
endproc
proc 111
    task_do_option 1322 1
endproc
proc 112
    task_do_option 1337 1
endproc
proc 113
    task_do_option 1335 1
endproc
proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我可以直接将您送到目的地，但是中途不会停哟！
	
		option 301 #请直接将我送到目的地吧！
		option 302 #不用啦，我自己过去！
	talk

endproc 
proc 301
	
	NEW_OWNER_TMP_ITEM 1460062 9700 16500 0 0 0 303 1095 1 3600 1
endproc 
proc 302
	return
endproc 

proc 2
	get_trig_task 1 600
	if 600 = 1324 
		task_fini 1324 1
	endif

	if 600 = 1319
;		dialog 1
;		add_talk $_npc$:
;		add_talk #b#b小声点，附近都是凶狠的卡卡儿魔鬼。
		task_fini 1319 0
;		option 300 是否需要我送您一程？
;		talk
	endif
	if 600 = 1316
	
			ADD_ITEM_NUM_INPAK 5300385 1 1  0
	;		ADD_ITEM_NUM_INPAK 5300386 1 1 0
	endif

;	if 600 = 1060
;		GET_PLAYER_ID 1 54
;		GET_PLAYER_BASE_ATT 1 50 51 52
;
;		ADD_SYS_MSG_START
;		ADD_SYS_MSG 玩家ID=$@54$ 性别=$@50$ 
;		SET_SYS_MSG_MODE 1 0 0
;		MSG_INFO
;
;		IF 50 = 1
;			TARGET_ADD_STATUS 1 @54 1088 1 300 53
;		ENDIF
;		IF 50 = 2
;			TARGET_ADD_STATUS 1 @54 1088 2 300 53
;		ENDIF
;	endif


endproc
