;�����ػ���Ŧ�    ,��������
proc 1
	DIALOG 1
	strvar ClanRet
	is_in_my_clan_city  ClanRet
	if ClanRet = 0
		add_talk $_npc$:
		add_talk #b#b�����ѻ��ǵ��ˣ����Ǹ����⡭��
		add_talk #b#b���ǳ�����������$_npc$���Ҵ���#c186506$_CityOwnerClanName$#n�������ȫ���Ա��ӭ���ĵ�����
		add_talk #b#b
		add_talk #b#b�����ڿ����ڡ�#c186506$_CityOwnerClanName$#n������ĳ������һ���ˣ�����Ҫע�����ǵ�����Ŷ������ǵ����ǿ��ǻ�ܲҵİ�����
	else
	
	strvar Ret RetC RetG RetCh ZRet
	GET_CLAN_BUILDING_LV Ret 0 0	
	GET_CLAN_BUILDING_LV RetC 0 1
	GET_CLAN_BUILDING_LV RetG 0 2
	GET_CLAN_BUILDING_LV RetCh 0 5
		
	GET_CLAN_USED_TECH_POINT ZRet 0 0
	
	add_talk $_npc$:
	add_talk #b#b���ܼ�����˵�����Ѿ���ǿ���ˣ�������߻��кܶ�����������ȥ̽����
;	add_talk #b#b#c186506��ǰ������еȼ�Ϊ��$@RetCh$��#n
	if Ret = 100 or Ret = 0
		add_talk #b#b#c186506��ǰ�Ƽ����ĵȼ�Ϊ��0����δ���裩#n
	endif
	if Ret = 1 or Ret = 101
		add_talk #b#b#c186506��ǰ�Ƽ����ĵȼ�Ϊ��1��#n
	endif
	if Ret = 2 or Ret = 102
		add_talk #b#b#c186506��ǰ�Ƽ����ĵȼ�Ϊ��2��#n
	endif
;;;;;;;;;;;;;;;;;;;;;;;;
	if RetC = 100 or RetC = 0
;		add_talk #b#b#c186506��ǰ�ɼ����ĵȼ�Ϊ��0����δ���裩#n
	endif
	if RetC = 1 or RetC = 101
;		add_talk #b#b#c186506��ǰ�ɼ����ĵȼ�Ϊ��1��#n
	endif
	if RetC = 2 or RetC = 102
;		add_talk #b#b#c186506��ǰ�ɼ����ĵȼ�Ϊ��2��#n
	endif
;;;;;;;;;;;;;;;;;;;;;;;;;
	if RetG = 100 or RetG = 0
;		add_talk #b#b#c186506��ǰ�����ȼ�Ϊ��0����δ���裩#n
	endif
	if RetG = 101 or RetG = 1
;		add_talk #b#b#c186506��ǰ�����ȼ�Ϊ��1��#n
	endif
	if RetG = 102 or RetG = 2
;		add_talk #b#b#c186506��ǰ�����ȼ�Ϊ��2��#n
	endif
	
	add_talk #b#b
;	add_talk #b#b#c186506��ǰ����Ͷ����ܿƼ���Ϊ��$@ZRet$#n	
		
;	TASKCYCLE_ADD_OPTION 110 1 133
;	TASKCYCLE_ADD_OPTION 119 1 160
;	TASKCYCLE_ADD_OPTION 120 1 161
;	TASKCYCLE_ADD_OPTION 112 1 162
;	TASKCYCLE_ADD_OPTION 113 1 163
;	TASKCYCLE_ADD_OPTION 114 1 164
	
;;;;;;;;;;;;;;;;;;;;;;;;;;30~40��	
	
	strvar Ret3227 Ret3228 Ret3229 Ret3230 Ret3231 Ret3232 Ret3233 Ret3234 Ret3235 Ret3236
	task_stat 3227 1 Ret3227
	task_stat 3228 1 Ret3228
	task_stat 3229 1 Ret3229
	task_stat 3230 1 Ret3230
	task_stat 3231 1 Ret3231
	task_stat 3232 1 Ret3232
	task_stat 3233 1 Ret3233
	task_stat 3234 1 Ret3234
	task_stat 3235 1 Ret3235
	task_stat 3236 1 Ret3236
	if Ret3227 = 2 or Ret3228 = 2 or Ret3229 = 2 or Ret3230 = 2 or Ret3231 = 2 or Ret3232 = 2 or Ret3233 = 2 or Ret3234 = 2 or Ret3235 = 2 or Ret3236 = 2 
		option 135 #��������-�Ͻ���Ʒ
	endif	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;41~50��
	strvar Ret3241 Ret3242 Ret3243 Ret3244 Ret3245 Ret3246 Ret3247 Ret3248 Ret3249 Ret3250
	task_stat 3241 1 Ret3241
	task_stat 3242 1 Ret3242
	task_stat 3243 1 Ret3243
	task_stat 3244 1 Ret3244
	task_stat 3245 1 Ret3245
	task_stat 3246 1 Ret3246
	task_stat 3247 1 Ret3247
	task_stat 3248 1 Ret3248
	task_stat 3249 1 Ret3249
	task_stat 3250 1 Ret3250
	if Ret3241 = 2 or Ret3242 = 2 or Ret3243 = 2 or Ret3244 = 2 or Ret3245 = 2 or Ret3246 = 2 or Ret3248 = 2 or Ret3249 = 2 or Ret3250 = 2 or Ret3247 = 2 
		option 136 #��������-�Ͻ���Ʒ
	endif	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;50~60��
	strvar Ret3259 Ret3260 Ret3261 Ret3262 Ret3263 Ret3264 Ret3265 Ret3266 Ret3267 Ret3268
	task_stat 3259 1 Ret3259
	task_stat 3260 1 Ret3260
	task_stat 3261 1 Ret3261
	task_stat 3262 1 Ret3262
	task_stat 3263 1 Ret3263
	task_stat 3264 1 Ret3264
	task_stat 3265 1 Ret3265
	task_stat 3266 1 Ret3266
	task_stat 3267 1 Ret3267
	task_stat 3268 1 Ret3268
	if Ret3259 = 2 or Ret3260 = 2 or Ret3261 = 2 or Ret3262 = 2 or Ret3263 = 2 or Ret3264 = 2 or Ret3265 = 2 or Ret3266 = 2 or Ret3267 = 2 or Ret3268 = 2 
		option 146 #��������-�Ͻ���Ʒ
	endif	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;60~70��
	strvar Ret3569 Ret3570 Ret3571 Ret3572 Ret3573 Ret3574 Ret3575 Ret3576 Ret3577 Ret3578
	task_stat 3569 1 Ret3569
	task_stat 3570 1 Ret3570
	task_stat 3571 1 Ret3571
	task_stat 3572 1 Ret3572
	task_stat 3573 1 Ret3573
	task_stat 3574 1 Ret3574
	task_stat 3575 1 Ret3575
	task_stat 3576 1 Ret3576
	task_stat 3577 1 Ret3577
	task_stat 3578 1 Ret3578
	if Ret3569 = 2 or Ret3570 = 2 or Ret3571 = 2 or Ret3572 = 2 or Ret3573 = 2 or Ret3574 = 2 or Ret3575 = 2 or Ret3576 = 2 or Ret3577 = 2 or Ret3578 = 2 
		option 156 #��������-�Ͻ���Ʒ
	endif	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;70~80��
	strvar Ret3617 Ret3618 Ret3619 Ret3620 Ret3621 Ret3622 Ret3623 Ret3624 Ret3625 Ret3626
	task_stat 3617 1 Ret3617
	task_stat 3618 1 Ret3618
	task_stat 3619 1 Ret3619
	task_stat 3620 1 Ret3620
	task_stat 3621 1 Ret3621
	task_stat 3622 1 Ret3622
	task_stat 3623 1 Ret3623
	task_stat 3624 1 Ret3624
	task_stat 3625 1 Ret3625
	task_stat 3626 1 Ret3626
	if Ret3617 = 2 or Ret3618 = 2 or Ret3619 = 2 or Ret3620 = 2 or Ret3621 = 2 or Ret3622 = 2 or Ret3623 = 2 or Ret3624 = 2 or Ret3625 = 2 or Ret3626 = 2 
		option 166 #��������-�Ͻ���Ʒ
	endif	
	
	if Ret = 0
	
		option 121 #����Ƽ�����	
	endif
