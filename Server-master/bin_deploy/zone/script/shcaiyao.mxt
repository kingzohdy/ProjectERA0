

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#Race#����������ʲô�£��ҿ��Ǻ�æ�ġ�
	add_talk #b#b����Ϊ��С���Ӿ��۸��ң��ҿ��ǲ�ҩ��ʦ�������ϰٸ�ѧ�������ҽ̵��ء�
	add_talk #b#b#c186506����ﵽ35�����ڴ�ѧϰ[��ҩ]�Ƽ�����רҵ#n

	task_add_option 4041 1 103
	task_add_option 4042 1 104
	task_add_option 4043 1 105
	task_add_option 4044 1 106
	task_add_option 4045 1 107

;	task_add_option 4023 1 304
;	task_stat 4020 1 10
;	if 10 = 3
		OPTION 202 #��ҩ�Ƽ�����רҵ�б�
		strvar m_Ret m_Lv
		GET_PLAYER_SECONDARY_SKILL 0 2 m_Ret m_Lv
		if m_Ret = 0	
			
		else
			OPTION 110 #��Ϊ��ϰ��ҩʦ
		endif
;	endif
	
		option 111 #ǰ��Ѱ���ؾ��ɼ���ҩ
;	endif	

	strvar RetTsk
	task_stat 3123 1 RetTsk
	if RetTsk = 2
		option 200 ����ʥ���Ϲ������������
	endif
	

	talk
endproc 

proc 202
	OPEN_UI 38
endproc

PROC 103	
	task_do_option 4041 1
ENDPROC

PROC 104	
	task_do_option 4042 1
ENDPROC

PROC 105	
	task_do_option 4043 1
ENDPROC

PROC 106	
	task_do_option 4044 1
ENDPROC

PROC 107	
	task_do_option 4045 1
ENDPROC
	
proc 200 
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ۺ���������ʥ�������ˣ�л������$_player$��
		option 210 #����ʥ������
	talk
endproc 
proc 210
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
			task_fini 3123 1
		endif
	endif
endproc 
	
	
	
	
;PROC 304	
;	task_do_option 4023 1
;ENDPROC

PROC 110
	task_stat 4020 1 10
	if 10 != 3
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b�����ﵽ35�����Ҳ�ѧʿ������ͨ���������ʲ��ԡ�
		talk
		return 		   
	endif
	
	strvar m_Ret m_Lv
	GET_PLAYER_SECONDARY_SKILL 0 2 m_Ret m_Lv
	if m_Ret = 0	
		OPEN_UI 38
	else
		strvar num
		;���������
		var num 0

		strvar m_Ret m_Lv
		GET_PLAYER_SECONDARY_SKILL 0 1 m_Ret m_Lv
		if m_Ret = 0
			add num 1
		endif
		GET_PLAYER_SECONDARY_SKILL 0 2 m_Ret m_Lv
		if m_Ret = 0
			add num 1
		endif
		GET_PLAYER_SECONDARY_SKILL 0 3 m_Ret m_Lv
		if m_Ret = 0
			add num 1
		endif
		GET_PLAYER_SECONDARY_SKILL 0 4 m_Ret m_Lv
		if m_Ret = 0
			add num 1
		endif
		GET_PLAYER_SECONDARY_SKILL 0 5 m_Ret m_Lv
		if m_Ret = 0
			add num 1
		endif
		GET_PLAYER_SECONDARY_SKILL 0 6 m_Ret m_Lv
		if m_Ret = 0
			add num 1
		endif
		;�������ܳ���2��
		if num >= 2
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b����ѧϰ�ĿƼ�����רҵ�Ѵﵽ���ޣ�Ҫѧϰ��רҵ����������һ����ѧ��רҵ��
			talk
			return 
		endif
		choose 1 201 1 �Ƿ�ȷ��ѧϰ��ҩ�Ƽ�����רҵ��		
	endif
ENDPROC

proc 201
	learn_sec_skill 1 2
endproc

proc 111
	transmit 4401 11000 13200 1
endproc 



