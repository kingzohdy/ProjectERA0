

proc 1
	strvar ret LV
	GET_PLAYER_SECONDARY_SKILL 0 5 ret LV
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����Ҫѧϰ���������Ҿ�����Ҫ�ҵ��ˣ����¿���
	add_talk #b#b�ѵ��������������������̫�ţ��·����һص���18����ȶ���฿���������
	add_talk #c0040FF���ͳ���ȡ;����
	add_talk #c0040FF1. ������С��ܶ���ʹ�ù��ṱ�׶ȹ���
	add_talk #c0040FF2. �����ɼ������ɼ���Ա����ɡ���ʹ�ð�������
	add_talk #b#b#c0040FF������ɼ�������ʱ�䣺ÿ��16��00-23��00
	add_talk #b#b#c0040FF�񡰴����������������������ɼ����֤
	if ret = -1 
		add_talk #b#b
		add_talk #c186506����ﵽ30���󣬿��ڴ�ѧϰ������רҵ#n
	elseif ret = 0 and LV <= 10
		add_talk #b#b
		add_talk #c186506����������������ʦҪ��
		add_talk 1.����ȼ��ﵽ10��
		add_talk 2.���������ȴﵽ����#n
	elseif ret = 0 and LV > 10 and LV <= 20
		add_talk #b#b
		add_talk #c186506�������������ʦҪ��
		add_talk 1.����ȼ��ﵽ20��
		add_talk 2.���������ȴﵽ����
		add_talk 3.����ȼ��ﵽ70��#n
	else
	endif

	task_add_option 4036 1 103	
	task_add_option 4037 1 104	
	task_add_option 4038 1 105	
	task_add_option 4039 1 106	
	task_add_option 4040 1 107

;	task_add_option 4022 1 303

;	strvar m_Ret m_Lv
;	GET_PLAYER_SECONDARY_SKILL 0 5 m_Ret m_Lv
;	if m_Ret = 0
;	task_stat 4020 1 10
;	if 10 = 3
		OPTION 202 #����Ƽ�����רҵ�б�
		strvar m_Ret m_Lv
		GET_PLAYER_SECONDARY_SKILL 0 5 m_Ret m_Lv
		if m_Ret = 0	
			
		else
			OPTION 110 #��Ϊ��ϰ����ʦ
		endif
		
		option 300 ��������ͼֽ��÷�ʽ
;	endif	

	talk
endproc 


proc 202
	OPEN_UI 41
endproc

PROC 103	
	task_do_option 4036 1
ENDPROC

PROC 104	
	task_do_option 4037 1
ENDPROC

PROC 105	
	task_do_option 4038 1
ENDPROC

PROC 106	
	task_do_option 4039 1
ENDPROC

PROC 107	
	task_do_option 4040 1
ENDPROC
	
;PROC 303	
;	task_do_option 4022 1
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
	GET_PLAYER_SECONDARY_SKILL 0 5 m_Ret m_Lv
	if m_Ret = 0
		OPEN_UI 41
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
		choose 1 201 1 �Ƿ�ȷ��ѧϰ����Ƽ�����רҵ��		
	endif
ENDPROC

proc 201
	learn_sec_skill 1 5
endproc
proc 300
	dialog 1
	add_talk #c0040FF�����ǹ�ʽ��÷�ʽ:#n
	add_talk #b#b#c186506��70�����¼����ǣ�65-70��Ұ��������#n
	add_talk #b#b#c186506��80�����¼����ǣ���������ڽܶ�����#n
	add_talk #c0040FF���ζƽ�ʽ:#n
	add_talk #b#b#c186506�ʧ��֮�����й���һ�����ʵ���#n
	add_talk #b#b
	
		option 1 ����
	talk
endproc