;	if RetC = 0 and Ret != 0
;		option 125 #����ɼ�����
;	endif
	if RetG = 0 and Ret != 0
		option 124 #���蹤��
	endif
	
;	if RetCh < 2
;		option 122 #��������
;	endif

	strvar judian kaiqi zhuangtai moshi shijian
	
	GET_EVENT_STAT shijian 64
	GET_MACHINE_PWORLD_STATUS zhuangtai moshi
	
	STRONG_ACTIVE_LEVEL judian
	IS_EXIST_MACHINE_PWORLD_UINTE kaiqi 0
	
	if shijian = 1 and zhuangtai = 0
		option 300 #���ػ�������Ŧ-��ͨģʽ����δ������
		if judian > 0
			option 500 #���ػ�������Ŧ-����ģʽ����δ������
		endif
	endif
	
	if kaiqi = 1
		if zhuangtai = 1 and moshi = 1
			option 450 #���ػ�������Ŧ-��ͨģʽ����δ������
			option 500 #���ػ�������Ŧ-����ģʽ������������У�
		else
			option 500 #���ػ�������Ŧ-����ģʽ�����������У�
		endif
	endif
	if kaiqi = 1
		if zhuangtai = 1 and moshi != 1
			option 300 #���ػ�������Ŧ-��ͨģʽ���������У�
			if judian > 0
				option 451 #���ػ�������Ŧ-����ģʽ����δ������
			endif
		endif
	endif
	
	if shijian = 1 and zhuangtai = 2
		option 452 #���ػ�������Ŧ-��ͨģʽ�����ѽ�����
		if judian > 0
			option 453 #���ػ�������Ŧ-����ģʽ�����ѽ�����
		endif
	endif
	
;	option 120 #�ص�ʱ���
	endif
;	option 210 ������Ϣ��ѯ
	talk
endproc 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
proc 210
	
	EFFECT 2 1 SetMessageTips(1)
endproc 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
proc 136
	exec ghtask40_50.mac 100
endproc 
proc 137
	exec ghtask40_50.mac 111
endproc 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;50~60
proc 146
	exec ghtask50_60.mac 150
endproc 
proc 147
	exec ghtask50_60.mac 151
endproc 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;60~70
proc 156
	exec ghtask60_70.mac 160
endproc
proc 157
	exec ghtask60_70.mac 161
endproc
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;70~80
proc 166
	exec ghtask70_80.mac 170
endproc
proc 167
	exec ghtask70_80.mac 171
endproc
proc 120
	TRANSMIT 1000 25200 39000 1
endproc 
proc 121
	open_ui 21
endproc
proc 122
	open_ui 18
endproc
proc 125
	open_ui 22
endproc	
proc 124
	open_ui 23
endproc
PROC 126
	TASKCYCLE_DO_OPTION 110 1
ENDPROC
PROC 160
	TASKCYCLE_DO_OPTION 119 1
ENDPROC
PROC 161
	TASKCYCLE_DO_OPTION 120 1
ENDPROC
PROC 162
	TASKCYCLE_DO_OPTION 112 1
ENDPROC
PROC 163
	TASKCYCLE_DO_OPTION 113 1
ENDPROC
PROC 164
	TASKCYCLE_DO_OPTION 114 1
