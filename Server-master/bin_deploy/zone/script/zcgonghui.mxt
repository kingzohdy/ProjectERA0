

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bһ���˾��뱣��ʱ��ǣ��ܶ����鲻�ǿ���Ѫ�Ϳ��Եģ������ˡ����Ž�������빫��ɣ��ŶӺ�������κ�ǿ���װ���������塣
	
	strvar RetTsk
	task_stat 3122 1 RetTsk
	if RetTsk = 2
		option 200 ����ʥ���Ϲ������������
	endif
	
	strvar RetTsk
	task_stat 4489 1 20
	if 20 = 2
		option 250 #��˹�ٷ�˹����һ������
	endif
	
	strvar Ret2 SJzt
	GET_EVENT_STAT SJzt 65
	CLAN_EVERYDAY_PWORLD_STAT_GET Ret2
	
	if SJzt = 1 and Ret2 = 0
		option 160 #���������֡����δ������
	elseif  Ret2 = 1
		option 160 #���������֡�����ѿ�����
	elseif  SJzt = 1 and Ret2 = 2
		option 160 #���������֡�����ѽ�����
	else
	endif
	
	strvar Rets
	DYN_ACTIVE_STATUS_GET  1 19 Rets
	if Rets = 1
		option 500 #�μӡ����ᱦ�ء��
	endif
	
	task_stat 3508 1 701
	task_stat 3509 1 702
	task_stat 3510 1 703
	task_stat 3511 1 704
	
	if 701 = 2 or 702 = 2 or 703 = 2 or 704 = 2
		option 750 #�������� - �Ͻ���Ʒ
	endif
	
	task_add_option 1102 1 105
	task_add_option 3637 1 206
	task_add_option 3638 1 207
	task_add_option 3639 1 208
	task_add_option 3640 1 209
	task_add_option 3641 1 210
	task_add_option 3642 1 211
	task_add_option 3643 1 212
	task_add_option 3680 1 213
	
	TASKCYCLE_ADD_OPTION 108 1 120
	TASKCYCLE_ADD_OPTION 111 1 121
	TASKCYCLE_ADD_OPTION 115 1 122
	TASKCYCLE_ADD_OPTION 116 1 123
	TASKCYCLE_ADD_OPTION 117 1 124
	TASKCYCLE_ADD_OPTION 118 1 125
	
	
	TASKCYCLE_ADD_OPTION 123 1 140
	TASKCYCLE_ADD_OPTION 124 1 141
	TASKCYCLE_ADD_OPTION 125 1 142
	TASKCYCLE_ADD_OPTION 126 1 143
	TASKCYCLE_ADD_OPTION 127 1 144
	TASKCYCLE_ADD_OPTION 128 1 145
	TASKCYCLE_ADD_OPTION 129 1 146
	TASKCYCLE_ADD_OPTION 144 1 147
	
	strvar Ret1 Pos
	player_clan_pos_info Ret1 0 0 0 Pos
	
	strvar Ret
	GET_CLAN_BUILDING_LV Ret 0 5	
	if Ret = 0
		option 116 #���빫�����
	endif
	if Ret > 0 and Ret < 100
		option 117 #���빫�����
	endif
	
	if Ret1 = 0 and Pos = 0 
		option 110 ��������
	endif
	
	option 112 #�����ѯ
	
	if Ret1 = 0 and Pos != 0
		strvar lv
		GET_PLAYER_BASE_ATT 0 0 lv 1
		if lv >= 40
			option 1000 ��Ҫ������ɡ��������񡿻�������
		endif
	endif
	
	if Ret1 = 0 and Pos != 0 and Pos != 8
		option 115 �˳�����	
	endif
	
	if Ret1 = 0 and Pos = 8
		option 113 #�༭����
		option 111 #�����ɢ
	endif
	
	talk
endproc 
proc 500
		GOTO_CLAN_CITY 1 12700 29100
endproc	

PROC 105
	task_do_option 1102 1
ENDPROC
PROC 206
	task_do_option 3637 1
