proc 1


		var 12 0
		set_task_var 3003 0 8 @12 1
		
		var 13 0
		set_task_var 3003 1 8 @13 1
		var 14 0
		set_task_var 3003 2 8 @14 1
		var 15 0
		set_task_var 3003 3 8 @15 1

		var 16 0
		set_task_var 3003 4 8 @16 1
endproc
proc 5
;放弃任务

		get_npc_num 352000 0 400
		get_npc_num 352001 0 401
		get_npc_num 352002 0 402
		get_npc_num 352003 0 403
		get_npc_num 352004 0 404
		get_npc_num 352005 0 405
		get_npc_num 352006 0 406
		if 400 > 0
			del_npc 352000 0
		endif
		if 401 > 0
			del_npc 352001 0
		endif
		if 402 > 0
			del_npc 352002 0
		endif
		if 403 > 0
			del_npc 352003 0
		endif
		if 404 > 0
			del_npc 352004 0
		endif
		if 405 > 0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
			del_npc 352005 0
		endif
		if 406 > 0
			del_npc 352006 0
		endif
		GET_MAP_MON_NUM 20200 293077 0 410 0
		GET_MAP_MON_NUM 20200 293078 0 411 0
		GET_MAP_MON_NUM 20200 293079 0 412 0
		GET_MAP_MON_NUM 20200 293080 0 413 0
		GET_MAP_MON_NUM 20200 293081 0 414 0
		GET_MAP_MON_NUM 20200 293082 0 415 0
		GET_MAP_MON_NUM 20200 293083 0 416 0
		if 410 > 0
			DELETE_MON_BY_DEFID 293077 0
		endif
		if 411 > 0
			DELETE_MON_BY_DEFID 293078 0
		endif
		if 412 > 0 
			DELETE_MON_BY_DEFID 293079 0
		endif
		if 413 > 0 
			DELETE_MON_BY_DEFID 293080 0
		endif
		if 414 > 0 
			DELETE_MON_BY_DEFID 293081 0
		endif
		if 415 > 0
			DELETE_MON_BY_DEFID 293082 0
		endif
		if 416 > 0
			DELETE_MON_BY_DEFID 293083 0
		endif