ENDPROC
proc 135
	dialog 3
	add_talk $_npc$:
	add_talk #b#b�ռ������Ķ��������������ӡ�
	add_talk #b#b
	strvar Ret3227 Ret3228 Ret3229 Ret3230 Ret3231 Ret3232 Ret3233 Ret3234 Ret3235 Ret3236
	
	task_stat 3227 1 Ret3227
	task_stat 3228 1 Ret3228
	task_stat 3229 1 Ret3229
	task_stat 3230 1 Ret3230
	task_stat 3231 1 Ret3231
	task_stat 3232 1 Ret3232
	task_stat 3233 1 Ret3233
	task_stat 3234 1 Ret3234
	task_stat 3235 1 Ret3235
	task_stat 3236 1 Ret3236
			        
	if Ret3227 = 2
		add_talk #b#b#c186506��Ҫ�ռ�30��~40���׶ε���ɫƷ��������Ʒ#n
	endif
	if Ret3228 = 2
		add_talk #b#b#c186506��Ҫ�ռ�30��~40���׶ε���ɫƷ�ʸ�����Ʒ#n
	endif
	if Ret3229 = 2
		add_talk #b#b#c186506��Ҫ�ռ�30��~40���׶ε���ɫƷ��ͷ��#n
	endif
	if Ret3230 = 2
		add_talk #b#b#c186506��Ҫ�ռ�30��~40���׶ε���ɫƷ�ʼ��#n
	endif
	if Ret3231 = 2
		add_talk #b#b#c186506��Ҫ�ռ�30��~40���׶ε���ɫƷ������#n
	endif
	if Ret3232 = 2
		add_talk #b#b#c186506��Ҫ�ռ�30��~40���׶ε���ɫƷ���ؼ�#n
	endif	
	if Ret3233 = 2
		add_talk #b#b#c186506��Ҫ�ռ�30��~40���׶ε���ɫƷ��սѥ#n
	endif
	if Ret3234 = 2
		add_talk #b#b#c186506��Ҫ�ռ�30��~40���׶ε���ɫƷ������#n
	endif
	if Ret3235 = 2
		add_talk #b#b#c186506��Ҫ�ռ�30��~40���׶ε���ɫƷ�ʽ�ָ#n
	endif
	if Ret3236 = 2
		add_talk #b#b#c186506��Ҫ�ռ�30��~40���׶ε���ɫƷ�ʻ���#n
	endif
	
	add_talk #b#b
	add_talk #b#b#c186506���������Ҫ�Ͻ�����Ʒ�ŵ��·����۴���#n
	add_talk #b#b#c186506<�Ҽ������������ƷҲ��ֱ�ӷ���>#n
	
		NPC_COMMIT_OPT 200
	talk
endproc 
proc 200	
		strvar m_ID m_Pos1 m_Pos2 m_Col Ret RetTask
		GET_COMMIT_ITEM_INFO 1 m_ID
		GET_COMMIT_ITEM_INFO 2 m_Pos1
		GET_COMMIT_ITEM_INFO 3 m_Pos2
		GET_COMMIT_ITEM_INFO 4 m_Col
		if m_ID = -1
			add_sys_msg_start
			add_sys_msg �㻹û���ύ��Ʒ��
			set_sys_msg_mode 1 0 0
			msg_info		
			return 
		endif
		
		task_stat 3227 1 Ret
		if Ret = 2
			if m_ID = 1013004 or m_ID = 1253004 or m_ID = 1203004 or m_ID = 1103004 or m_ID = 1033004 or m_ID = 1503004
				switch m_ID
					case 1013004
						DEC_ITEM_NUM_BYIDX 1013004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3227 1
						call 130
					endcase
					case 1253004
						DEC_ITEM_NUM_BYIDX 1253004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3227 1
						call 130
					endcase
					case 1203004
						DEC_ITEM_NUM_BYIDX 1203004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3227 1
						call 130
					endcase
					case 1103004
						DEC_ITEM_NUM_BYIDX 1103004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3227 1
						call 130
					endcase
					case 1033004
						DEC_ITEM_NUM_BYIDX 1033004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3227 1
						call 130
					endcase
					case 1503004
						DEC_ITEM_NUM_BYIDX 1503004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3227 1
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
		task_stat 3228 1 Ret
		if Ret = 2
			if m_ID = 1583004 or m_ID = 1283004 or m_ID = 1183004 or m_ID = 1063004 
				switch m_ID
					case 1583004
						DEC_ITEM_NUM_BYIDX 1583004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3228 1
						call 130
					endcase
					case 1283004
						DEC_ITEM_NUM_BYIDX 1283004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3228 1
						call 130
					endcase
					case 1183004
						DEC_ITEM_NUM_BYIDX 1183004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3228 1
						call 130
					endcase
					case 1063004
						DEC_ITEM_NUM_BYIDX 1063004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3228 1
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
		task_stat 3229 1 Ret
		if Ret = 2
			if m_ID = 2133004 or m_ID = 2143004 or m_ID = 2123004 or m_ID = 2113004 
				switch m_ID
					case 2133004
						DEC_ITEM_NUM_BYIDX 2133004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3229 1
						call 130
					endcase
					case 2143004
						DEC_ITEM_NUM_BYIDX 2143004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3229 1
						call 130
					endcase
					case 2123004
						DEC_ITEM_NUM_BYIDX 2123004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3229 1
						call 130
					endcase
					case 2113004
						DEC_ITEM_NUM_BYIDX 2113004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3229 1
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
		task_stat 3230 1 Ret
		if Ret = 2
			if m_ID = 2243004 or m_ID = 2223004 or m_ID = 2213004 or m_ID = 1273004 
				switch m_ID
					case 2243004
						DEC_ITEM_NUM_BYIDX 2243004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3230 1
						call 130
					endcase
					case 2223004
						DEC_ITEM_NUM_BYIDX 2223004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3230 1
						call 130
					endcase
					case 2213004
						DEC_ITEM_NUM_BYIDX 2213004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3230 1
						call 130
					endcase
					case 1273004
						DEC_ITEM_NUM_BYIDX 1273004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3230 1
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
		task_stat 3231 1 Ret
		if Ret = 2
			if m_ID = 2423004 or m_ID = 2433004 or m_ID = 2443004 or m_ID = 2413004 
				switch m_ID
					case 2423004
						DEC_ITEM_NUM_BYIDX 2423004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3231 1
						call 130
					endcase
					case 2433004
						DEC_ITEM_NUM_BYIDX 2433004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3231 1
						call 130
					endcase
					case 2443004
						DEC_ITEM_NUM_BYIDX 2443004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3231 1
						call 130
					endcase
					case 2413004
						DEC_ITEM_NUM_BYIDX 2413004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3231 1
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
		task_stat 3232 1 Ret
		if Ret = 2
			if m_ID = 2013004 or m_ID = 2023004 or m_ID = 2033004 or m_ID = 2043004 
				switch m_ID
					case 2013004
						DEC_ITEM_NUM_BYIDX 2013004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3232 1
						call 130
					endcase
					case 2023004
						DEC_ITEM_NUM_BYIDX 2023004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3232 1
						call 130
					endcase
					case 2033004
						DEC_ITEM_NUM_BYIDX 2033004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3232 1
						call 130
					endcase
					case 2043004
						DEC_ITEM_NUM_BYIDX 2043004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3232 1
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
		task_stat 3233 1 Ret
		if Ret = 2
			if m_ID = 2613004 or m_ID = 2633004 or m_ID = 2643004 or m_ID = 2623004 
				switch m_ID
					case 2613004
						DEC_ITEM_NUM_BYIDX 2613004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3233 1
						call 130
					endcase
					case 2633004
						DEC_ITEM_NUM_BYIDX 2633004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3233 1
						call 130
					endcase
					case 2643004
						DEC_ITEM_NUM_BYIDX 2643004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3233 1
						call 130
					endcase
					case 2623004
						DEC_ITEM_NUM_BYIDX 2623004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3233 1
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
		task_stat 3234 1 Ret
		if Ret = 2
		
			if m_ID = 2813004 or m_ID = 2833004 or m_ID = 2843004 or m_ID = 2823004 
				switch m_ID
					case 2813004
						DEC_ITEM_NUM_BYIDX 2813004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3234 1
						call 130
					endcase
					case 2833004
						DEC_ITEM_NUM_BYIDX 2833004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3234 1
						call 130
					endcase
					case 2843004
						DEC_ITEM_NUM_BYIDX 2843004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3234 1
						call 130
					endcase
					case 2823004
						DEC_ITEM_NUM_BYIDX 2823004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3234 1
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
		task_stat 3235 1 Ret
		if Ret = 2
			if m_ID = 2933004 or m_ID = 2923004 or m_ID = 2913004 or m_ID = 2943004 
				switch m_ID
					case 2933004
						DEC_ITEM_NUM_BYIDX 2933004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3235 1
						call 130
					endcase
					case 2923004
						DEC_ITEM_NUM_BYIDX 2923004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3235 1
						call 130
					endcase
					case 2913004
						DEC_ITEM_NUM_BYIDX 2913004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3235 1
						call 130
					endcase
					case 2943004
						DEC_ITEM_NUM_BYIDX 2943004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3235 1
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
		task_stat 3236 1 Ret
		if Ret = 2
			if m_ID = 2713004 or m_ID = 2723004 or m_ID = 2733004 or m_ID = 2743004 
				switch m_ID
					case 2713004
						DEC_ITEM_NUM_BYIDX 2713004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3236 1
						call 130
					endcase
					case 2723004
						DEC_ITEM_NUM_BYIDX 2723004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3236 1
						call 130
					endcase
					case 2733004
						DEC_ITEM_NUM_BYIDX 2733004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3236 1
						call 130
					endcase
					case 2743004
						DEC_ITEM_NUM_BYIDX 2743004 1 @m_Pos1 @m_Pos2 RetTask
						task_fini 3236 1
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
proc 130
	add_sys_msg_start
	add_sys_msg �ɹ��Ͻ���Ʒ���������
	set_sys_msg_mode 1 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc 
