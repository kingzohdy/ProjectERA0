;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;
proc 130
	add_sys_msg_start
	add_sys_msg �ɹ��Ͻ���Ʒ���������
	set_sys_msg_mode 1 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc 

 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
proc 170
	dialog 3
	add_talk $_npc$:
	add_talk #b#b�ռ������Ķ��������������ӡ�
	add_talk #b#b

	task_stat 3617 1 101
	task_stat 3618 1 102
	task_stat 3619 1 103
	task_stat 3620 1 104
	task_stat 3621 1 105
	task_stat 3622 1 106
	task_stat 3623 1 107
	task_stat 3624 1 108
	task_stat 3625 1 109
	task_stat 3626 1 110
			        
	if 101 = 2
		add_talk #b#b#c186506��Ҫ�ռ�71��~80���׶ε���ɫƷ��������Ʒ#n
	endif
	if 102 = 2
		add_talk #b#b#c186506��Ҫ�ռ�71��~80���׶ε���ɫƷ�ʸ�����Ʒ#n
	endif
	if 103 = 2
		add_talk #b#b#c186506��Ҫ�ռ�71��~80���׶ε���ɫƷ��ͷ��#n
	endif
	if 104 = 2
		add_talk #b#b#c186506��Ҫ�ռ�71��~80���׶ε���ɫƷ�ʼ��#n
	endif
	if 105 = 2
		add_talk #b#b#c186506��Ҫ�ռ�71��~80���׶ε���ɫƷ������#n
	endif
	if 106 = 2
		add_talk #b#b#c186506��Ҫ�ռ�71��~80���׶ε���ɫƷ���·�#n
	endif	
	if 107 = 2
		add_talk #b#b#c186506��Ҫ�ռ�71��~80���׶ε���ɫƷ��սѥ#n
	endif
	if 108 = 2
		add_talk #b#b#c186506��Ҫ�ռ�71��~80���׶ε���ɫƷ������#n
	endif
	if 109 = 2
		add_talk #b#b#c186506��Ҫ�ռ�71��~80���׶ε���ɫƷ�ʽ�ָ#n
	endif
	if 110 = 2
		add_talk #b#b#c186506��Ҫ�ռ�71��~80���׶ε���ɫƷ�ʻ���#n
	endif
	
	add_talk #b#b
	add_talk #b#b#c186506���������Ҫ�Ͻ�����Ʒ�ŵ��·����۴���#n
	add_talk #b#b#c186506<�Ҽ������������ƷҲ��ֱ�ӷ���>#n
	
		NPC_COMMIT_OPT 167
	talk
endproc
proc 171
	GET_COMMIT_ITEM_INFO 1 112
	;ID
	GET_COMMIT_ITEM_INFO 2 113
	;λ��
	GET_COMMIT_ITEM_INFO 3 114
	;����

	if 112 = -1
		add_sys_msg_start
		add_sys_msg �㻹û���ύ��Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info		
		return 
	endif
	task_stat 3617 1 116
	if 116 = 2
		if 112 = 1013008 or 112 = 1253008 or 112 = 1203008 or 112 = 1103008 or 112 = 1033008 or 112 = 1503008 or 112 = 1513008 or 112 = 1113008
			switch 112
				case 10130078
					DEC_ITEM_NUM_BYIDX 1013008 1 @113 @114 1
					task_fini 3617 1
					call 130
				endcase
				case 1253008
					DEC_ITEM_NUM_BYIDX 1253008 1 @113 @114 1
					task_fini 3617 1
					call 130
				endcase
				case 1203008
					DEC_ITEM_NUM_BYIDX 1203008 1 @113 @114 1
					task_fini 3617 1
					call 130
				endcase
				case 1103008
					DEC_ITEM_NUM_BYIDX 1103008 1 @113 @114 1
					task_fini 3617 1
					call 130
				endcase
				case 1033008
					DEC_ITEM_NUM_BYIDX 1033008 1 @113 @114 1
					task_fini 3617 1
					call 130
				endcase
				case 1503008
					DEC_ITEM_NUM_BYIDX 1503008 1 @113 @114 1
					task_fini 3617 1
					call 130
				endcase
				case 1513008
					DEC_ITEM_NUM_BYIDX 1513008 1 @113 @114 1
					task_fini 3617 1
					call 130
				endcase
				case 1113008
					DEC_ITEM_NUM_BYIDX 1113008 1 @113 @114 1
					task_fini 3617 1
					call 130
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg ��Ʒ��������Ҫ��δ�ܳɹ��Ͻ�
			set_sys_msg_mode 1 0 0
			msg_info
			
		endif
		
	endif
