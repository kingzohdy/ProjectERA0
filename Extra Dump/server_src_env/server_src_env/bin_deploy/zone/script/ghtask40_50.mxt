;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;
proc 130
	add_sys_msg_start
	add_sys_msg 成功上交物品，任务完成
	set_sys_msg_mode 1 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc 

proc 100
	dialog 3
	add_talk $_npc$:
	add_talk #b#b收集回来的东西交给我先审视.
	add_talk #b#b

	task_stat 3241 1 101
	task_stat 3242 1 102
	task_stat 3243 1 103
	task_stat 3244 1 104
	task_stat 3245 1 105
	task_stat 3246 1 106
	task_stat 3247 1 107
	task_stat 3248 1 108
	task_stat 3249 1 109
	task_stat 3250 1 110
			        
	if 101 = 2
		add_talk #b#b#c186506需要收集41级~50级阶段的绿色品质主手物品#n
	endif
	if 102 = 2
		add_talk #b#b#c186506需要收集41级~50级阶段的绿色品质副手物品#n
	endif
	if 103 = 2
		add_talk #b#b#c186506需要收集41级~50级阶段的绿色品质头盔#n
	endif
	if 104 = 2
		add_talk #b#b#c186506需要收集41级~50级阶段的绿色品质肩甲#n
	endif
	if 105 = 2
		add_talk #b#b#c186506需要收集41级~50级阶段的绿色品质面饰#n
	endif
	if 106 = 2
		add_talk #b#b#c186506需要收集41级~50级阶段的绿色品质衣服#n
	endif	
	if 107 = 2
		add_talk #b#b#c186506需要收集41级~50级阶段的绿色品质战靴#n
	endif
	if 108 = 2
		add_talk #b#b#c186506需要收集41级~50级阶段的绿色品质项链#n
	endif
	if 109 = 2
		add_talk #b#b#c186506需要收集41级~50级阶段的绿色品质戒指#n
	endif
	if 110 = 2
		add_talk #b#b#c186506需要收集41级~50级阶段的绿色品质护符#n
	endif
	
	add_talk #b#b
	add_talk #b#b#c186506请把任务需要上交的物品放到下方方槽处。#n
	add_talk #b#b#c186506<右键点击背包内物品也可直接放入>#n
	
		NPC_COMMIT_OPT 137
	talk
endproc 
proc 111

	GET_COMMIT_ITEM_INFO 1 112
	;ID
	GET_COMMIT_ITEM_INFO 2 113
	;位置
	GET_COMMIT_ITEM_INFO 3 114
	;格子
	
	if 112 = -1
		add_sys_msg_start
		add_sys_msg 你还没有提交物品！
		set_sys_msg_mode 1 0 0
		msg_info		
		return 
	endif
	task_stat 3241 1 116
	if 116 = 2
		if 112 = 1013005 or 112 = 1253005 or 112 = 1203005 or 112 = 1103005 or 112 = 1033005 or 112 = 1503005
			switch 112
				case 1013005
					DEC_ITEM_NUM_BYIDX 1013005 1 @113 @114 1
					task_fini 3241 1
					call 130
				endcase
				case 1253005
					DEC_ITEM_NUM_BYIDX 1253005 1 @113 @114 1
					task_fini 3241 1
					call 130
				endcase
				case 1203005
					DEC_ITEM_NUM_BYIDX 1203005 1 @113 @114 1
					task_fini 3241 1
					call 130
				endcase
				case 1103005
					DEC_ITEM_NUM_BYIDX 1103005 1 @113 @114 1
					task_fini 3241 1
					call 130
				endcase
				case 1033005
					DEC_ITEM_NUM_BYIDX 1033005 1 @113 @114 1
					task_fini 3241 1
					call 130
				endcase
				case 1503005
					DEC_ITEM_NUM_BYIDX 1503005 1 @113 @114 1
					task_fini 3241 1
					call 130
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg 物品不符任务要求，未能成功上交
			set_sys_msg_mode 1 0 0
			msg_info
			
		endif
		
	endif