proc 300
	strvar ret ret2 ret3 pos time ope
	GET_EVENT_STAT ret 64
	GET_MACHINE_PWORLD_STATUS ret2 0
	get_now_time 1 1 1 1 time 1 1
	IS_CLAN_MACHINE_PWORLD_OPEN ope
	
	strvar judian
	STRONG_ACTIVE_LEVEL judian
	
	DIALOG 1
;	add_talk $_npc$:
	add_talk ����ͨģʽ������:
	add_talk #b#b1.�з�Ϯ��һ����Ϊ30��
	add_talk #b#b2.������Ŧ���з����ٻʧ��
	add_talk #b#b3.��л�ɱ�б��������˿ɻ�þ���
	if ret = 1 and ret2 = 0
		add_talk #b#b
		if judian > 0
			add_talk #c186506����Ҫ��δ�������ػ�������Ŧ-����ģʽ��
			add_talk �μ�Ҫ��δ�μ��κ�ģʽ�ġ��ػ�������Ŧ��#n
		endif
		add_talk #c186506�ʱ�䣺ÿ����1��(06:00-24:00)
		add_talk �᳤������06:00-22:00
		add_talk ��Ա������22:00-24:00��ǰ�᣺�᳤����δ������
		add_talk ���չ��δ����#n
		add_talk #b#b
		add_talk #b#b#c005ebeʹ�û�е�ɶԵз���ɽϸߵ��˺�
		add_talk #b#b��Ҫ����Э�����ܳɹ��������е���#n

		if time >= 6 and time < 24
			if ope = 0
				option 301 #�����ټ���Ա���������ػ�������Ŧ-��ͨģʽ��
			endif
		endif
	elseif ret2 = 1
		add_talk #b#b
		if judian > 0
			add_talk #c186506�μ�Ҫ��δ�μ��κ�ģʽ�ġ��ػ�������Ŧ��#n
		endif
		add_talk #c186506�ʱ�䣺ÿ����1��(06:00-24:00)
		add_talk ���չ��������#n
		add_talk #b#b
		add_talk #b#b#c005ebeʹ�û�е�ɶԵз���ɽϸߵ��˺�
		add_talk #b#b��Ҫ����Э�����ܳɹ��������е���#n
			
		if ope = 1
			option 400 #����ǰ����������Ŧ����-��ͨģʽ��
		endif
	else
	endif
	
	option 1 #����
	talk
