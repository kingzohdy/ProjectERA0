

proc 1
	strvar ret LV
	GET_PLAYER_SECONDARY_SKILL 0 4 ret LV
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�Ҵ�������һ���������ֻ��Ϊ�����ǵ�������֫�����Ǿʹ��ˡ�
	add_talk #b#bû��������ң�������˼������θ���Ч�ļ���װ��������
	add_talk #c0040FF�ɿ󳵻�ȡ;����
	add_talk #c0040FF1. ������С��ܶ���ʹ�ù��ṱ�׶ȹ���
	add_talk #c0040FF2. �����ɼ������ɼ���Ա����ɡ���ʹ�ð�������
	add_talk #b#b#c0040FF������ɼ�������ʱ�䣺ÿ��16��00-23��00
	add_talk #b#b#c0040FF�񡰴����������������������ɼ����֤
	if ret = -1 
		add_talk #b#b
		add_talk #c186506����ﵽ30���󣬿��ڴ�ѧϰ�����졿רҵ#n
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
;	task_add_option 3405 1 103
	
	;��һ�
	task_stat 4430 1 51
	if 51 = 2
		option 510 ����ο�ʡ�����������
	endif
	
	task_add_option 4046 1 104
	task_add_option 4047 1 105
	task_add_option 4048 1 106
	task_add_option 4049 1 107
	task_add_option 4050 1 108

	task_add_option 3405 1 305

;	strvar m_Ret m_Lv
;	GET_PLAYER_SECONDARY_SKILL 0 4 m_Ret m_Lv
;	if m_Ret = 0
;	task_stat 4020 1 10
;	if 10 = 3
		OPTION 202 #����Ƽ�����רҵ�б�
		strvar m_Ret m_Lv
		GET_PLAYER_SECONDARY_SKILL 0 4 m_Ret m_Lv
		if m_Ret != 0	
			OPTION 110 #��Ϊ��ϰ����ʦ
		endif
		option 300 ��������ͼֽ��÷�ʽ
;	endif	

	talk
endproc 

proc 202
	OPEN_UI 40
endproc

PROC 103
	task_do_option 3405 1
ENDPROC

PROC 104	
	task_do_option 4046 1
ENDPROC

PROC 105	
	task_do_option 4047 1
ENDPROC

PROC 106	
	task_do_option 4048 1
ENDPROC

PROC 107	
	task_do_option 4049 1
ENDPROC

PROC 108	
	task_do_option 4050 1
ENDPROC
	
PROC 305	
	task_do_option 3405 1
ENDPROC

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
	GET_PLAYER_SECONDARY_SKILL 0 4 m_Ret m_Lv
	if m_Ret = 0
		OPEN_UI 40
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
		;�������ܳ���3��
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
	learn_sec_skill 1 4
endproc

proc 300
	DIALOG 1
	add_talk #c0040FF����ͼֽ��÷�ʽ:#n
	add_talk #b#b#c186506��ʱ��Ǻ��������ꡢɳ���ȴ����䷽ϻ����#n
	add_talk #b#b#c186506��Ұ�⾫Ӣ������һ�����ʵ�������ͼֽ#n
	add_talk #b#b
		
		
		option 1 ����
	
	talk
endproc 




;��һ�
proc 510
	GET_ITEM_NUM_INPAK 5301004 1 10
	STRVAR flag
	var flag 0
	if 10 > 0
		DEC_ITEM_NUM_INPAK 5301004 1 11
		if 11 = 0
			var flag 1
			task_fini 4430 1
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#bлл�㣬$_player$���뽫�ҵ�ף���������ͣ�˳������лл����
			add_talk #b#b���������Ӳӣ�ף�������������
			add_talk #b#b���»�����ƮƮ��ף����ͺ�������
			add_talk #b#b����ϲȵ��������ף����ƽƽ������
			add_talk #b#b�������Ҹ���࣬ף���������֣�
			option 511 #û����
			talk
		endif
	endif
	
	if flag = 0
		add_sys_msg_start
		add_sys_msg ���ı�����û�С���Ʒ�С���
		set_sys_msg_mode 1 0 0
		msg_info
		PLAYER_CLOSE_DIALOG1 0 0
	endif
endproc



