

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�٣������ˣ�����Ȥ������һ���𣡿�˭��Ų��������ǿ��ʯ��
	add_talk #b#b��С�����ˣ���Ϊ�ɿ��ʦ���ҿ������춼���ô��ж����أ�������
	add_talk #b#b#c186506����ﵽ35�����ڴ�ѧϰ[�ɿ�]�Ƽ�����רҵ#n

	task_add_option 4051 1 103
	task_add_option 4052 1 104
	task_add_option 4053 1 105
	task_add_option 4054 1 106
	task_add_option 4055 1 107

;	task_add_option 4025 1 306


	OPTION 202 #�ɿ�Ƽ�����רҵ�б�
	strvar m_Ret m_Lv
	GET_PLAYER_SECONDARY_SKILL 0 1 m_Ret m_Lv
	if m_Ret = 0	
		
	else
		OPTION 110 #��Ϊ��ϰ�ɿ�ʦ
	endif


;	strvar m_Ret m_Lv
;	GET_PLAYER_SECONDARY_SKILL 0 1 m_Ret m_Lv
;	if m_Ret = 0		
		option 111 #ǰ��Ѱ���ؾ��ڲɿ�ʯ
;	endif

	talk
endproc 



proc 202
	OPEN_UI 37
endproc
	
PROC 103	
	task_do_option 4051 1
ENDPROC

PROC 104	
	task_do_option 4052 1
ENDPROC

PROC 105	
	task_do_option 4053 1
ENDPROC

PROC 106	
	task_do_option 4054 1
ENDPROC

PROC 107	
	task_do_option 4055 1
ENDPROC

;PROC 306	
;	task_do_option 4025 1
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
	GET_PLAYER_SECONDARY_SKILL 0 1 m_Ret m_Lv
	if m_Ret = 0
		OPEN_UI 37
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
		choose 1 201 1 �Ƿ�ȷ��ѧϰ�ɿ�Ƽ�����רҵ��		
	endif
ENDPROC

proc 201
	learn_sec_skill 1 1
endproc

proc 111
	transmit 4401 11000 13200 1
endproc 




