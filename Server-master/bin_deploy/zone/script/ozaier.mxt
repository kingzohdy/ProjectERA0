

;侯赛恩


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b即使战斗到只剩最后一口气，我也绝不让外面那些怪物再伤害任何一个人。
	

;20~30修改
	
	task_add_option 1406 1 133
	task_add_option 1407 1 134
	task_add_option 1408 1 135
;	task_add_option 1409 1 136
;	task_add_option 1410 1 137
	
;	task_add_option 1429 1 138
	task_add_option 1430 1 139
	task_add_option 1431 1 140
	task_add_option 1432 1 141
	task_add_option 1433 1 142
	task_add_option 1434 1 143
	task_add_option 1435 1 144
	task_add_option 1436 1 145
	task_add_option 1494 1 146
	task_add_option 1495 1 147
	
;	task_add_option 1497 1 148
;	task_add_option 1498 1 149
	task_add_option 1422 1 150
	
	task_add_option 1413 1 200


	talk
endproc 


PROC 133
	task_do_option 1406 1
ENDPROC
PROC 134
	task_do_option 1407 1
ENDPROC
PROC 135
	task_do_option 1408 1
ENDPROC
;PROC 136
;	task_do_option 1409 1
;ENDPROC
;PROC 137
;	task_do_option 1410 1
;ENDPROC

PROC 139
	task_do_option 1430 1
ENDPROC
PROC 140
	task_do_option 1431 1
ENDPROC
PROC 141
	task_do_option 1432 1
ENDPROC
PROC 142
	task_do_option 1433 1
ENDPROC
PROC 143
	task_do_option 1434 1
ENDPROC
PROC 144
	task_do_option 1435 1
ENDPROC
PROC 145
	task_do_option 1436 1
ENDPROC
PROC 146
	task_do_option 1494 1
ENDPROC
PROC 147
	task_do_option 1495 1
ENDPROC
;PROC 148
;	task_do_option 1497 1
;ENDPROC
;PROC 149
;	task_do_option 1498 1
;ENDPROC
PROC 150
	task_do_option 1422 1
ENDPROC
PROC 200
	task_do_option 1413 1
ENDPROC


proc 2
	get_trig_task 1 2

	if 2 = 1431
;		ADD_ITEM_NUM_INPAK 5300327 8 1 0
	ENDIF
	if 2 = 1432
;		ADD_ITEM_NUM_INPAK 5300328 8 1 0
	ENDIF
	if 2 = 1434
;		ADD_ITEM_NUM_INPAK 5300330 5 1 0
;		GET_PLAYER_ID 1 54
;		TARGET_ADD_STATUS 1 @54 1099 2 300 53
	ENDIF

	if 2 = 1413
	
		ADD_ITEM_NUM_INPAK 5300338 5 1 0
	endif
	if 2 = 1422
		task_fini 1422 1
	endif

endproc
