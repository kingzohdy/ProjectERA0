;������

proc 1
	DIALOG 1
	strvar Rflag Maxlv ret num ret1 num1
	GET_CUR_MAX_LEVEL Rflag Maxlv
	GET_ITEM_NUM_INPAK 5060080 ret num
	GET_VAILD_ITEM_NUM_INPAK 5060080 ret1 num1
	
	add_talk $_npc$:
	if Rflag = 0 and Maxlv <= 79
		add_talk #b#b����ʧ��֮���ħ�����쳣�궯��Ϊ���ҵ������Դͷ��������������ʧ��֮��ײ㡣
		add_talk #b#b����ѹ�ȥһ��ʱ�䣬��������������ƺ���û����ļ򵥡�ϣ������ͻ�Ƹ���ʧ��֮���ҵ��������������
	endif
	if Rflag = 0 and Maxlv > 79
		add_talk #b#b���ܸо���һ��ǿ�������ʧ��֮��ĵײ������ӿ����Ҳ����˹����ķ�ӡ���ٴα������
		; add_talk #b#b�¹��ش��ҽ������㡾#c005ebe��ʿ֤֮#n����ϣ������ͻ�Ƹ���ʧ��֮���ټ��ȷ��ߡ�
		; add_talk #b#b
		; add_talk #b #c005ebe����ʧ��֮���߲㣬��Ҫ���С���ʿ֤֮��
	endif
	add_talk #b#b
	strvar LV
	GET_ID_PLAYER_INFO 0 8 LV
	if LV < 70
		add_talk #c186506���´�ʧ��֮�򡿻˵����
		add_talk �ﵽ70�������ʧ��֮����㡾����������ȡ����#n
	else
		add_talk #c186506���´�ʧ��֮�򡿻˵����
		add_talk ͻ�Ƹ���ʧ��֮��������������ȡ����
		add_talk Խ�߲�����������������ȡ�ľ���Խ��#n
	
	; if Rflag = 0 and Maxlv > 69
		; if ret = 0 and num < 1
			; option 200 #�������ҡ�����֤֮��
		; endif
		; if ret = 0 and num > 0
			; if ret1 = 0 and num1 < 1
				; option 201 #��ȡ�µġ�����֤֮��
			; endif
		; endif
	; endif
			option 300 ��������������Щ��Ա
	endif
	talk
endproc

proc 200
	strvar ret num
	GET_ITEM_NUM_INPAK 5060080 ret num
	
	if ret = 0 and num < 1
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5060080 1 1 0
		
		add_sys_msg_start
		add_sys_msg ������ո�µġ�����֤֮��������߲��¸������ɣ�
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc

proc 201
	strvar ret num
	GET_ITEM_NUM_INPAK 5060080 ret num

	if ret = 0 and num > 0
		DEC_ITEM_NUM_INPAK 5060080 @num 0
		ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5060080 1 1 0
	
		add_sys_msg_start
		add_sys_msg �㻻ȡ��ո�µġ�����֤֮��������߲��¸������ɣ�
		set_sys_msg_mode 1 0 0
		msg_info
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
	add_talk #b#bΪ���ҵ�ħ���궯��ԭͷ������ǲ��������פ����ʧ��֮����㡣
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