endproc
proc 400
	strvar get ret2 pos time

	GET_MACHINE_PWORLD_STATUS get 0
	player_clan_pos_info ret2 0 0 0 pos
	get_now_time 1 1 1 1 time 1 1

	if  get = 2
		add_sys_msg_start
		add_sys_msg ���ػ�������Ŧ-��ͨģʽ���ѽ����������޷��ٲμ�
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time < 6
		add_sys_msg_start
		add_sys_msg ���ڿ���ʱ���ڣ��޷��μӡ��ػ�������Ŧ-��ͨģʽ�� 
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time >= 6 and time < 24
		if ret2 = 0 and pos > 0
			ENTER_CLAN_MACHINE_PWORLD 1
		else
			add_sys_msg_start
			add_sys_msg �ǹ����Ա�޷��μӡ��ػ�������Ŧ-��ͨģʽ��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc
proc 301
	strvar Ret RetC RetG RetCh ZRet
	GET_CLAN_BUILDING_LV Ret 0 0
	GET_CLAN_BUILDING_LV RetG 0 2
	
	if Ret < 1 
		add_sys_msg_start
		add_sys_msg ����Ƽ����Ļ򹫻Ṥ����δ���죬�޷������
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	if Ret >= 100
		add_sys_msg_start
		add_sys_msg ����Ƽ����Ļ򹫻Ṥ����δ���죬�޷������
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif 
	if RetG < 1 
		add_sys_msg_start
		add_sys_msg ����Ƽ����Ļ򹫻Ṥ����δ���죬�޷������
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	if RetG >= 100
		add_sys_msg_start
		add_sys_msg ����Ƽ����Ļ򹫻Ṥ����δ���죬�޷������ 
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif 
	
	strvar get ret2 pos time

	GET_MACHINE_PWORLD_STATUS get 0
	player_clan_pos_info ret2 0 0 0 pos
	get_now_time 1 1 1 1 time 1 1

	if  get = 1
		add_sys_msg_start
		add_sys_msg ���ػ�������Ŧ-��ͨģʽ���ѿ����������޷��ٿ���
		set_sys_msg_mode 4 0 0
		msg_info
	elseif  get = 2
		add_sys_msg_start
		add_sys_msg ���ػ�������Ŧ-��ͨģʽ���ѽ����������޷��ٿ���
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time < 6
		add_sys_msg_start
		add_sys_msg ���ڿ���ʱ���ڣ��޷��������ػ�������Ŧ-��ͨģʽ��
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time >= 6 and time < 22
		if ret2 = 0 and ( pos = 7 or pos = 8 )
			choose 2 302 303 �Ƿ�ȷ�Ͽ������ػ�������Ŧ-��ͨģʽ����
		else
			add_sys_msg_start
			add_sys_msg 22:00��᳤��δ��������Ա�����п������ػ�������Ŧ-��ͨģʽ��
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time >= 22 and time < 24
		if ret2 = 0 and pos > 0
			choose 2 302 303 �Ƿ�ȷ�Ͽ������ػ�������Ŧ-��ͨģʽ����
		else
			add_sys_msg_start
			add_sys_msg �ǹ����Ա�޷��������ػ�������Ŧ-��ͨģʽ��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc
proc 302
	strvar Ret RetC RetG RetCh ZRet
	GET_CLAN_BUILDING_LV Ret 0 0
	GET_CLAN_BUILDING_LV RetG 0 2
	
	if Ret < 1 
		add_sys_msg_start
		add_sys_msg ����Ƽ����Ļ򹫻Ṥ����δ���죬�޷������
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	if Ret >= 100
		add_sys_msg_start
		add_sys_msg ����Ƽ����Ļ򹫻Ṥ����δ���죬�޷������  
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif 
	if RetG < 1 
		add_sys_msg_start
		add_sys_msg ����Ƽ����Ļ򹫻Ṥ����δ���죬�޷������  
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	if RetG >= 100
		add_sys_msg_start
		add_sys_msg ����Ƽ����Ļ򹫻Ṥ����δ���죬�޷������  
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	strvar get ret2 pos time ret3

	GET_MACHINE_PWORLD_STATUS get 0
	player_clan_pos_info ret2 0 0 0 pos
	get_now_time 1 1 1 1 time 1 1

	if get = 1
		add_sys_msg_start
		add_sys_msg ���ػ�������Ŧ-��ͨģʽ���ѿ����������޷��ٿ���
		set_sys_msg_mode 4 0 0
		msg_info
	elseif get = 2
		add_sys_msg_start
		add_sys_msg ���ػ�������Ŧ-��ͨģʽ���ѽ����������޷��ٿ���
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time < 6
		add_sys_msg_start
		add_sys_msg ���ڿ���ʱ���ڣ��޷��������ػ�������Ŧ-��ͨģʽ��
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time >= 6 and time < 22
		if ret2 = 0 and ( pos = 7 or pos = 8 )
			CLAN_MACHINE_PWORLD_CREATE ret3 0
			if ret3 = -1
				add_sys_msg_start
				add_sys_msg �ѿ��������ػ�������Ŧ-��ͨģʽ����ÿ�����ֻ�ܿ���1��  
				set_sys_msg_mode 4 0 0
				msg_info
			elseif ret3 = -2
				add_sys_msg_start
				add_sys_msg ��������ĵ�һ�ܣ��޷��������ػ�������Ŧ-��ͨģʽ��
				set_sys_msg_mode 4 0 0
				msg_info
			elseif ret3 = 0
				add_sys_msg_start
				add_sys_msg ��$_Player$�������ˡ��ػ�������Ŧ-��ͨģʽ������ͨ��#L�������@@(1000,25400,39200)#n��#L����˹����@@(60000,17700,25500)#n���͵��ػ���Ŧ����Э����ս
				set_sys_msg_mode 14 0 0
				clan_msg_info 1 0
			elseif ret3 = -3
				add_sys_msg_start
				add_sys_msg ͬʱֻ��1�����Ὺ�����ػ�������Ŧ-����ģʽ������������ٿ���
				set_sys_msg_mode 14 0 0
				clan_msg_info 1 0
			else
			endif
		else
			add_sys_msg_start
			add_sys_msg 22:00��᳤��δ��������Ա�����п������ػ�������Ŧ-��ͨģʽ��
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time >= 22 and time < 24
		if ret2 = 0 and pos > 0
			CLAN_MACHINE_PWORLD_CREATE ret3 0
			if ret3 = -1
				add_sys_msg_start
				add_sys_msg �ѿ��������ػ�������Ŧ-��ͨģʽ����ÿ�����ֻ�ܿ���1��  
				set_sys_msg_mode 4 0 0
				msg_info
			elseif ret3 = -2
				add_sys_msg_start
				add_sys_msg ��������ĵ�һ�ܣ��޷��������ػ�������Ŧ-��ͨģʽ��
				set_sys_msg_mode 4 0 0
				msg_info
			elseif ret3 = 0
				add_sys_msg_start
				add_sys_msg ��$_Player$�������ˡ��ػ�������Ŧ-��ͨģʽ������ͨ��#L�������@@(1000,25400,39200)#n��#L����˹����@@(60000,17700,25500)#n���͵��ػ���Ŧ����Э����ս
				set_sys_msg_mode 14 0 0
				clan_msg_info 1 0
			elseif ret3 = -3
				add_sys_msg_start
				add_sys_msg ͬʱֻ��1�����Ὺ�����ػ�������Ŧ-����ģʽ�������������ٿ���
				set_sys_msg_mode 14 0 0
				clan_msg_info 1 0
			else
			endif
		else
			add_sys_msg_start
			add_sys_msg �ǹ����Ա�޷��������ػ�������Ŧ-��ͨģʽ��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc

proc 303
endproc

proc 500
	strvar time kaiqi yinbi
	get_now_time 1 1 1 1 time 1 1
	IS_EXIST_MACHINE_PWORLD_UINTE kaiqi 0
	
	DIALOG 1
;	add_talk $_npc$:
	add_talk ������ģʽ������:
	add_talk #b#b1.�з�Ϯ��һ����Ϊ30��
	add_talk #b#b2.������Ŧ���з����ٻʧ��
	add_talk #b#b3.��л�ɱ�б��������˿ɻ�þ���
	add_talk #c005ebe#b#b4.��ɱ�б���þ���ֵ����10%
	add_talk #b#b5.�κι���ĳ�Ա���ɲ���
	add_talk #b#b6.�����Ĺ���ɻ��20000�����ʽ�#n
	
	if kaiqi != 1
		add_talk #b#b
		add_talk #c186506����Ҫ��δ�������ػ�������Ŧ-��ͨģʽ��
		add_talk �μ�Ҫ��δ�μ��κ�ģʽ�ġ��ػ�������Ŧ��
		add_talk �ʱ�䣺ÿ����1��(06:00-24:00)
		add_talk �᳤������06:00-22:00
		add_talk ��Ա������22:00-24:00��ǰ�᣺�᳤����δ������
		add_talk ���չ��δ����#n
		
		GET_ID_PLAYER_INFO 0 14 yinbi
		if yinbi < 50000
			add_talk #c186506�������ã�#n#cF80B0B $@yinbi$ #n#c186506/50000����#n
		else
			add_talk #c186506�������ã�$@yinbi$ /50000����#n			
		endif
			
		if time >= 6 and time < 24
			option 501 #�����ټ���Ա���������ػ�������Ŧ-����ģʽ��
		endif
	elseif kaiqi = 1
		add_talk #b#b
		add_talk #c186506�μ�Ҫ��δ�μ��κ�ģʽ�ġ��ػ�������Ŧ��
		add_talk �ʱ�䣺ÿ����1��(06:00-24:00)
		add_talk ���չ��������#n
		
		option 600 #����ǰ����������Ŧ����-����ģʽ��
	else
	endif
	
	option 1 #����
	talk
endproc

proc 600
	strvar ret2 pos time

	player_clan_pos_info ret2 0 0 0 pos
	get_now_time 1 1 1 1 time 1 1

	if time < 6
		add_sys_msg_start
		add_sys_msg ����ʱ���ڣ��޷��μӡ��ػ�������Ŧ-����ģʽ��
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time >= 6 and time < 24
		if ret2 = 0 and pos > 0
			ENTER_CLAN_MACHINE_PWORLD_UNITE 0
		else
			add_sys_msg_start
			add_sys_msg �ǹ����Ա�޷��μӡ��ػ�������Ŧ-����ģʽ��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc

proc 501
	strvar Ret RetG
	GET_CLAN_BUILDING_LV Ret 0 0
	GET_CLAN_BUILDING_LV RetG 0 2
	
	if Ret < 1 
		add_sys_msg_start
		add_sys_msg ����Ƽ����Ļ򹫻Ṥ����δ���죬�޷������
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	if Ret >= 100
		add_sys_msg_start
		add_sys_msg ����Ƽ����Ļ򹫻Ṥ����δ���죬�޷������
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif 
	if RetG < 1 
		add_sys_msg_start
		add_sys_msg ����Ƽ����Ļ򹫻Ṥ����δ���죬�޷������
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	if RetG >= 100
		add_sys_msg_start
		add_sys_msg ����Ƽ����Ļ򹫻Ṥ����δ���죬�޷������ 
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif 
	
	strvar get ret2 pos time fanrong
	GET_MACHINE_PWORLD_STATUS get 0
	player_clan_pos_info ret2 0 0 0 pos
	get_now_time 1 1 1 1 time 1 1
	RANGE_CLAN_FANRONG fanrong 0

	if fanrong > 6
		add_sys_msg_start
		add_sys_msg ���ٶ�����ǰ6�Ĺ���ɿ������ػ�������Ŧ-����ģʽ��
		set_sys_msg_mode 4 0 0
		msg_info
	elseif  get = 1
		add_sys_msg_start
		add_sys_msg ���ػ�������Ŧ-����ģʽ���ѿ����������޷��ٿ���
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time < 6
		add_sys_msg_start
		add_sys_msg ���ڿ���ʱ���ڣ��޷��������ػ�������Ŧ-����ģʽ��
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time >= 6 and time < 22
		if ret2 = 0 and ( pos = 7 or pos = 8 )
			choose 2 502 503 �������ػ�������Ŧ-����ģʽ����Ҫ֧��50000����  �Ƿ�ȷ�Ͽ�����
		else
			add_sys_msg_start
			add_sys_msg 22:00��᳤��δ��������Ա�����п������ػ�������Ŧ-����ģʽ��
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time >= 22 and time < 24
		if ret2 = 0 and pos > 0
			choose 2 502 503 �������ػ�������Ŧ-����ģʽ����Ҫ֧��50000����  �Ƿ�ȷ�Ͽ�����
		else
			add_sys_msg_start
			add_sys_msg �ǹ����Ա�޷��������ػ�������Ŧ-����ģʽ��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc
proc 502
	strvar Ret RetG
	GET_CLAN_BUILDING_LV Ret 0 0
	GET_CLAN_BUILDING_LV RetG 0 2
	
	if Ret < 1 
		add_sys_msg_start
		add_sys_msg ����Ƽ����Ļ򹫻Ṥ����δ���죬�޷������
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	if Ret >= 100
		add_sys_msg_start
		add_sys_msg ����Ƽ����Ļ򹫻Ṥ����δ���죬�޷������  
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif 
	if RetG < 1 
		add_sys_msg_start
		add_sys_msg ����Ƽ����Ļ򹫻Ṥ����δ���죬�޷������  
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	if RetG >= 100
		add_sys_msg_start
		add_sys_msg ����Ƽ����Ļ򹫻Ṥ����δ���죬�޷������  
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	strvar get ret2 pos time ret3 yinbi biaoji fanrong

	GET_MACHINE_PWORLD_STATUS get 0
	player_clan_pos_info ret2 0 0 0 pos
	get_now_time 1 1 1 1 time 1 1
	GET_ID_PLAYER_INFO 0 14 yinbi
	RANGE_CLAN_FANRONG fanrong 0

	if fanrong > 6
		add_sys_msg_start
		add_sys_msg ���ٶ�����ǰ6�Ĺ���ɿ������ػ�������Ŧ-����ģʽ��
		set_sys_msg_mode 4 0 0
		msg_info
	elseif get = 1
		add_sys_msg_start
		add_sys_msg ���ػ�������Ŧ-����ģʽ���ѿ����������޷��ٿ���
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time < 6
		add_sys_msg_start
		add_sys_msg ���ڿ���ʱ���ڣ��޷��������ػ�������Ŧ-����ģʽ��
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time >= 6 and time < 22
		if ret2 = 0 and ( pos = 7 or pos = 8 )
			if yinbi >= 50000
				CLAN_MACHINE_PWORLD_CREATE ret3 1
				if ret3 = -1
					add_sys_msg_start
					add_sys_msg �ѿ��������ػ�������Ŧ-����ģʽ����ÿ�����ֻ�ܿ���1��  
					set_sys_msg_mode 4 0 0
					msg_info
				elseif ret3 = -2
					add_sys_msg_start
					add_sys_msg ��������ĵ�һ�ܣ��޷��������ػ�������Ŧ-����ģʽ��
					set_sys_msg_mode 4 0 0
					msg_info
				elseif ret3 = 0
					DEC_MONEY 0 50000 20 biaoji
					if biaoji = 0
						ADD_ID_CLAN_MONEY 0 0 20000 1 0
					endif

					
					add_sys_msg_start
					add_sys_msg ��$_Player$�������ˡ��ػ�������Ŧ-����ģʽ������ͨ��#L�������@@(1000,25400,39200)#n��#L����˹����@@(60000,17700,25500)#n���͵��ػ���Ŧ����Э����ս
					set_sys_msg_mode 14 0 0
					clan_msg_info 1 0
				elseif ret3 = -3
					add_sys_msg_start
					add_sys_msg ͬʱֻ��1�����Ὺ�����ػ�������Ŧ-����ģʽ������������ٿ���
					set_sys_msg_mode 14 0 0
					clan_msg_info 1 0
				else
				endif
			else
				add_sys_msg_start
				add_sys_msg �������ػ�������Ŧ-����ģʽ����Ҫ֧��50000����
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg 22:00��᳤��δ��������Ա�����п������ػ�������Ŧ-����ģʽ��
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time >= 22 and time < 24
		if ret2 = 0 and pos > 0
			if yinbi >= 50000
				CLAN_MACHINE_PWORLD_CREATE ret3 1
				if ret3 = -1
					add_sys_msg_start
					add_sys_msg �ѿ��������ػ�������Ŧ-����ģʽ����ÿ�����ֻ�ܿ���1��  
					set_sys_msg_mode 4 0 0
					msg_info
				elseif ret3 = -2
					add_sys_msg_start
					add_sys_msg ��������ĵ�һ�ܣ��޷��������ػ�������Ŧ-����ģʽ��
					set_sys_msg_mode 4 0 0
					msg_info
				elseif ret3 = 0
					DEC_MONEY 0 50000 20 biaoji
					if biaoji = 0
						ADD_ID_CLAN_MONEY 0 0 20000 0 0
					endif
					
					add_sys_msg_start
					add_sys_msg ��$_Player$�������ˡ��ػ�������Ŧ-����ģʽ������ͨ��#L�������@@(1000,25400,39200)#n��#L����˹����@@(60000,17700,25500)#n���͵��ػ���Ŧ����Э����ս
					set_sys_msg_mode 14 0 0
					clan_msg_info 1 0	
				elseif ret3 = -3
					add_sys_msg_start
					add_sys_msg ͬʱֻ��1�����Ὺ�����ػ�������Ŧ-����ģʽ������������ٿ���
					set_sys_msg_mode 14 0 0
					clan_msg_info 1 0
				else
				endif
			else
				add_sys_msg_start
				add_sys_msg �������ػ�������Ŧ-����ģʽ����Ҫ֧��50000����
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg �ǹ����Ա�޷��������ػ�������Ŧ-����ģʽ��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc

