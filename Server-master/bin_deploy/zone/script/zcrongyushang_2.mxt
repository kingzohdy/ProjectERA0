
;С�̸�
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b���겻��������ս�������̵�û�����ˣ�ϣ���ı��ܸ��Ҵ��������ͲƸ���
	add_talk #b#b��ֻҪ�μӲ���� #c186506ָ�����ճ��������Ʒ��񡢱��뻻������ħ����¯����������߸���#n ����������Ϳ��Ի�����䷢�����潱�¡�
	add_talk #b#b���潱�»���Ϊһ�ֻ��Ҵ�����������潱��һҳ�У��������ڹ�����������۵ĸ��ֻ��
	
		strvar Ret num1
		GET_ITEM_NUM_INPAK 5300769 1 num1
		if num1 > 0
			TASK_STAT 3304 1 Ret
			if Ret = 2
				option 500 �����뻻����
			endif
		endif
		; option 150 #���̵�
		
		task_stat 4493 1 20
		if 20 = 2
			option 200 #���м���������ŵ������һ������
		endif
		
		task_add_option 1600 1 100
		
		task_add_option 2266 1 101
		task_add_option 2267 1 102
		task_add_option 2268 1 103
		task_add_option 2269 1 104
		
		strvar LV
		GET_PLAYER_BASE_ATT 1 1 LV 1
		
		if @LV < 50
			option 301 #(����)�����潱�¡��̵�
		endif
		if @LV >= 50 and @LV < 60
			option 302 #(����)�����潱�¡��̵�
		endif
		if @LV >= 60 and @LV < 70
			option 303 #(����)�����潱�¡��̵�
		endif
		if @LV >= 70 and @LV < 80
			option 304 #(����)�����潱�¡��̵�
		endif
		if @LV >= 80
			option 305 #(����)�����潱�¡��̵�
		endif
		
		
	talk

endproc
proc 150
	OPEN_NPC_SHOP 

endproc


proc 200
	dialog 3
	add_talk $_npc$:
	add_talk #b#b��������ĸ����з���#c186506��˼��#n��#c186506������#n
	add_talk #b#b�����û�����ӵĻ�����ȥ�����Ƕ������ɣ�
	
	NPC_COMMIT_OPT 201
	
	talk
endproc 
proc 201
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
	task_stat 4493 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4493 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012124 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4493 1
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

proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��������ڲ����һ����Ժ�Ҫ�������ҡ�

	option 501 #����

	talk
endproc
	
proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300769 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300769 1 1
			
			rand Rad 100
			if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300770 1 0 0
			endif
			if @Rad >= 80 and @Rad < 95
				ADD_ITEM_NUM_INPAK 5300771 1 0 0
			endif
			if @Rad >= 95 and @Rad < 99
				ADD_ITEM_NUM_INPAK 5300772 1 0 0
			endif
			if @Rad = 99
				ADD_ITEM_NUM_INPAK 5300773 1 0 0
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
	
 


PROC 100
	task_do_option 1600 1
ENDPROC

PROC 101
	task_do_option 2266 1
ENDPROC
PROC 102
	task_do_option 2267 1
ENDPROC
PROC 103
	task_do_option 2268 1
ENDPROC
PROC 104
	task_do_option 2269 1
ENDPROC

proc 301
	OPEN_SHOP_BY_ID 10010
endproc
proc 302
	OPEN_SHOP_BY_ID 10011
endproc
proc 303
	OPEN_SHOP_BY_ID 10012
endproc
proc 304
	OPEN_SHOP_BY_ID 10013
endproc
proc 305
	OPEN_SHOP_BY_ID 10014
endproc