ENDPROC
PROC 207
	task_do_option 3638 1
ENDPROC
PROC 208
	task_do_option 3639 1
ENDPROC
PROC 209
	task_do_option 3640 1
ENDPROC
PROC 210
	task_do_option 3641 1
ENDPROC
PROC 211
	task_do_option 3642 1
ENDPROC
PROC 212
	task_do_option 3643 1
ENDPROC
PROC 213
	task_do_option 3680 1
ENDPROC
; PROC 214
	; task_do_option 3508 1
; ENDPROC
; PROC 215
	; task_do_option 3509 1
; ENDPROC
; PROC 216
	; task_do_option 3510 1
; ENDPROC
; PROC 217
	; task_do_option 3511 1
; ENDPROC

proc 116
	 open_ui 18
	 strvar Lv
	 GET_CLAN_BUILDING_LV Lv 0 5
	 if Lv = 1
		EFFECT 2 0 SetMessageTips(id)
	 
	 endif
endproc

proc 117
	enter_clan_city	1
endproc
proc 200 
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bÿһ��ʥ���Ϲ�����������������ʥ������������Ѳ����Ǻ�ͯ��
		option 201 #����ʥ������
	talk
endproc 
proc 201
	GET_ITEM_NUM_INPAK 5300844 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
	ENDIF
	if 11 >= 1
			
		dec_item_num_inpak 5300844 1 30
		if 30 != -1
			task_fini 3122 1
		endif
	endif
endproc 

PROC 120	
	TASKCYCLE_DO_OPTION 108 1
ENDPROC
PROC 121	
	TASKCYCLE_DO_OPTION 111 1
ENDPROC
PROC 122	
	TASKCYCLE_DO_OPTION 115 1
ENDPROC
PROC 123	
	TASKCYCLE_DO_OPTION 116 1
ENDPROC
PROC 124	
	TASKCYCLE_DO_OPTION 117 1
ENDPROC
PROC 125	
	TASKCYCLE_DO_OPTION 118 1
ENDPROC

PROC 140	
	TASKCYCLE_DO_OPTION 123 1
ENDPROC
PROC 141
	TASKCYCLE_DO_OPTION 124 1
ENDPROC
PROC 142	
	TASKCYCLE_DO_OPTION 125 1
ENDPROC
PROC 143	
	TASKCYCLE_DO_OPTION 126 1
ENDPROC
PROC 144	
	TASKCYCLE_DO_OPTION 127 1
ENDPROC
PROC 145	
	TASKCYCLE_DO_OPTION 128 1
ENDPROC
PROC 146	
	TASKCYCLE_DO_OPTION 129 1
ENDPROC
PROC 147	
	TASKCYCLE_DO_OPTION 144 1
ENDPROC


proc 115

	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b���˻���Լ��
	add_talk #b#b#b �˻��ȫ��������ǰʣ���#c186506���׶�#n
	add_talk #b �˻�󷵻�#c18650680��#nѧϰ���Ἴ�ܻ��ѵ�#c186506���׶�#n
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b#b�����˻�
	add_talk #b#b#b �˻�#c18650624Сʱ#n�����������κι���
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#b#b�߳��˻�
	add_talk #b#b#b#b�˻����ʱ����������κι���
	add_talk #b#b#b #c005ebe�������ɢ����Ϊ�Զ��߳��˻ᣩ#n
	
		option 300 #ȷ���˳�����
		option 1 #����

	talk
endproc 
proc 300
	choose 0 330 331 �Ƿ�ȷ���˳����᣿
endproc

PROC 110
	DIALOG 1
	add_talk ���Ứ�ѣ�
	add_talk #b#b#c18650610000����
	add_talk #b#bԭ�ʿ��150��
	add_talk #b#bԭ���͡�150��#n
	add_talk #b#b
	add_talk �������ѣ�
	add_talk �����������δ��꣬���Զ����ս�ɢ����
	add_talk #b#b[1-5]�����������#c186506����7��#n����10��
	add_talk #b#b[1]��������ʽ�#c186506����15��#n�������ά����
	add_talk #b#b[1-5]������ĳ�Ա#c186506����60��#n���κ�������

	option 332 #ȷ����������
	option 1 #����
	talk
