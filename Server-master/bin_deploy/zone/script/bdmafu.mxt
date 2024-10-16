


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b哎呀呀，大名鼎鼎的$_player$啊，你是来挑选如花美眷……啊，不对，是宝马良驹的么？
	add_talk #b#b您来我这真是来对了！
	add_talk #b#b您瞧瞧，您瞧瞧，这可都是一等一的千里宝马啊，我这些宝马，价格便宜，最便宜的只要500，而且还有3种颜色可选，世上还有比这更便宜的事吗？

;		task_add_option 1351 1 100
		option 150 #租赁坐骑
	talk
	
endproc 
proc 150
	OPEN_NPC_SHOP 
	strvar ret
	task_stat 1377 1 ret
	if ret = 2
		task_fini 1377 1
	endif
endproc
;PROC 100
;	task_do_option 1351 1
;ENDPROC
;
;proc 2
;	get_trig_task 1 2
;	if 2 = 1355
;		call 150
;		NEW_OWNER_TMP_ITEM 1460060 9800 8800 0 0 0 184 1095 1 3600 1
;	endif
;endproc 
	
	