endproc
proc 6
	get_pworld_var 100 8 1 40
	if 40 = 10
		;5秒钟1次提示
		get_task_var 3003 4 8 1 16	
		add 16 1
		set_task_var 3003 4 8 @16 1
		if 16 = 5
			add_sys_msg_start
			add_sys_msg 对“可捕捉”的松鼠枪手使用任务追踪右侧的“捕兽夹”！
			set_sys_msg_mode 5 0 0
			msg_info
		endif
	endif

	strvar RetNum1 RetNum2 RetNum3 RetNum4 RetNum5 RetNum6 RetNum7
	GET_ITEM_NUM_INPAK 5610004 1 RetNum1
	GET_ITEM_NUM_INPAK 5610005 1 RetNum2
	GET_ITEM_NUM_INPAK 5610006 1 RetNum3
	GET_ITEM_NUM_INPAK 5610007 1 RetNum4
	GET_ITEM_NUM_INPAK 5610008 1 RetNum5
	GET_ITEM_NUM_INPAK 5610009 1 RetNum6
	GET_ITEM_NUM_INPAK 5610010 1 RetNum7
	if RetNum1 > 0 or RetNum2 > 0 or RetNum3 > 0 or RetNum4 > 0 or RetNum5 > 0 or RetNum6 > 0 or RetNum7 > 0
		task_fini 3003 0
		return
	endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		get_task_var 3003 1 8 1 13

		if 13 = 1
			get_task_var 3003 2 8 1 14
			add 14 1
			set_task_var 3003 2 8 @14 1
			if 14 = 3
				get_task_var 3003 3 8 1 15
				get_pworld_var 8 32 1 30
				
				switch 15
					del_npc 352000 1
					del_npc 352001 1
					del_npc 352002 1
					del_npc 352003 1
					del_npc 352004 1 
					del_npc 352005 1 
					del_npc 352006 1
					case 1
						effect 0 0 playEffct(6806,5935,"effect\\entity\\6100_18.ent")
					
						new_npc 0 352000 6806 5935 0 cwtiexin1.mac
						var 30 352000
						set_pworld_var 8 32 @30 1
						
						GET_MAP_NPC_ID 352000 50 0 
						mon_speech @50 0 1 0 无法想象，生活若缺少了想象力该是多么可怕！
						var 13 0
						set_task_var 3003 1 8 @13 1
						var 14 0
						set_task_var 3003 2 8 @14 1
						;;;;;;;;;;
						call 100
					endcase
					case 2
						effect 0 0 playEffct(6806,5935,"effect\\entity\\6100_18.ent")
						
						new_npc 0 352001 6806 5935 0 cwtiexin2.mac
						var 30 352001
						set_pworld_var 8 32 @30 1
						
						GET_MAP_NPC_ID 352001 51 0 
						mon_speech @51 0 1 0 无法想象，生活若缺少了想象力该是多么可怕！
						var 13 0
						set_task_var 3003 1 8 @13 1
						var 14 0
						set_task_var 3003 2 8 @14 1
						call 100
		
					endcase
					case 3
						
						effect 0 0 playEffct(6806,5935,"effect\\entity\\6100_18.ent")
						
						new_npc 0 352002 6806 5935 0 cwtiexin3.mac
						var 30 352002
						set_pworld_var 8 32 @30 1
						
						GET_MAP_NPC_ID 352002 52 0 
						mon_speech @52 0 1 0 无法想象，生活若缺少了想象力该是多么可怕！
						var 13 0
						set_task_var 3003 1 8 @13 1
						var 14 0
						set_task_var 3003 2 8 @14 1
					
						call 100
					endcase
					case 4
						effect 0 0 playEffct(6806,5935,"effect\\entity\\6100_18.ent")
					
						new_npc 0 352003 6806 5935 0 cwtiexin4.mac
						var 30 352003
						set_pworld_var 8 32 @30 1
						
						GET_MAP_NPC_ID 352003 53 0 
						mon_speech @53 0 1 0 无法想象，生活若缺少了想象力该是多么可怕！
						var 13 0
						set_task_var 3003 1 8 @13 1
						var 14 0
						set_task_var 3003 2 8 @14 1
						
						call 100
					endcase
					case 5
						effect 0 0 playEffct(6806,5935,"effect\\entity\\6100_18.ent")
					
						new_npc 0 352004 6806 5935 0 cwtiexin5.mac
						var 30 352004
						set_pworld_var 8 32 @30 1
						
						GET_MAP_NPC_ID 352004 54 0 
						mon_speech @54 0 1 0 无法想象，生活若缺少了想象力该是多么可怕！
						var 13 0
						set_task_var 3003 1 8 @13 1
						var 14 0
						set_task_var 3003 2 8 @14 1
					
						call 100
					endcase
					case 6
						effect 0 0 playEffct(6806,5935,"effect\\entity\\6100_18.ent")
						
						new_npc 0 352005 6806 5935 0 cwtiexin6.mac
						var 30 352005
						set_pworld_var 8 32 @30 1
						
						GET_MAP_NPC_ID 352005 55 0 
						mon_speech @55 0 1 0 无法想象，生活若缺少了想象力该是多么可怕！
						var 13 0
						set_task_var 3003 1 8 @13 1
						var 14 0
						set_task_var 3003 2 8 @14 1
					
						call 100
					endcase
					case 7  
						effect 0 0 playEffct(6806,5935,"effect\\entity\\6100_18.ent")
					
						new_npc 0 352006 6806 5935 0 cwtiexin7.mac
						var 30 352006
						set_pworld_var 8 32 @30 1
						
						GET_MAP_NPC_ID 352006 56 0 
						mon_speech @56 0 1 0 无法想象，生活若缺少了想象力该是多么可怕！
						var 13 0
						set_task_var 3003 1 8 @13 1
						var 14 0
						set_task_var 3003 2 8 @14 1
					
						call 100
					endcase
					case 8
						effect 0 0 playEffct(6806,5935,"effect\\entity\\6100_18.ent")
						
						new_npc 0 352002 6806 5935 0 cwtiexin3.mac
						var 30 352002
						set_pworld_var 8 32 @30 1
						
						GET_MAP_NPC_ID 352002 57 0 
						mon_speech @57 0 1 0 无法想象，生活若缺少了想象力该是多么可怕！
						var 13 0
						set_task_var 3003 1 8 @13 1
						var 14 0
						set_task_var 3003 2 8 @14 1
					
						call 100
					endcase

				endswitch

			endif
	
		endif

endproc 

proc 100
	GET_MAP_NPC_ID 317009 60 0 
	mon_speech @60 0 1 0 与“松鼠枪手”对话，开启挑战！
	add_sys_msg_start
	add_sys_msg 与“松鼠枪手”对话，开启挑战！
	set_sys_msg_mode 3 0 0
	msg_info

endproc 