;;;;;;;;;		;;;;;;;;
	task_stat 3618 1 117
	if 117 = 2
		if 112 = 1583008 or 112 = 1283008 or 112 = 1183008 or 112 = 1063008 or 112 = 1183028 or 112 = 1583028
			switch 112
				case 1583008
					DEC_ITEM_NUM_BYIDX 1583008 1 @113 @114 1
					task_fini 3618 1             
					call 130                     
				endcase                          
				case 1283008                     
					DEC_ITEM_NUM_BYIDX 1283008 1 @113 @114 1
					task_fini 3618 1             
					call 130                     
				endcase                          
				case 1183008                     
					DEC_ITEM_NUM_BYIDX 1183008 1 @113 @114 1
					task_fini 3618 1             
					call 130                     
				endcase                          
				case 1063008                     
					DEC_ITEM_NUM_BYIDX 1063008 1 @113 @114 1
					task_fini 3618 1            
					call 130
				endcase
				case 1183028                     
					DEC_ITEM_NUM_BYIDX 1183028 1 @113 @114 1
					task_fini 3618 1            
					call 130
				endcase
				case 1583028       
					DEC_ITEM_NUM_BYIDX 1583028 1 @113 @114 1
					task_fini 3618 1            
					call 130
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg ��Ʒ��������Ҫ��δ�ܳɹ��Ͻ�
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif
;;;;;;;;;		;;;;;;;;
	task_stat 3619 1 118
	if 118 = 2
		if 112 = 2133008 or 112 = 2143008 or 112 = 2123008 or 112 = 2113008 
			switch 112
				case 2133008
					DEC_ITEM_NUM_BYIDX 2133008 1 @113 @114 1
					task_fini 3619 1             
					call 130                     
				endcase                          
				case 2143008                     
					DEC_ITEM_NUM_BYIDX 2143008 1 @113 @114 1
					task_fini 3619 1             
					call 130                     
				endcase                          
				case 2123008                     
					DEC_ITEM_NUM_BYIDX 2123008 1 @113 @114 1
					task_fini 3619 1             
					call 130                     
				endcase                          
				case 2113008                     
					DEC_ITEM_NUM_BYIDX 2113008 1 @113 @114 1
					task_fini 3619 1            
					call 130
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg ��Ʒ��������Ҫ��δ�ܳɹ��Ͻ�
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif
;;;;;;;;;		;;;;;;;;
	task_stat 3620 1 119
	if 119 = 2
		if 112 = 2243008 or 112 = 2223008 or 112 = 2213008 or 112 = 1273008 
			switch 112
				case 2243008
					DEC_ITEM_NUM_BYIDX 2243008 1 @113 @114 1
					task_fini 3620 1             
					call 130                     
				endcase                          
				case 2223008                     
					DEC_ITEM_NUM_BYIDX 2223008 1 @113 @114 1
					task_fini 3620 1             
					call 130                     
				endcase                          
				case 2213008                     
					DEC_ITEM_NUM_BYIDX 2213008 1 @113 @114 1
					task_fini 3620 1             
					call 130                     
				endcase                          
				case 1273008                     
					DEC_ITEM_NUM_BYIDX 1273008 1 @113 @114 1
					task_fini 3620 1            
					call 130
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg ��Ʒ��������Ҫ��δ�ܳɹ��Ͻ�
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif
;;;;;;;;;		;;;;;;;;
	task_stat 3621 1 120
	if 120 = 2
		if 112 = 2423008 or 112 = 2433008 or 112 = 2443008 or 112 = 2413008 
			switch 112
				case 2423008
					DEC_ITEM_NUM_BYIDX 2423008 1 @113 @114 1
					task_fini 3621 1             
					call 130                     
				endcase                          
				case 2433008                     
					DEC_ITEM_NUM_BYIDX 2433008 1 @113 @114 1
					task_fini 3621 1             
					call 130                     
				endcase                          
				case 2443008                     
					DEC_ITEM_NUM_BYIDX 2443008 1 @113 @114 1
					task_fini 3621 1             
					call 130                     
				endcase                          
				case 2413008                     
					DEC_ITEM_NUM_BYIDX 2413008 1 @113 @114 1
					task_fini 3621 1            
					call 130
				endcase
			
			endswitch
		else
			add_sys_msg_start
			add_sys_msg ��Ʒ��������Ҫ��δ�ܳɹ��Ͻ�
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif
;;;;;;;;;		;;;;;;;;
	task_stat 3622 1 121
	if 121 = 2
		if 112 = 2013008 or 112 = 2023008 or 112 = 2033008 or 112 = 2043008 
			switch 112
				case 2013008
					DEC_ITEM_NUM_BYIDX 2013008 1 @113 @114 1
					task_fini 3622 1             
					call 130                     
				endcase                          
				case 2023008                     
					DEC_ITEM_NUM_BYIDX 2023008 1 @113 @114 1
					task_fini 3622 1             
					call 130                     
				endcase                          
				case 2033008                     
					DEC_ITEM_NUM_BYIDX 2033008 1 @113 @114 1
					task_fini 3622 1             
					call 130                     
				endcase                          
				case 2043008                     
					DEC_ITEM_NUM_BYIDX 2043008 1 @113 @114 1
					task_fini 3622 1            
					call 130
				endcase
			
			endswitch
		else
			add_sys_msg_start
			add_sys_msg ��Ʒ��������Ҫ��δ�ܳɹ��Ͻ�
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif
;;;;;;;;;		;;;;;;;;
	task_stat 3623 1 123
	if 123 = 2
		if 112 = 2613008 or 112 = 2633008 or 112 = 2643008 or 112 = 2623008 
			switch 112
				case 2613008
					DEC_ITEM_NUM_BYIDX 2613008 1 @113 @114 1
					task_fini 3623 1             
					call 130                     
				endcase                          
				case 2633008                     
					DEC_ITEM_NUM_BYIDX 2633008 1 @113 @114 1
					task_fini 3623 1             
					call 130                     
				endcase                          
				case 2643008                     
					DEC_ITEM_NUM_BYIDX 2643008 1 @113 @114 1
					task_fini 3623 1             
					call 130                     
				endcase                          
				case 2623008                     
					DEC_ITEM_NUM_BYIDX 2623008 1 @113 @114 1
					task_fini 3623 1            
					call 130
				endcase
			
			endswitch
		else
			add_sys_msg_start
			add_sys_msg ��Ʒ��������Ҫ��δ�ܳɹ��Ͻ�
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif
;;;;;;;;;		;;;;;;;;
	task_stat 3624 1 124
	if 124 = 2
	
		if 112 = 2813008 
			switch 112
				case 2813008
					DEC_ITEM_NUM_BYIDX 2813008 1 @113 @114 1
					task_fini 3624 1             
					call 130                     
				endcase                          