proc 450
	
	strvar judian
	STRONG_ACTIVE_LEVEL judian
	
	DIALOG 1
	add_talk ����ͨģʽ������:
	add_talk #b#b1.�з�Ϯ��һ����Ϊ30��
	add_talk #b#b2.������Ŧ���з����ٻʧ��
	add_talk #b#b3.��л�ɱ�б��������˿ɻ�þ���
	add_talk #b#b
	if judian > 0
		add_talk #c186506����Ҫ��δ�������ػ�������Ŧ-����ģʽ��#n
	endif
	add_talk #c186506�μ�Ҫ��δ�μ��κ�ģʽ�ġ��ػ�������Ŧ��
	add_talk �ʱ�䣺ÿ����1��(06:00-24:00)
	add_talk �᳤������06:00-22:00
	add_talk ��Ա������22:00-24:00��ǰ�᣺�᳤����δ������
	add_talk ���չ���ѿ������ػ�������Ŧ-����ģʽ��#n
	add_talk #b#b
	add_talk #b#b#c005ebe���δ�μ��κ�ģʽ�ġ��ػ�������Ŧ���������Կɲμӡ��ػ�������Ŧ-����ģʽ��#n
	
		option 454 #�����ټ���Ա���������ػ�������Ŧ-��ͨģʽ��
		option 1 #����
	talk
endproc

proc 451
	strvar judian
	STRONG_ACTIVE_LEVEL judian
	
	DIALOG 1
	add_talk ������ģʽ������:
	add_talk #b#b1.�з�Ϯ��һ����Ϊ30��
	add_talk #b#b2.������Ŧ���з����ٻʧ��
	add_talk #b#b3.��л�ɱ�б��������˿ɻ�þ���
	add_talk #c005ebe#b#b4.��ɱ�б���þ���ֵ����10%
	add_talk #b#b5.�κι���ĳ�Ա���ɲ���
	add_talk #b#b6.�����Ĺ���ɻ��20000�����ʽ�#n
	add_talk #b#b
	if judian > 0
		add_talk #c186506����Ҫ��δ�������ػ�������Ŧ-��ͨģʽ��#n
	endif
	add_talk #c186506�μ�Ҫ��δ�μ��κ�ģʽ�ġ��ػ�������Ŧ��
	add_talk �ʱ�䣺ÿ����1��(06:00-24:00)
	add_talk �᳤������06:00-22:00
	add_talk ��Ա������22:00-24:00��ǰ�᣺�᳤����δ������
	add_talk ���չ���ѿ������ػ�������Ŧ-��ͨģʽ��#n
	add_talk #b#b
	add_talk #b#b#c005ebe���δ�μ��κ�ģʽ�ġ��ػ�������Ŧ���������Կɲμӡ��ػ�������Ŧ-����ģʽ��#n
	
		option 455 #�����ټ���Ա���������ػ�������Ŧ-����ģʽ��
		option 1 #����
	talk
endproc

proc 452
	strvar shijian zhuangtai moshi judian
	
	GET_EVENT_STAT shijian 64
	GET_MACHINE_PWORLD_STATUS zhuangtai moshi
	STRONG_ACTIVE_LEVEL judian
	
	DIALOG 1
	add_talk ����ͨģʽ������:
	add_talk #b#b1.�з�Ϯ��һ����Ϊ30��
	add_talk #b#b2.������Ŧ���з����ٻʧ��
	add_talk #b#b3.��л�ɱ�б��������˿ɻ�þ���
	add_talk #b#b
	if judian > 0
		add_talk #c186506����Ҫ��δ�������ػ�������Ŧ-����ģʽ��#n
	endif
	add_talk #c186506�μ�Ҫ��δ�μ��κ�ģʽ�ġ��ػ�������Ŧ��
	add_talk �ʱ�䣺ÿ����1��(06:00-24:00)
	add_talk �᳤������06:00-22:00
	add_talk ��Ա������22:00-24:00��ǰ�᣺�᳤����δ������
	add_talk ���չ���ѽ���#n
	add_talk #b#b
	; if shijian = 1 and zhuangtai = 2
		; add_talk #b#b��ÿ���˵�����Э���£����ǳɹ��������޶��ػ�����Ŧ��
	; endif
	add_talk #b#b#c005ebe���δ�μ��κ�ģʽ�ġ��ػ�������Ŧ���������Կɲμӡ��ػ�������Ŧ-����ģʽ��#n
	
	option 1 #����
	talk
endproc

proc 453
	strvar shijian zhuangtai moshi judian
	
	GET_EVENT_STAT shijian 64
	GET_MACHINE_PWORLD_STATUS zhuangtai moshi
	STRONG_ACTIVE_LEVEL judian
	
	DIALOG 1
	add_talk ������ģʽ������:
	add_talk #b#b1.�з�Ϯ��һ����Ϊ30��
	add_talk #b#b2.������Ŧ���з����ٻʧ��
	add_talk #b#b3.��л�ɱ�б��������˿ɻ�þ���
	add_talk #c005ebe#b#b4.��ɱ�б���þ���ֵ����10%
	add_talk #b#b5.�κι���ĳ�Ա���ɲ���
	add_talk #b#b6.�����Ĺ���ɻ��20000�����ʽ�#n
	add_talk #b#b
	if judian > 0
		add_talk #c186506����Ҫ��δ�������ػ�������Ŧ-��ͨģʽ��#n
	endif
	add_talk #c186506�μ�Ҫ��δ�μ��κ�ģʽ�ġ��ػ�������Ŧ��
	add_talk �ʱ�䣺ÿ����1��(06:00-24:00)
	add_talk �᳤������06:00-22:00
	add_talk ��Ա������22:00-24:00��ǰ�᣺�᳤����δ������
	add_talk ���չ���ѽ���#n
	add_talk #b#b
	; if shijian = 1 and zhuangtai = 2
		; add_talk #b#b��ÿ���˵�����Э���£����ǳɹ��������޶��ػ�����Ŧ��
	; endif
	add_talk #b#b#c005ebe���δ�μ��κ�ģʽ�ġ��ػ�������Ŧ���������Կɲμӡ��ػ�������Ŧ-����ģʽ��#n
	
	option 1 #����
	talk
endproc

proc 454
	add_sys_msg_start
	add_sys_msg �����ѿ������ػ�������Ŧ-����ģʽ���������޷��ٿ������ػ�������Ŧ-��ͨģʽ��
	set_sys_msg_mode 4 0 0
	msg_info
endproc

proc 455
	add_sys_msg_start
	add_sys_msg �����ѿ������ػ�������Ŧ-��ͨģʽ���������޷��ٿ������ػ�������Ŧ-����ģʽ��
	set_sys_msg_mode 4 0 0
	msg_info
endproc
