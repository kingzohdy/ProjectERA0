

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ޣ���ã��װ������ѣ�����פʱ��ǵķֽ��ʦ�������кι�ɣ�
	add_talk #b#b#c186506����ﵽ35�����ڴ�ѧϰ[�ֽ�]�Ƽ�����רҵ#n

	task_add_option 4031 1 103	
	task_add_option 4032 1 104	
	task_add_option 4033 1 105	
	task_add_option 4034 1 106	
	task_add_option 4035 1 107

;	task_add_option 4021 1 302

;	strvar m_Ret m_Lv
;	GET_PLAYER_SECONDARY_SKILL 0 3 m_Ret m_Lv
;	if m_Ret = 0
;	task_stat 4020 1 10
;	if 10 = 3
		OPTION 202 #�ֽ�Ƽ�����רҵ�б�
		strvar m_Ret m_Lv
		GET_PLAYER_SECONDARY_SKILL 0 3 m_Ret m_Lv
		if m_Ret = 0	
			
		else
			OPTION 110 #��Ϊ��ϰ�ֽ�ʦ
		endif
;	endif	


	talk
endproc 


proc 202
	OPEN_UI 39
endproc

PROC 103	
	task_do_option 4031 1
ENDPROC
	
PROC 104	
	task_do_option 4032 1
ENDPROC
	
PROC 105	
	task_do_option 4033 1
ENDPROC
	
PROC 106	
	task_do_option 4034 1
ENDPROC
	
PROC 107	
	task_do_option 4035 1
ENDPROC

;PROC 302	
;	task_do_option 4021 1
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
	GET_PLAYER_SECONDARY_SKILL 0 3 m_Ret m_Lv
	if m_Ret = 0
		OPEN_UI 39
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
		
		choose 1 201 1 �Ƿ�ȷ��ѧϰ�ֽ�Ƽ�����רҵ��		
	endif
ENDPROC

proc 201
	learn_sec_skill 1 3
endproc