;;;;;;;;;		;;;;;;;;
	task_stat 3242 1 117
	if 117 = 2
		if 112 = 1583005 or 112 = 1283005 or 112 = 1183005 or 112 = 1063005 
			switch 112
				case 1583005
					DEC_ITEM_NUM_BYIDX 1583005 1 @113 @114 1
					task_fini 3242 1             
					call 130                     
				endcase                          
				case 1283005                     
					DEC_ITEM_NUM_BYIDX 1283005 1 @113 @114 1
					task_fini 3242 1             
					call 130                     
				endcase                          
				case 1183005                     
					DEC_ITEM_NUM_BYIDX 1183005 1 @113 @114 1
					task_fini 3242 1             
					call 130                     
				endcase                          
				case 1063005                     
					DEC_ITEM_NUM_BYIDX 1063005 1 @113 @114 1
					task_fini 3242 1            
					call 130
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg 物品不符任务要求，未能成功上交
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif
;;;;;;;;;		;;;;;;;;
	task_stat 3243 1 118
	if 118 = 2
		if 112 = 2133005 or 112 = 2143005 or 112 = 2123005 or 112 = 2113005 
			switch 112
				case 2133005
					DEC_ITEM_NUM_BYIDX 2133004 1 @113 @114 1
					task_fini 3243 1             
					call 130                     
				endcase                          
				case 2143005                     
					DEC_ITEM_NUM_BYIDX 2143004 1 @113 @114 1
					task_fini 3243 1             
					call 130                     
				endcase                          
				case 2123005                     
					DEC_ITEM_NUM_BYIDX 2123004 1 @113 @114 1
					task_fini 3243 1             
					call 130                     
				endcase                          
				case 2113005                     
					DEC_ITEM_NUM_BYIDX 2113004 1 @113 @114 1
					task_fini 3243 1            
					call 130
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg 物品不符任务要求，未能成功上交
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif
;;;;;;;;;		;;;;;;;;
	task_stat 3244 1 119
	if 119 = 2
		if 112 = 2243005 or 112 = 2223005 or 112 = 2213005 or 112 = 1273005 
			switch 112
				case 2243005
					DEC_ITEM_NUM_BYIDX 2243005 1 @113 @114 1
					task_fini 3244 1             
					call 130                     
				endcase                          
				case 2223005                     
					DEC_ITEM_NUM_BYIDX 2223005 1 @113 @114 1
					task_fini 3244 1             
					call 130                     
				endcase                          
				case 2213005                     
					DEC_ITEM_NUM_BYIDX 2213005 1 @113 @114 1
					task_fini 3244 1             
					call 130                     
				endcase                          
				case 1273005                     
					DEC_ITEM_NUM_BYIDX 1273005 1 @113 @114 1
					task_fini 3244 1            
					call 130
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg 物品不符任务要求，未能成功上交
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif
;;;;;;;;;		;;;;;;;;
	task_stat 3245 1 120
	if 120 = 2
		if 112 = 2423005 or 112 = 2433005 or 112 = 2443005 or 112 = 2413005 
			switch 112
				case 2423005
					DEC_ITEM_NUM_BYIDX 2423005 1 @113 @114 1
					task_fini 3245 1             
					call 130                     
				endcase                          
				case 2433005                     
					DEC_ITEM_NUM_BYIDX 2433005 1 @113 @114 1
					task_fini 3245 1             
					call 130                     
				endcase                          
				case 2443005                     
					DEC_ITEM_NUM_BYIDX 2443005 1 @113 @114 1
					task_fini 3245 1             
					call 130                     
				endcase                          
				case 2413005                     
					DEC_ITEM_NUM_BYIDX 2413005 1 @113 @114 1
					task_fini 3245 1            
					call 130
				endcase
			
			endswitch
		else
			add_sys_msg_start
			add_sys_msg 物品不符任务要求，未能成功上交
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif
;;;;;;;;;		;;;;;;;;
	task_stat 3246 1 121
	if 121 = 2
		if 112 = 2013005 or 112 = 2023005 or 112 = 2033005 or 112 = 2043005 
			switch 112
				case 2013005
					DEC_ITEM_NUM_BYIDX 2013005 1 @113 @114 1
					task_fini 3246 1             
					call 130                     
				endcase                          
				case 2023005                     
					DEC_ITEM_NUM_BYIDX 2023005 1 @113 @114 1
					task_fini 3246 1             
					call 130                     
				endcase                          
				case 2033005                     
					DEC_ITEM_NUM_BYIDX 2033005 1 @113 @114 1
					task_fini 3246 1             
					call 130                     
				endcase                          
				case 2043005                     
					DEC_ITEM_NUM_BYIDX 2043005 1 @113 @114 1
					task_fini 3246 1            
					call 130
				endcase
			
			endswitch
		else
			add_sys_msg_start
			add_sys_msg 物品不符任务要求，未能成功上交
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif
;;;;;;;;;		;;;;;;;;
	task_stat 3247 1 123
	if 123 = 2
		if 112 = 2613005 or 112 = 2633005 or 112 = 2643005 or 112 = 2623005 
			switch 112
				case 2613005
					DEC_ITEM_NUM_BYIDX 2613005 1 @113 @114 1
					task_fini 3247 1             
					call 130                     
				endcase                          
				case 2633005                     
					DEC_ITEM_NUM_BYIDX 2633005 1 @113 @114 1
					task_fini 3247 1             
					call 130                     
				endcase                          
				case 2643005                     
					DEC_ITEM_NUM_BYIDX 2643005 1 @113 @114 1
					task_fini 3247 1             
					call 130                     
				endcase                          
				case 2623005                     
					DEC_ITEM_NUM_BYIDX 2623005 1 @113 @114 1
					task_fini 3247 1            
					call 130
				endcase
			
			endswitch
		else
			add_sys_msg_start
			add_sys_msg 物品不符任务要求，未能成功上交
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif
;;;;;;;;;		;;;;;;;;
	task_stat 3248 1 124
	if 124 = 2
	
		if 112 = 2813005 or 112 = 2833005 or 112 = 2843005 or 112 = 2823005 
			switch 112
				case 2813005
					DEC_ITEM_NUM_BYIDX 2813005 1 @113 @114 1
					task_fini 3248 1             
					call 130                     
				endcase                          
				case 2833005                     
					DEC_ITEM_NUM_BYIDX 2833005 1 @113 @114 1
					task_fini 3248 1             
					call 130                     
				endcase                          
				case 2843005                     
					DEC_ITEM_NUM_BYIDX 2843005 1 @113 @114 1
					task_fini 3248 1             
					call 130                     
				endcase                          
				case 2823005                     
					DEC_ITEM_NUM_BYIDX 2823005 1 @113 @114 1
					task_fini 3248 1            
					call 130
				endcase
			
			endswitch
		else
			add_sys_msg_start
			add_sys_msg 物品不符任务要求，未能成功上交
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif
;;;;;;;;;		;;;;;;;;
	task_stat 3249 1 125
	if 125 = 2
		if 112 = 2933005 or 112 = 2923005 or 112 = 2913005 or 112 = 2943005 
			switch 112
				case 2933005
					DEC_ITEM_NUM_BYIDX 2933005 1 @113 @114 1
					task_fini 3249 1             
					call 130                     
				endcase                          
				case 2923005                     
					DEC_ITEM_NUM_BYIDX 2923005 1 @113 @114 1
					task_fini 3249 1             
					call 130                     
				endcase                          
				case 2913005                     
					DEC_ITEM_NUM_BYIDX 2913005 1 @113 @114 1
					task_fini 3249 1             
					call 130                     
				endcase                          
				case 2943005                     
					DEC_ITEM_NUM_BYIDX 2943005 1 @113 @114 1
					task_fini 3249 1            
					call 130
				endcase
			
			endswitch
		else
			add_sys_msg_start
			add_sys_msg 物品不符任务要求，未能成功上交
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif
;;;;;;;;;		;;;;;;;;
	task_stat 3250 1 126
	if 126 = 2
		if 112 = 2713005 or 112 = 2723005 or 112 = 2733005 or 112 = 2743005 
			switch 112
				case 2713005
					DEC_ITEM_NUM_BYIDX 2713005 1 @113 @114 1
					task_fini 3250 1             
					call 130                     
				endcase                          
				case 2723005                     
					DEC_ITEM_NUM_BYIDX 2723005 1 @113 @114 1
					task_fini 3250 1             
					call 130                     
				endcase                          
				case 2733005                     
					DEC_ITEM_NUM_BYIDX 2733005 1 @113 @114 1
					task_fini 3250 1             
					call 130                     
				endcase                          
				case 2743005                     
					DEC_ITEM_NUM_BYIDX 2743005 1 @113 @114 1
					task_fini 3250 1            
					call 130
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg 物品不符任务要求，未能成功上交
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif
endproc 
 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
