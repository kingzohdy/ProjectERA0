;������

proc 1
	DIALOG 1
	strvar Rflag Maxlv
	GET_CUR_MAX_LEVEL Rflag Maxlv
	
	add_talk $_npc$:
	if Rflag = 0 and Maxlv <= 79
		add_talk #b#b���ﾹȻ���ܼ������ˣ���������
		add_talk #b#b#name#����˵����˹����������Ԩ�����ͷ�ӡ��ʧ��֮�����ײ㡣
		add_talk #b#b����ħ���궯��ԭ��ϣ��������������
	endif
	if Rflag = 0 and Maxlv > 79
		add_talk #b#bħ���ѱ�����Եֵ����������ǰ�����߲㣡���ｻ���������������
		add_talk #b#b�������������У�
	endif
	add_talk #b#b
	strvar ret LV
	GET_ID_PLAYER_INFO 0 8 LV
	PLOY_GC_GET_AWARD ret 5
	if LV < 70
		add_talk #c186506���´�ʧ��֮�򡿻˵����
		add_talk �ﵽ70�������ʧ��֮����㡾����������ȡ����#n
	else
		add_talk #c186506���´�ʧ��֮�򡿻˵����
		add_talk ͻ�Ƹ���ʧ��֮��������������ȡ����
		add_talk Խ�߲�����������������ȡ�ľ���Խ��#n
		
		if ret != -1 and ret != -2
			option 200 #����������������������ȡ���飩
		endif
			option 300 ��������������Щ��Ա
	endif
		option 400 ����������������˹�����
	
	talk
endproc

proc 200
	strvar ret Retm Con Val
	PLOY_GC_GET_AWARD ret 5
	
	if ret != -1 and ret != -2
		GET_ID_PLAYER_INFO 0 19 Con	
		if Con != -1
			var Val @Con
			dev Val 100
			mul Val 11
			; mul Val 10000
			; dev Val 10000
	
			ADD_EXP 0 @Val Retm
			if Retm = 0
				PLOY_GC_SET_AWARDE 0 5
				
				add_sys_msg_start
				add_sys_msg $_player$�ڡ��´�ʧ��֮�򡿻�гɹ�ͻ�Ƶ���㣬��ȡ�˴�������!
				set_sys_msg_mode 3 0 0
				SVR_MSG_INFO
			endif
		endif
	endif
endproc

proc 300
	DIALOG 1
	strvar Rflag Maxlv
	GET_CUR_MAX_LEVEL Rflag Maxlv
	
	strvar ret1 ret2 ret3 ret4 ret5
	PLOY_GC_GET_AWARD ret1 1
	PLOY_GC_GET_AWARD ret2 2
	PLOY_GC_GET_AWARD ret3 4
	PLOY_GC_GET_AWARD ret4 5
	PLOY_GC_GET_AWARD ret5 7
	
	
	add_talk $_npc$:
	add_talk #b#b�����������ȷ��߷ֲ�ʧ��֮����㣬����פ������Ӧ������
	add_talk #b#b
	add_talk #c186506���´�ʧ��֮�򡿻˵����
	add_talk ͻ�Ƹ���ʧ��֮��������������ȡ����
	add_talk Խ�߲�����������������ȡ�ľ���Խ��#n
	add_talk #b#b
	if ret1 = -2
		add_talk ��һ�㣺������˾����#c186506����ȡ����#n��
	else
		add_talk ��һ�㣺#L������˾��@@(3211,23400,21900)#n��δ��ȡ���飩
	endif
	if ret2 = -2
		add_talk �ڶ��㣺�������೤��#c186506����ȡ����#n��
	else
		add_talk �ڶ��㣺#L�������೤@@(3212,19900,22800)#n��δ��ȡ���飩
	endif
	if ret3 = -2
		add_talk ���Ĳ㣺�������ӳ���#c186506����ȡ����#n)
	else
		add_talk ���Ĳ㣺#L�������ӳ�@@(3214,13800,16800)#n��δ��ȡ���飩
	endif
	
	if Rflag = 0 and Maxlv > 74
		if ret4 = -2
			add_talk ����㣺������������#c186506����ȡ����#n��
		else
			add_talk ����㣺#L����������@@(3215,26500,6000)#n��δ��ȡ���飩
		endif
	endif
	
	if Rflag = 0 and Maxlv > 79
		if ret5 = -2
			add_talk ���߲㣺�������ų���#c186506����ȡ����#n��
		else
			add_talk ���߲㣺#L�������ų�@@(3217,9300,6400)#n��δ��ȡ���飩
		endif
	endif
	
	
	option 1 ����
	talk
endproc

proc 400
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b������Ԩ��������˹������Ĵ�˵�����Ҷ���İ����
	add_talk #b#b�������缸����������ͷ�Ϲ�ħ�����ϣ������൱ʱ�����������޷����������������ֻ�ܸ�������Ĵ��۽����ӡ��
	add_talk #b#bȻ����һ�в��Ǵ�˵����˹����ͷ�ӡ������ʧ��֮����߲㡣
	add_talk #b#b�������������ĵ�ʹ�������ػ������ӡ��ֱ�������ٴν������졣
	
	
	option 1 ����
	talk
endproc
