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
proc 150
	dialog 3
	add_talk $_npc$:
	add_talk #b#b�ռ������Ķ��������������ӡ�
	add_talk #b#b

	task_stat 3259 1 101
	task_stat 3260 1 102
	task_stat 3261 1 103
	task_stat 3262 1 104
	task_stat 3263 1 105
	task_stat 3264 1 106
	task_stat 3265 1 107
	task_stat 3266 1 108
	task_stat 3267 1 109
	task_stat 3268 1 110
			        
	if 101 = 2
		add_talk #b#b#c186506��Ҫ�ռ�51��~60���׶ε���ɫƷ��������Ʒ#n
	endif
	if 102 = 2
		add_talk #b#b#c186506��Ҫ�ռ�51��~60���׶ε���ɫƷ�ʸ�����Ʒ#n
	endif
	if 103 = 2
		add_talk #b#b#c186506��Ҫ�ռ�51��~60���׶ε���ɫƷ��ͷ��#n
	endif
	if 104 = 2
		add_talk #b#b#c186506��Ҫ�ռ�51��~60���׶ε���ɫƷ�ʼ��#n
	endif
	if 105 = 2
		add_talk #b#b#c186506��Ҫ�ռ�51��~60���׶ε���ɫƷ������#n
	endif
	if 106 = 2
		add_talk #b#b#c186506��Ҫ�ռ�51��~60���׶ε���ɫƷ���·�#n
	endif	
	if 107 = 2
		add_talk #b#b#c186506��Ҫ�ռ�51��~60���׶ε���ɫƷ��սѥ#n
	endif
	if 108 = 2
		add_talk #b#b#c186506��Ҫ�ռ�51��~60���׶ε���ɫƷ������#n
	endif
	if 109 = 2
		add_talk #b#b#c186506��Ҫ�ռ�51��~60���׶ε���ɫƷ�ʽ�ָ#n
	endif
	if 110 = 2
		add_talk #b#b#c186506��Ҫ�ռ�51��~60���׶ε���ɫƷ�ʻ���#n
	endif
	
	add_talk #b#b
	add_talk #c186506���������Ҫ�Ͻ�����Ʒ�ŵ��·����۴���#n
	add_talk #c186506<�Ҽ������������ƷҲ��ֱ�ӷ���>#n
	
		NPC_COMMIT_OPT 147
	talk