ENDPROC

proc 111
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b��ɢ����Լ��
	add_talk #b#b �᳤���������ɢ����ȷ�Ϻ��ɢ����
	add_talk #b#b#b #c005ebe������������48-96Сʱ��ȷ�ϣ�#n
	
		option 301 #ȷ����ɢ����
		option 1 #����
	talk
endproc 

proc 330
	EXIT_CLAN
endproc
proc 332

	strvar num num1 
	GET_ITEM_NUM_INPAK 5101001 1 num
	GET_ITEM_NUM_INPAK 5102001 1 num1
	
	if num >= 150 and num1 >= 150

		; strvar ret
		; get_id_player_info 0 21 ret
		; get_id_player_info 0 22 30
		
		; if 30 < 10000 and @ret >= 10000
			; choose 1 333 331 ���������Ѵﵽÿ��������ޣ���������ʱ����ֱ���������ң��Ƿ����������
		; elseif ret < 10000
			
			; choose 1 333 331 ��������10000����������ɹ�����������ң��Ƿ����������
		; else
			call 333
		; endif
		
		
	else
		add_sys_msg_start
		add_sys_msg ��������Ĳ��ϲ��㣡
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc
proc 333
	strvar num num1 
	GET_ITEM_NUM_INPAK 5101001 1 num
	GET_ITEM_NUM_INPAK 5102001 1 num1
	if num >= 150 and num1 >= 150
		open_ui 6
	else
		add_sys_msg_start
		add_sys_msg ��������Ĳ��ϲ��㣡
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
endproc 
PROC 301
	open_ui 7
ENDPROC
PROC 112
	open_ui 8
ENDPROC
PROC 113
	open_ui 9
ENDPROC

proc 2
	get_trig_task 1 2 
	if 2 = 3676
;		ADD_ITEM_NUM_INPAK 5300873 1 1 0
;		ADD_ITEM_NUM_INPAK 5300878 1 1 0
;		ADD_ITEM_NUM_INPAK 5300879 1 1 0
;		ADD_ITEM_NUM_INPAK 5300880 1 1 0
	endif

endproc


;��������
proc 160
	strvar ret3 pos time ret ret2
	; PLAYER_CLAN_POS_INFO ret3 0 0 0 pos
	get_now_time 1 1 1 1 time 1 1
	GET_EVENT_STAT ret 65
	CLAN_EVERYDAY_PWORLD_STAT_GET ret2

	DIALOG 1
		add_talk $_npc$:
		add_talk #b#b����������һ����Դ�ḻ�ĺ��������ܽ���ұ��������Ϊ������Ҫ���������ء�����Ϣ����й¶��ĩ��׷���߶��У�Ϊ��������Դ�����Ƕ�ұ����������Ƶ���Ľ�����
	if ret = 1 and ret2 = 0
		add_talk #b#b���ڼ����ټ������ԱЭ������ĩ��׷���ߡ������Ѿ��ڵ��ϵȺ��ţ��������������ô����
		add_talk #b#b
		add_talk #c186506����Ҫ�󣺽��������2��
		add_talk �μ�Ҫ�󣺼��빫���2��
		add_talk �ʱ�䣺�ܶ������ĸ�1��(06:00-24:00)
		add_talk �᳤������06:00-22:00
		add_talk ��Ա������22:00-24:00��ǰ�᣺�᳤����δ������
		add_talk ���չ��δ����
		add_talk ����ʱ�䣺�����20����#n

			if time >= 6 and time < 24
					option 400 #�����ټ���Ա���������������֡��
			endif
	elseif ret2 = 1
			add_talk #b#b׼���ú�����Ҵ��͵�����ұ�����ɣ������Ѿ��ڵ��ϵȺ��ţ�����������Ǹ���ô����
			add_talk #b#b
			add_talk #b#b#c005ebe�����ͨ��������Ϸ��ġ��������֡���ť�һ����ֽ�������ѯ������ʣ��ʱ�䡣#n
			add_talk #b#b
			add_talk #c186506�μ�Ҫ�󣺼��빫���2��
			add_talk �ʱ�䣺�ܶ������ĸ�1��(06:00-24:00)
			add_talk ���չ��������
			add_talk ����ʱ�䣺�����20����#n

			option 401 #����ǰ��������ұ������
	elseif ret = 1 and ret2 = 2
			add_talk #b#b������ҵ�Я�ַ�ս�����������ٴλ��������ߣ��ɹ����������ǵ����ϣ�
			add_talk #b#b
			add_talk #c186506�ʱ�䣺�ܶ������ĸ�1��(06:00-24:00)
			add_talk �᳤������06:00-22:00
			add_talk ��Ա������22:00-24:00��ǰ�᣺�᳤����δ������
			add_talk ���չ���ѽ���#n
	else
	endif
	
		option 1 ����
	talk
