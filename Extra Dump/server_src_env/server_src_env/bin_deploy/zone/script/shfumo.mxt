

proc 1
	strvar ret LV
	GET_PLAYER_SECONDARY_SKILL 0 6 ret LV
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�Ҿ��Ǹ�ħ��ʦ�������������ߡ�
	if ret = -1 
		add_talk #b#b
		add_talk #c186506����ﵽ30���󣬿��ڴ�ѧϰ����ħ��רҵ#n
	elseif ret = 0 and LV <= 10
		add_talk #b#b
		add_talk #c186506��������������ħʦҪ��
		add_talk 1.��ħ�ȼ��ﵽ10��
		add_talk 2.��ħ�����ȴﵽ����#n
	elseif ret = 0 and LV > 10 and LV <= 20
		add_talk #b#b
		add_talk #c186506�����������ħʦҪ��
		add_talk 1.��ħ�ȼ��ﵽ20��
		add_talk 2.��ħ�����ȴﵽ����
		add_talk 3.����ȼ��ﵽ70��#n
	else
	endif
	
	task_add_option 4011 1 101
	task_add_option 4012 1 102

	task_add_option 4026 1 103
	task_add_option 4027 1 104
	task_add_option 4028 1 105
	task_add_option 4029 1 106
	task_add_option 4030 1 107

	;task_add_option 4020 1 301


;	task_stat 4020 1 10
;	if 10 = 3
		OPTION 202 #��ħ�Ƽ�����רҵ�б�
		strvar m_Ret m_Lv
		GET_PLAYER_SECONDARY_SKILL 0 6 m_Ret m_Lv
		if m_Ret = 0	
			
		else
			OPTION 110 #��Ϊ��ϰ��ħʦ
		endif
		
		
		option 300 ���ָ�ħͼֽ��÷�ʽ
		
;	endif	

	talk
endproc 

proc 202
	OPEN_UI 42
endproc

PROC 103	
	task_do_option 4026 1
ENDPROC

PROC 104	
	task_do_option 4027 1
ENDPROC

PROC 105	
	task_do_option 4028 1
ENDPROC

PROC 106	
	task_do_option 4029 1
ENDPROC

PROC 107	
	task_do_option 4030 1
ENDPROC

;PROC 301	
;	task_do_option 4020 1
;ENDPROC

PROC 110
	task_stat 4020 1 10
	if 10 != 3
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b�����ﵽ30�����Ҳ�ѧʿ������ͨ���������ʲ��ԡ�
		talk
		return 		   
	endif
	
	strvar m_Ret m_Lv
	GET_PLAYER_SECONDARY_SKILL 0 6 m_Ret m_Lv
	if m_Ret = 0
		OPEN_UI 42
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
		if num >= 3
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b����ѧϰ�ĿƼ�����רҵ�Ѵﵽ���ޣ�Ҫѧϰ��רҵ����������һ����ѧ��רҵ��
			talk
			return 
		endif
		choose 1 201 1 �Ƿ�ȷ��ѧϰ��ħ�Ƽ�����רҵ��		
	endif
ENDPROC

proc 201
	learn_sec_skill 1 6
endproc

PROC 101
	task_do_option 4011 1
ENDPROC
PROC 102
	task_do_option 4012 1
ENDPROC


proc 2
	get_trig_task 1 2
	if 2 = 4011
		task_fini 4011 1
	endif
	if 2 = 4012
		task_fini 4012 1
	endif

endproc
proc 300
	DIALOG 1
	add_talk #c0040FFװ����ħ�䷽��÷�ʽ:#n
	add_talk #b#b#c186506��ʱ��Ǻ��������ꡢɳ���ȴ����䷽ϻ����#n
	add_talk #b#b#c186506��Ұ�⾫Ӣ������һ�����ʵ��丽ħ�䷽#n
	add_talk #b#b

		option 1 ����
	talk
endproc 