endproc
proc 151
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
	task_stat 3259 1 116
	if 116 = 2
		if 112 = 1013006 or 112 = 1253006 or 112 = 1203006 or 112 = 1103006 or 112 = 1033006 or 112 = 1503006
			switch 112
				case 1013006
					DEC_ITEM_NUM_BYIDX 1013006 1 @113 @114 1
					task_fini 3259 1
					call 130
				endcase
				case 1253006
					DEC_ITEM_NUM_BYIDX 1253006 1 @113 @114 1
					task_fini 3259 1
					call 130
				endcase
				case 1203006
					DEC_ITEM_NUM_BYIDX 1203006 1 @113 @114 1
					task_fini 3259 1
					call 130
				endcase
				case 1103006
					DEC_ITEM_NUM_BYIDX 1103006 1 @113 @114 1
					task_fini 3259 1
					call 130
				endcase
				case 1033006
					DEC_ITEM_NUM_BYIDX 1033006 1 @113 @114 1
					task_fini 3259 1
					call 130
				endcase
				case 1503006
					DEC_ITEM_NUM_BYIDX 1503006 1 @113 @114 1
					task_fini 3259 1
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
	task_stat 3260 1 117
	if 117 = 2
		if 112 = 1583006 or 112 = 1283006 or 112 = 1183006 or 112 = 1063006 
			switch 112
				case 1583006
					DEC_ITEM_NUM_BYIDX 1583006 1 @113 @114 1
					task_fini 3260 1             
					call 130                     
				endcase                          
				case 1283006                     
					DEC_ITEM_NUM_BYIDX 1283006 1 @113 @114 1
					task_fini 3260 1             
					call 130                     
				endcase                          
				case 1183006                     
					DEC_ITEM_NUM_BYIDX 1183006 1 @113 @114 1
					task_fini 3260 1             
					call 130                     
				endcase                          
				case 1063006                     
					DEC_ITEM_NUM_BYIDX 1063006 1 @113 @114 1
					task_fini 3260 1            
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
	task_stat 3261 1 118
	if 118 = 2
		if 112 = 2133006 or 112 = 2143006 or 112 = 2123006 or 112 = 2113006 
			switch 112
				case 2133006
					DEC_ITEM_NUM_BYIDX 2133006 1 @113 @114 1
					task_fini 3261 1             
					call 130                     
				endcase                          
				case 2143006                     
					DEC_ITEM_NUM_BYIDX 2143006 1 @113 @114 1
					task_fini 3261 1             
					call 130                     
				endcase                          
				case 2123006                     
					DEC_ITEM_NUM_BYIDX 2123006 1 @113 @114 1
					task_fini 3261 1             
					call 130                     
				endcase                          
				case 2113006                     
					DEC_ITEM_NUM_BYIDX 2113006 1 @113 @114 1
					task_fini 3261 1            
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
	task_stat 3262 1 119
	if 119 = 2
		if 112 = 2243006 or 112 = 2223006 or 112 = 2213006 or 112 = 1273006 
			switch 112
				case 2243006
					DEC_ITEM_NUM_BYIDX 2243006 1 @113 @114 1
					task_fini 3262 1             
					call 130                     
				endcase                          
				case 2223006                     
					DEC_ITEM_NUM_BYIDX 2223006 1 @113 @114 1
					task_fini 3262 1             
					call 130                     
				endcase                          
				case 2213006                     
					DEC_ITEM_NUM_BYIDX 2213006 1 @113 @114 1
					task_fini 3262 1             
					call 130                     
				endcase                          
				case 1273006                     
					DEC_ITEM_NUM_BYIDX 1273006 1 @113 @114 1
					task_fini 3262 1            
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
	task_stat 3263 1 120
	if 120 = 2
		if 112 = 2423006 or 112 = 2433006 or 112 = 2443006 or 112 = 2413006 
			switch 112
				case 2423006
					DEC_ITEM_NUM_BYIDX 2423006 1 @113 @114 1
					task_fini 3263 1             
					call 130                     
				endcase                          
				case 2433006                     
					DEC_ITEM_NUM_BYIDX 2433006 1 @113 @114 1
					task_fini 3263 1             
					call 130                     
				endcase                          
				case 2443006                     
					DEC_ITEM_NUM_BYIDX 2443006 1 @113 @114 1
					task_fini 3263 1             
					call 130                     
				endcase                          
				case 2413006                     
					DEC_ITEM_NUM_BYIDX 2413006 1 @113 @114 1
					task_fini 3263 1            
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
	task_stat 3264 1 121
	if 121 = 2
		if 112 = 2013006 or 112 = 2023006 or 112 = 2033006 or 112 = 2043006 
			switch 112
				case 2013006
					DEC_ITEM_NUM_BYIDX 2013006 1 @113 @114 1
					task_fini 3264 1             
					call 130                     
				endcase                          
				case 2023006                     
					DEC_ITEM_NUM_BYIDX 2023006 1 @113 @114 1
					task_fini 3264 1             
					call 130                     
				endcase                          
				case 2033006                     
					DEC_ITEM_NUM_BYIDX 2033006 1 @113 @114 1
					task_fini 3264 1             
					call 130                     
				endcase                          
				case 2043006                     
					DEC_ITEM_NUM_BYIDX 2043006 1 @113 @114 1
					task_fini 3264 1            
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
	task_stat 3265 1 123
	if 123 = 2
		if 112 = 2613006 or 112 = 2633006 or 112 = 2643006 or 112 = 2623006 
			switch 112
				case 2613006
					DEC_ITEM_NUM_BYIDX 2613006 1 @113 @114 1
					task_fini 3265 1             
					call 130                     
				endcase                          
				case 2633006                     
					DEC_ITEM_NUM_BYIDX 2633006 1 @113 @114 1
					task_fini 3265 1             
					call 130                     
				endcase                          
				case 2643006                     
					DEC_ITEM_NUM_BYIDX 2643006 1 @113 @114 1
					task_fini 3265 1             
					call 130                     
				endcase                          
				case 2623006                     
					DEC_ITEM_NUM_BYIDX 2623006 1 @113 @114 1
					task_fini 3265 1            
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
	task_stat 3266 1 124
	if 124 = 2
	
		if 112 = 2813006 or 112 = 2833006 or 112 = 2843006 or 112 = 2823006 
			switch 112
				case 2813006
					DEC_ITEM_NUM_BYIDX 2813006 1 @113 @114 1
					task_fini 3266 1             
					call 130                     
				endcase                          
				case 2833006                     
					DEC_ITEM_NUM_BYIDX 2833006 1 @113 @114 1
					task_fini 3266 1             
					call 130                     
				endcase                          
				case 2843006                     
					DEC_ITEM_NUM_BYIDX 2843006 1 @113 @114 1
					task_fini 3266 1             
					call 130                     
				endcase                          
				case 2823006                     
					DEC_ITEM_NUM_BYIDX 2823006 1 @113 @114 1
					task_fini 3266 1            
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
	task_stat 3267 1 125
	if 125 = 2
		if 112 = 2933006 or 112 = 2923006 or 112 = 2913006 or 112 = 2943006 
			switch 112
				case 2933006
					DEC_ITEM_NUM_BYIDX 2933006 1 @113 @114 1
					task_fini 3267 1             
					call 130                     
				endcase                          
				case 2923006                     
					DEC_ITEM_NUM_BYIDX 2923006 1 @113 @114 1
					task_fini 3267 1             
					call 130                     
				endcase                          
				case 2913006                     
					DEC_ITEM_NUM_BYIDX 2913006 1 @113 @114 1
					task_fini 3267 1             
					call 130                     
				endcase                          
				case 2943006                     
					DEC_ITEM_NUM_BYIDX 2943006 1 @113 @114 1
					task_fini 3267 1            
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
	task_stat 3268 1 126
	if 126 = 2
		if 112 = 2713006 or 112 = 2723006 or 112 = 2733006 or 112 = 2743006 
			switch 112
				case 2713006
					DEC_ITEM_NUM_BYIDX 2713006 1 @113 @114 1
					task_fini 3268 1             
					call 130                     
				endcase                          
				case 2723006                     
					DEC_ITEM_NUM_BYIDX 2723006 1 @113 @114 1
					task_fini 3268 1             
					call 130                     
				endcase                          
				case 2733006                     
					DEC_ITEM_NUM_BYIDX 2733006 1 @113 @114 1
					task_fini 3268 1             
					call 130                     
				endcase                          
				case 2743006                     
					DEC_ITEM_NUM_BYIDX 2743006 1 @113 @114 1
					task_fini 3268 1            
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
endproc
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 