;				case 2833006                     
;					DEC_ITEM_NUM_BYIDX 2833006 1 @113 @114 1
;					task_fini 3266 1             
;					call 130                     
;				endcase                          
;				case 2843006                     
;					DEC_ITEM_NUM_BYIDX 2843006 1 @113 @114 1
;					task_fini 3266 1             
;					call 130                     
;				endcase                          
;				case 2823006                     
;					DEC_ITEM_NUM_BYIDX 2823006 1 @113 @114 1
;					task_fini 3266 1            
;					call 130
;				endcase		
			endswitch
		else
			add_sys_msg_start
			add_sys_msg ��Ʒ��������Ҫ��δ�ܳɹ��Ͻ�
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif
;;;;;;;;;		;;;;;;;;
	task_stat 3625 1 125
	if 125 = 2
		if 112 = 2923008 or 112 = 2913008 
			switch 112
				case 2923008
					DEC_ITEM_NUM_BYIDX 2923008 1 @113 @114 1
					task_fini 3625 1             
					call 130                     
				endcase                          
				case 2913008                     
					DEC_ITEM_NUM_BYIDX 2913008 1 @113 @114 1
					task_fini 3625 1             
					call 130                     
				endcase                          
;				case 2913006                     
;					DEC_ITEM_NUM_BYIDX 2913006 1 @113 @114 1
;					task_fini 3267 1             
;					call 130                     
;				endcase                          
;				case 2943006                     
;					DEC_ITEM_NUM_BYIDX 2943006 1 @113 @114 1
;					task_fini 3267 1            
;					call 130
;				endcase			
			endswitch
		else
			add_sys_msg_start
			add_sys_msg ��Ʒ��������Ҫ��δ�ܳɹ��Ͻ�
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif
;;;;;;;;;		;;;;;;;;
	task_stat 3626 1 126
	if 126 = 2
		if 112 = 2713008 
			switch 112
				case 2713008
					DEC_ITEM_NUM_BYIDX 2713008 1 @113 @114 1
					task_fini 3626 1             
					call 130                     
				endcase                          
;				case 2723006                     
;					DEC_ITEM_NUM_BYIDX 2723006 1 @113 @114 1
;					task_fini 3268 1             
;					call 130                     
;				endcase                          
;				case 2733006                     
;					DEC_ITEM_NUM_BYIDX 2733006 1 @113 @114 1
;					task_fini 3268 1             
;					call 130                     
;;				endcase                          
	;			case 2743006                     
	;				DEC_ITEM_NUM_BYIDX 2743006 1 @113 @114 1
	;				task_fini 3268 1            
	;				call 130
	;			endcase
		endswitch
		else
			add_sys_msg_start
			add_sys_msg ��Ʒ��������Ҫ��δ�ܳɹ��Ͻ�
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	endif	
endproc
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 