endproc

proc 400
	strvar ret time ret2 ret3 pos time2 ret4

	CLAN_JOIN_TIME_NO_SAMEDAY ret
	GET_CLAN_CREATE_TIME 1 0 time
	is_same_day @time ret2
	player_clan_pos_info ret3 0 0 0 pos
	get_now_time 1 1 1 1 time2 1 1
	CLAN_EVERYDAY_PWORLD_STAT_GET ret4

	if  ret4 = 1
		add_sys_msg_start
		add_sys_msg ���������֡��ѿ����������޷��ٿ����
		set_sys_msg_mode 4 0 0
		msg_info
	elseif  ret4 = 2
		add_sys_msg_start
		add_sys_msg ���������֡��ѽ����������޷��ٿ����
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret = 0
		add_sys_msg_start
		add_sys_msg ���빫�ᵱ�죬�޷��������������֡��
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret2 = 0
		add_sys_msg_start
		add_sys_msg �������ᵱ�죬�޷��������������֡��
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time2 < 6
		add_sys_msg_start
		add_sys_msg ���ڻ����ʱ���ڣ��޷��������������֡�� 
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time2 >= 6 and time2 < 22
		if ret3 = 0 and ( pos = 7 or pos = 8 )
			choose 0 402 403 �Ƿ�ȷ�Ͽ������������֡����
		else
			add_sys_msg_start
			add_sys_msg 22:00��᳤��δ��������Ա�����п������������֡��
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time2 >= 22 and time2 < 24
		if ret3 = 0 and pos > 0
			choose 0 402 403 �Ƿ�ȷ�Ͽ������������֡����
		else
			add_sys_msg_start
			add_sys_msg �ǹ����Ա�޷��������������֡��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif	
endproc

proc 402
	strvar ret time ret2 ret3 pos time2 ret4

	CLAN_JOIN_TIME_NO_SAMEDAY ret
	GET_CLAN_CREATE_TIME 1 0 time
	is_same_day @time ret2
	player_clan_pos_info ret3 0 0 0 pos
	get_now_time 1 1 1 1 time2 1 1
	CLAN_EVERYDAY_PWORLD_STAT_GET ret4

	if  ret4 = 1
		add_sys_msg_start
		add_sys_msg ���������֡��ѿ����������޷��ٿ����
		set_sys_msg_mode 4 0 0
		msg_info
	elseif  ret4 = 2
		add_sys_msg_start
		add_sys_msg ���������֡��ѽ����������޷��ٿ����
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret = 0
		add_sys_msg_start
		add_sys_msg ���빫�ᵱ�죬�޷��������������֡��
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret2 = 0
		add_sys_msg_start
		add_sys_msg �������ᵱ�죬�޷��������������֡��
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time2 < 6
		add_sys_msg_start
		add_sys_msg ���ڻ����ʱ���ڣ��޷��������������֡�� 
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time2 >= 6 and time2 < 22
		if ret3 = 0 and ( pos = 7 or pos = 8 )
			CLAN_EVERYDAY_PWORLD_CREATE 0 @pos
		else
			add_sys_msg_start
			add_sys_msg 22:00��᳤��δ��������Ա�����п������������֡��
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time2 >= 22 and time2 < 24
		if ret3 = 0 and pos > 0
			CLAN_EVERYDAY_PWORLD_CREATE 0 @pos
		else
			add_sys_msg_start
			add_sys_msg �ǹ����Ա�޷��������������֡��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc

proc 401
	strvar ret ret3 pos time2 ret4

	CLAN_JOIN_TIME_NO_SAMEDAY ret
	player_clan_pos_info ret3 0 0 0 pos
	get_now_time 1 1 1 1 time2 1 1
	CLAN_EVERYDAY_PWORLD_STAT_GET ret4
	
	if  ret4 = 2
		add_sys_msg_start
		add_sys_msg ���������֡��ѽ����������޷��ٲμӻ
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret = 0
		add_sys_msg_start
		add_sys_msg ���빫�ᵱ�죬�޷��μӡ��������֡��
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time2 < 6
		add_sys_msg_start
		add_sys_msg ���ڻʱ���ڣ��޷��μӡ��������֡�� 
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time2 >= 6 and time2 < 24
		if ret3 = 0 and pos > 0
			CLAN_EVERYDAY_PWORLD_ENTER 0
		else
			add_sys_msg_start
			add_sys_msg �ǹ����Ա�޷��μӡ��������֡��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc

proc 600
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bǰЩʱ����ϰǹ����ʱ�򣬲����߻�ʧ�˴󲿷ֵ���أ���ʵ�Ͱ����Ȿ����־������������ʱ��
	option 601 #����
		
	talk
endproc
proc 601
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300775 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300775 1 1
			
			rand Rad 100
			if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300776 1 0 0
			endif
			if @Rad >= 80 and @Rad < 95
				ADD_ITEM_NUM_INPAK 5300777 1 0 0
			endif
			if @Rad >= 95 and @Rad < 99
				ADD_ITEM_NUM_INPAK 5300778 1 0 0
			endif
			if @Rad = 99
				ADD_ITEM_NUM_INPAK 5300779 1 0 0
			endif
		else
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg ������û�п��Խ�������Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc 
;���յȼ����Һ�æ����
proc 1000
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1

	if lv >= 40 and lv <= 49
		PLAYER_BUSY_UI 24 1
	endif
	if lv >= 50 and lv <= 59
		PLAYER_BUSY_UI 25 1
	endif
	if lv >= 60 and lv <= 69
		PLAYER_BUSY_UI 26 1
	endif
	if lv >= 70 and lv <= 79
		PLAYER_BUSY_UI 27 1
	endif
	if lv >= 80 and lv <= 89
		PLAYER_BUSY_UI 28 1
	endif
endproc


proc 250
	dialog 3
	add_talk $_npc$:
	add_talk #b#b��������ĸ����з���#c186506��˼��#n��#c186506������#n
	add_talk #b#b�����û�����ӵĻ�����ȥ�����Ƕ������ɣ�
	
	NPC_COMMIT_OPT 251
	
	talk
endproc 
proc 251
	strvar m_ID m_Pos1 m_Pos2 m_Col Ret RetTask
	GET_COMMIT_ITEM_INFO 1 m_ID
	if m_ID = -1
		add_sys_msg_start
		add_sys_msg �㻹û���ύ��Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info		
		return 
	endif
	
	GET_COMMIT_ITEM_INFO 2 m_Pos1
	GET_COMMIT_ITEM_INFO 3 m_Pos2
	GET_COMMIT_ITEM_INFO 4 m_Col
	task_stat 4489 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4489 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012124 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4489 1
					call 502
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


proc 502
	add_sys_msg_start
	add_sys_msg �ɹ��Ͻ���Ʒ���������
	set_sys_msg_mode 3 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc 

