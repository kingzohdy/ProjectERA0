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
proc 160
	dialog 3
	add_talk $_npc$:
	add_talk #b#b�ռ������Ķ��������������ӡ�
	add_talk #b#b

	task_stat 3569 1 101
	task_stat 3570 1 102
	task_stat 3571 1 103
	task_stat 3572 1 104
	task_stat 3573 1 105
	task_stat 3574 1 106
	task_stat 3575 1 107
	task_stat 3576 1 108
	task_stat 3577 1 109
	task_stat 3578 1 110
			        
	if 101 = 2
		add_talk #b#b#c186506��Ҫ�ռ�61��~70���׶ε���ɫƷ��������Ʒ#n
	endif
	if 102 = 2
		add_talk #b#b#c186506��Ҫ�ռ�61��~70���׶ε���ɫƷ�ʸ�����Ʒ#n
	endif
	if 103 = 2
		add_talk #b#b#c186506��Ҫ�ռ�61��~70���׶ε���ɫƷ��ͷ��#n
	endif
	if 104 = 2
		add_talk #b#b#c186506��Ҫ�ռ�61��~70���׶ε���ɫƷ�ʼ��#n
	endif
	if 105 = 2
		add_talk #b#b#c186506��Ҫ�ռ�61��~70���׶ε���ɫƷ������#n
	endif
	if 106 = 2
		add_talk #b#b#c186506��Ҫ�ռ�61��~70���׶ε���ɫƷ���·�#n
	endif	
	if 107 = 2
		add_talk #b#b#c186506��Ҫ�ռ�61��~70���׶ε���ɫƷ��սѥ#n
	endif
	if 108 = 2
		add_talk #b#b#c186506��Ҫ�ռ�61��~70���׶ε���ɫƷ������#n
	endif
	if 109 = 2
		add_talk #b#b#c186506��Ҫ�ռ�61��~70���׶ε���ɫƷ�ʽ�ָ#n
	endif
	if 110 = 2
		add_talk #b#b#c186506��Ҫ�ռ�61��~70���׶ε���ɫƷ�ʻ���#n
	endif
	
	add_talk #b#b
	add_talk #b#b#c186506���������Ҫ�Ͻ�����Ʒ�ŵ��·����۴���#n
	add_talk #b#b#c186506<�Ҽ������������ƷҲ��ֱ�ӷ���>#n
	
		NPC_COMMIT_OPT 157
	talk
