

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ʱ��ǵ��������ˣ���Ҫ������������������κ�����Ҫ�������Ȼ���ܳ��ܹ�
	
	
	task_add_option 3137 1 104
	task_add_option 3136 1 105
	
	
	task_add_option 3141 1 106
	task_add_option 3145 1 107
	task_add_option 3150 1 108
	task_add_option 3155 1 109
	task_add_option 3160 1 111
	task_add_option 3138 1 112
	
;	OPTION 110 #(����)���̵�
	option 180 #�����������ȼ�ת��

	task_stat 4490 1 20
	if 20 = 2
		option 200 #�������ʦ��ŵ������һ������
	endif
	
	task_stat 3030 1 20
	if 20 = 2
		option 170 �밢ŵ�½�̸
	endif
	talk
endproc 

PROC 104
	task_do_option 3137 1
ENDPROC
PROC 105
	task_do_option 3136 1
ENDPROC
PROC 106
	task_do_option 3141 1
ENDPROC
PROC 107
	task_do_option 3145 1
ENDPROC
PROC 108
	task_do_option 3150 1
ENDPROC
PROC 109
	task_do_option 3155 1
ENDPROC
PROC 111
	task_do_option 3160 1
ENDPROC
PROC 112
	task_do_option 3138 1
ENDPROC


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
	task_stat 4490 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4490 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4490 1
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



PROC 110
	OPEN_NPC_SHOP
ENDPROC

proc 170
	DIALOG 1
   	ADD_TALK $_npc$       
	ADD_TALK #b#b�������ѣ�������ҵ�������Σ��ǲ��Ǳ���˫���ܵÿ�ܶࣿ
	ADD_TALK #b#bֻҪ��Ը������ң��Ҿͻ�����һƥ�����Ȼ������û�ıȣ������ٱ����ߵĿ졣���ģ����Բ��ȱ��˵Ĳ
		OPTION 171 #����֮�� 
	TALK	

endproc
proc 171
	task_fini 3030 1
endproc  

proc 2
	get_trig_task 1 2
	if 2 = 1096
		task_fini 1096 1
	endif 
	if 2 = 3138
		task_fini 3138 1
	endif

	if 2 = 3141
		task_fini 3141 1
	endif
endproc

proc 180
	OPEN_UI 74
	;������ȼ�ת�ƽ���
endproc 





