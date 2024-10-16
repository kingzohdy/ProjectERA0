;;;;;特大木箱

proc 1

	strvar Ret
	task_stat 1363 1 Ret
	if Ret = 1 or Ret = 3 or Ret = 4
		return
	endif
	get_player_mapinfo 0 1 2 3 4
;	41374 38055
	if 2 = 2202
			dec_item_num_inpak 5300854 1 0
;			GET_MON_INFO 10 11 12 13 14 15 16 17 
;			effect 0 0 playEffct($@16$,$@17$,"effect\\entity\\9546_16.ent")	
	;	NEW_OWNER_TMP_ITEM 1460001 0 0 0 0 0 0 0 0 0 0 1
		APPLY_TMP_ITEM 1460076 0 1
		;;;给自己加状态
	;	GET_PLAYER_ID 1 60
	;	TARGET_ADD_STATUS 1 @60 id lv time 1
		
		;;记录两只怪物ID		
;		strvar mid1 mid2
;		NEW_LIFE_MON 304075 1 41374 38055 0 0 0 0 30 0 30
;		var mid1 @30
;		set_task_var 1363 4 32 @mid1 1
		
;		NEW_LIFE_MON 304076 1 41300 37700 0 0 0 0 30 0 40
;		var mid2 @40
;		set_task_var 1363 8 32 @mid2 1
		strvar Ret mid1 mid2
		GET_TEAM_PLAYER_ID Ret 1 1 1 1 1 1 1 1 
		if Ret = 0
			NEW_NPC_PRIVATE mid1 1 304075 30 42400 37500 0 npc.mac 
			NEW_NPC_PRIVATE mid2 1 304076 30 42300 37200 90 npc.mac 	
		else	
			NEW_NPC_PRIVATE mid1 0 304075 30 42400 37500 0 npc.mac 
			NEW_NPC_PRIVATE mid2 0 304076 30 42300 37200 90 npc.mac 
		endif            
		
		get_task_var 1363 4 32 1 10
		var 10 @mid1 
		set_task_var 1363 4 32 @10 1
		
		get_task_var 1363 8 32 1 20
		var 20 @mid2 
		set_task_var 1363 8 32 @20 1
		
		strvar mid
		get_player_id 1 mid
		TARGET_ADD_STATUS 1 @mid 3099  1 34 1
		
		
		;;锁		
		strvar val
		get_task_var 1363 0 8 1 val
		var val 1
		set_task_var 1363 0 8 @val 1

	  else
			add_sys_msg_start
			add_sys_msg 此物品只能在冰岛使用！
			set_sys_msg_mode 3 0 0
			msg_info
	  endif

endproc