endproc
proc 161
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
	task_stat 3569 1 116
	if 116 = 2
		if 112 = 1013007 or 112 = 1253007 or 112 = 1203007 or 112 = 1103007 or 112 = 1033007 or 112 = 1503007 or 112 = 1513007 or 112 = 1113007
			switch 112
				case 1013007
					DEC_ITEM_NUM_BYIDX 1013007 1 @113 @114 1
					task_fini 3569 1
					call 130
				endcase
				case 1253007
					DEC_ITEM_NUM_BYIDX 1253007 1 @113 @114 1
					task_fini 3569 1
					call 130
				endcase
				case 1203007
					DEC_ITEM_NUM_BYIDX 1203007 1 @113 @114 1
					task_fini 3569 1
					call 130
				endcase
				case 1103007
					DEC_ITEM_NUM_BYIDX 1103007 1 @113 @114 1
					task_fini 3569 1
					call 130
				endcase
				case 1033007
					DEC_ITEM_NUM_BYIDX 1033007 1 @113 @114 1
					task_fini 3569 1
					call 130
				endcase
				case 1503007
					DEC_ITEM_NUM_BYIDX 1503007 1 @113 @114 1
					task_fini 3569 1
					call 130
				endcase
				case 1513007
					DEC_ITEM_NUM_BYIDX 1513007 1 @113 @114 1
					task_fini 3569 1
					call 130
				endcase
				case 1113007
					DEC_ITEM_NUM_BYIDX 1113007 1 @113 @114 1
					task_fini 3569 1
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
	task_stat 3570 1 117
	if 117 = 2
		if 112 = 1583007 or 112 = 1283007 or 112 = 1183007 or 112 = 1063007 
			switch 112
				case 1583007
					DEC_ITEM_NUM_BYIDX 1583007 1 @113 @114 1
					task_fini 3570 1             
					call 130                     
				endcase                          
				case 1283007                     
					DEC_ITEM_NUM_BYIDX 1283007 1 @113 @114 1
					task_fini 3570 1             
					call 130                     
				endcase                          
				case 1183007                     
					DEC_ITEM_NUM_BYIDX 1183007 1 @113 @114 1
					task_fini 3570 1             
					call 130                     
				endcase                          
				case 1063007                     
					DEC_ITEM_NUM_BYIDX 1063007 1 @113 @114 1
					task_fini 3570 1            
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
	task_stat 3571 1 118
	if 118 = 2
		if 112 = 2133007 or 112 = 2143007 or 112 = 2123007 or 112 = 2113007 
			switch 112
				case 2133007
					DEC_ITEM_NUM_BYIDX 2133007 1 @113 @114 1
					task_fini 3571 1             
					call 130                     
				endcase                          
				case 2143007                     
					DEC_ITEM_NUM_BYIDX 2143007 1 @113 @114 1
					task_fini 3571 1             
					call 130                     
				endcase                          
				case 2123007                     
					DEC_ITEM_NUM_BYIDX 2123007 1 @113 @114 1
					task_fini 3571 1             
					call 130                     
				endcase                          
				case 2113007                     
					DEC_ITEM_NUM_BYIDX 2113007 1 @113 @114 1
					task_fini 3571 1            
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
	task_stat 3572 1 119
	if 119 = 2
		if 112 = 2243007 or 112 = 2223007 or 112 = 2213007 or 112 = 1273007 
			switch 112
				case 2243007
					DEC_ITEM_NUM_BYIDX 2243007 1 @113 @114 1
					task_fini 3572 1             
					call 130                     
				endcase                          
				case 2223007                     
					DEC_ITEM_NUM_BYIDX 2223007 1 @113 @114 1
					task_fini 3572 1             
					call 130                     
				endcase                          
				case 2213007                     
					DEC_ITEM_NUM_BYIDX 2213007 1 @113 @114 1
					task_fini 3572 1             
					call 130                     
				endcase                          
				case 1273007                     
					DEC_ITEM_NUM_BYIDX 1273007 1 @113 @114 1
					task_fini 3572 1            
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
	task_stat 3573 1 120
	if 120 = 2
		if 112 = 2423007 or 112 = 2433007 or 112 = 2443007 or 112 = 2413007 
			switch 112
				case 2423007
					DEC_ITEM_NUM_BYIDX 2423007 1 @113 @114 1
					task_fini 3573 1             
					call 130                     
				endcase                          
				case 2433007                     
					DEC_ITEM_NUM_BYIDX 2433007 1 @113 @114 1
					task_fini 3573 1             
					call 130                     
				endcase                          
				case 2443007                     
					DEC_ITEM_NUM_BYIDX 2443007 1 @113 @114 1
					task_fini 3573 1             
					call 130                     
				endcase                          
				case 2413007                     
					DEC_ITEM_NUM_BYIDX 2413007 1 @113 @114 1
					task_fini 3573 1            
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
	task_stat 3574 1 121
	if 121 = 2
		if 112 = 2013007 or 112 = 2023007 or 112 = 2033007 or 112 = 2043007 
			switch 112
				case 2013007
					DEC_ITEM_NUM_BYIDX 2013007 1 @113 @114 1
					task_fini 3574 1             
					call 130                     
				endcase                          
				case 2023007                     
					DEC_ITEM_NUM_BYIDX 2023007 1 @113 @114 1
					task_fini 3574 1             
					call 130                     
				endcase                          
				case 2033007                     
					DEC_ITEM_NUM_BYIDX 2033007 1 @113 @114 1
					task_fini 3574 1             
					call 130                     
				endcase                          
				case 2043007                     
					DEC_ITEM_NUM_BYIDX 2043007 1 @113 @114 1
					task_fini 3574 1            
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
	task_stat 3575 1 123
	if 123 = 2
		if 112 = 2613007 or 112 = 2633007 or 112 = 2643007 or 112 = 2623007 
			switch 112
				case 2613007
					DEC_ITEM_NUM_BYIDX 2613007 1 @113 @114 1
					task_fini 3575 1             
					call 130                     
				endcase                          
				case 2633007                     
					DEC_ITEM_NUM_BYIDX 2633007 1 @113 @114 1
					task_fini 3575 1             
					call 130                     
				endcase                          
				case 2643007                     
					DEC_ITEM_NUM_BYIDX 2643007 1 @113 @114 1
					task_fini 3575 1             
					call 130                     
				endcase                          
				case 2623007                     
					DEC_ITEM_NUM_BYIDX 2623007 1 @113 @114 1
					task_fini 3575 1            
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
	task_stat 3576 1 124
	if 124 = 2
	
		if 112 = 2813007 
			switch 112
				case 2813007
					DEC_ITEM_NUM_BYIDX 2813007 1 @113 @114 1
					task_fini 3576 1             
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
	task_stat 3577 1 125
	if 125 = 2
		if 112 = 2923007 or 112 = 2913007 
			switch 112
				case 2923007
					DEC_ITEM_NUM_BYIDX 2923007 1 @113 @114 1
					task_fini 3577 1             
					call 130                     
				endcase                          
				case 2913007                     
					DEC_ITEM_NUM_BYIDX 2913007 1 @113 @114 1
					task_fini 3577 1             
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
	task_stat 3578 1 126
	if 126 = 2
		if 112 = 2713007 
			switch 112
				case 2713007
					DEC_ITEM_NUM_BYIDX 2713007 1 @113 @114 1
					task_fini 3578 1             
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
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 