proc 750
	dialog 3
	add_talk $_npc$:
	add_talk #b#b�����������Ʒ�Ͻ����ң������Ƿ�������Ҫ����Ʒ��
	add_talk #b#b
	
	task_stat 3508 1 701
	task_stat 3509 1 702
	task_stat 3510 1 703
	task_stat 3511 1 704
	
	if 701 = 2
		add_talk #b#b#c186506��Ҫ�Ͻ�1���ƺŵ�ͷ��#n
	endif
	if 702 = 2
		add_talk #b#b#c186506��Ҫ�Ͻ�1�ſƼ��������䷽#n
	endif
	if 703 = 2
		add_talk #b#b#c186506��Ҫ�Ͻ�1�ſƼ��������䷽#n
	endif
	if 704 = 2
		add_talk #b#b#c186506��Ҫ�Ͻ�1�ſƼ��������䷽#n
	endif
	
	add_talk #b#b
	add_talk #b#b#c005ebe���Ͻ�����Ʒ�����·��Ŀղ���#n
		
	NPC_COMMIT_OPT 751
	talk
endproc

proc 751
	strvar itemID pos index ret
	GET_COMMIT_ITEM_INFO 1 itemID
	GET_COMMIT_ITEM_INFO 2 pos
	GET_COMMIT_ITEM_INFO 3 index

	if itemID = -1
		add_sys_msg_start
		add_sys_msg ����û���ύ��Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	task_stat 3508 1 701
	if 701 = 2
	ITEM_IS_CHENGHAO 1 @itemID
		if 1 = -1
			add_sys_msg_start
			add_sys_msg ��ֻ��Ҫ�ƺŵ�ͷ�Σ��κ�һ���ƺ�ͷ�ζ�����Ӵ��
			set_sys_msg_mode 4 0 0
			msg_info
			return
		endif
		if 1 = 0
		DEC_ITEM_NUM_BYIDX @itemID 1 @pos @index ret
			if @ret = -1
				call 753
			else
				if @ret = 0
					task_fini 3508 0
					call 752
				endif
			endif
		endif
	endif

	task_stat 3509 1 703
	if 703 = 2
	ITEM_IS_ZHUZHAO 1 @itemID
	ITEM_IS_FUMO 2 @itemID
	ITEM_IS_LIANJIN 3 @itemID
	
		if 1 = 0 or 2 = 0 or 3 = 0
			DEC_ITEM_NUM_BYIDX @itemID 1 @pos @index ret
			if @ret = -1
				call 753
			else
				if @ret = 0
					task_fini 3509 0
					call 752
				endif
			endif
		else
			add_sys_msg_start
			add_sys_msg ��ֻ��Ҫ�Ƽ��������䷽���κ�һ��Ƽ����䷽������Ӵ��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	; ITEM_IS_FUMO 2 @itemID
		; if 2 = -1
			; add_sys_msg_start
			; add_sys_msg ��ֻ��Ҫ�Ƽ��������䷽���κ�һ��Ƽ����䷽������Ӵ��
			; set_sys_msg_mode 4 0 0
			; msg_info
			; return
		; endif
		; if 2 = 0	
			; DEC_ITEM_NUM_BYIDX @itemID 1 @pos @index ret
			; if @ret = -1
				; call 753
			; else
				; if @ret = 0
					; task_fini 3509 0
					; call 752
				; endif
			; endif
		; endif
	; ITEM_IS_LIANJIN 3 @itemID
		; if 3 = -1
			; add_sys_msg_start
			; add_sys_msg ��ֻ��Ҫ�Ƽ��������䷽���κ�һ��Ƽ����䷽������Ӵ��
			; set_sys_msg_mode 4 0 0
			; msg_info
			; return
		; endif
		; if 3 = 0
			; DEC_ITEM_NUM_BYIDX @itemID 1 @pos @index ret
			; if @ret = -1
				; call 753
			; else
				; if @ret = 0
					; task_fini 3509 0
					; call 752
				; endif
			; endif
		; endif
	endif
	
	task_stat 3510 1 703
	if 703 = 2
	ITEM_IS_ZHUZHAO 1 @itemID
	ITEM_IS_FUMO 2 @itemID
	ITEM_IS_LIANJIN 3 @itemID
	
		if 1 = 0 or 2 = 0 or 3 = 0
			DEC_ITEM_NUM_BYIDX @itemID 1 @pos @index ret
			if @ret = -1
				call 753
			else
				if @ret = 0
					task_fini 3510 0
					call 752
				endif
			endif
		else
			add_sys_msg_start
			add_sys_msg ��ֻ��Ҫ�Ƽ��������䷽���κ�һ��Ƽ����䷽������Ӵ��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	; ITEM_IS_FUMO 2 @itemID
		; if 2 = -1
			; add_sys_msg_start
			; add_sys_msg ��ֻ��Ҫ�Ƽ��������䷽���κ�һ��Ƽ����䷽������Ӵ��
			; set_sys_msg_mode 4 0 0
			; msg_info
			; return
		; endif
		; if 2 = 0	
			; DEC_ITEM_NUM_BYIDX @itemID 1 @pos @index ret
			; if @ret = -1
				; call 753
			; else
				; if @ret = 0
					; task_fini 3510 0
					; call 752
				; endif
			; endif
		; endif
	; ITEM_IS_LIANJIN 3 @itemID
		; if 3 = -1
			; add_sys_msg_start
			; add_sys_msg ��ֻ��Ҫ�Ƽ��������䷽���κ�һ��Ƽ����䷽������Ӵ��
			; set_sys_msg_mode 4 0 0
			; msg_info
			; return
		; endif
		; if 3 = 0
			; DEC_ITEM_NUM_BYIDX @itemID 1 @pos @index ret
			; if @ret = -1
				; call 753
			; else
				; if @ret = 0
					; task_fini 3510 0
					; call 752
				; endif
			; endif
		; endif
	endif
	
	task_stat 3511 1 704
	if 704 = 2
	ITEM_IS_ZHUZHAO 1 @itemID
	ITEM_IS_FUMO 2 @itemID
	ITEM_IS_LIANJIN 3 @itemID
	
		if 1 = 0 or 2 = 0 or 3 = 0
			DEC_ITEM_NUM_BYIDX @itemID 1 @pos @index ret
			if @ret = -1
				call 753
			else
				if @ret = 0
					task_fini 3511 0
					call 752
				endif
			endif
		else
			add_sys_msg_start
			add_sys_msg ��ֻ��Ҫ�Ƽ��������䷽���κ�һ��Ƽ����䷽������Ӵ��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	; ITEM_IS_FUMO 2 @itemID
		; if 2 = -1
			; add_sys_msg_start
			; add_sys_msg ��ֻ��Ҫ�Ƽ��������䷽���κ�һ��Ƽ����䷽������Ӵ��
			; set_sys_msg_mode 4 0 0
			; msg_info
			; return
		; endif
		; if 2 = 0	
			; DEC_ITEM_NUM_BYIDX @itemID 1 @pos @index ret
			; if @ret = -1
				; call 753
			; else
				; if @ret = 0
					; task_fini 3511 0
					; call 752
				; endif
			; endif
		; endif
	; ITEM_IS_LIANJIN 3 @itemID
		; if 3 = -1
			; add_sys_msg_start
			; add_sys_msg ��ֻ��Ҫ�Ƽ��������䷽���κ�һ��Ƽ����䷽������Ӵ��
			; set_sys_msg_mode 4 0 0
			; msg_info
			; return
		; endif
		; if 3 = 0
			; DEC_ITEM_NUM_BYIDX @itemID 1 @pos @index ret
			; if @ret = -1
				; call 753
			; else
				; if @ret = 0
					; task_fini 3511 0
					; call 752
				; endif
			; endif
		; endif
	endif
	
endproc

proc 752
	add_sys_msg_start
	add_sys_msg �ɹ��Ͻ���Ʒ���������
	set_sys_msg_mode 3 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc

proc 753
	add_sys_msg_start
	add_sys_msg ��Ʒ����Ҫ��������δ�ܳɹ��Ͻ�
	set_sys_msg_mode 3 0 0
	msg_info
endproc
