;
; Я������\����503

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#bÿ���˶��ᾭ������׶Σ�����һ��ɽ������֪��ɽ������ʲô���󲿷��˻���Ϊɽ���վ�����ʧ�������С��Һ���������������֪��ɽ�Ǳ���ʲô��������ȥ�����ɣ�Ҳ����������ջ�

	;��һ�
	task_stat 4433 1 51
	if 51 = 2
		option 510 ����ο�ʡ�����������
	endif
	
	task_stat 1351 1 52
	if 52 = 2
		task_fini 1351 1
	endif
	
;	task_add_option 1089 1 112
	task_add_option 1090 1 113
	task_add_option 1091 1 114
	task_add_option 1092 1 115
	task_add_option 1093 1 116
	
	task_add_option 1084 1 117
	task_add_option 1085 1 118
	task_add_option 1088 1 119
	task_add_option 1086 1 120
	task_add_option 1087 1 121
	task_add_option 1089 1 122
	 

	task_add_option 1094 1 123
	
	strvar Ret
	strvar rum
	GET_ITEM_NUM_INPAK 5300966 1 rum
	task_stat 3025 1 Ret
	if Ret = 2 and rum = 0
		option 500 #�������ڵ�����ĺ����

	endif
	
	
	strvar Ret513
	TASK_STAT 1092 1 Ret513
	
	if Ret513 = 2
		option 160 #����֮��
	endif
	strvar ret
	task_stat 1088 1 ret
	if ret = 2
		OPTION 180 ��ʼ����
	endif
	
	option 130 #���̵�
	option 170 ����ǿ��������
	option 200 װ���ֽ⣨��ʯ��ã�ʾ��ͼ
	option 201 ��Ƕ��ʯ���������ԣ�ʾ��ͼ
	option 202 ��ʯ���գ���ʯ������ʾ��ͼ

	talk

endproc

proc 112
    task_do_option 1089 1
endproc
proc 113
    task_do_option 1090 1
endproc
proc 114
    task_do_option 1091 1
endproc
proc 115
    task_do_option 1092 1
endproc
proc 116
    task_do_option 1093 1
endproc

proc 117
    task_do_option 1084 1
endproc
proc 118
    task_do_option 1085 1
endproc
proc 119
    task_do_option 1088 1
endproc

proc 120
    task_do_option 1086 1
endproc
proc 121
    task_do_option 1087 1
endproc
proc 122
    task_do_option 1089 1
endproc
proc 123

    task_do_option 1094 1
endproc

proc 130
	task_stat 1094 1 20
	if 20 = 2
		task_fini 1094 10
	endif
	
	OPEN_NPC_SHOP 
endproc 
proc 160
	enter_pworld 513 0 1
endproc 

proc 170
;	CHOOSE 1 171 172 ����ǿ����������Ҫ����50000���ң�ȷ����Ҫ����
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�������ò����ϣ���������Խ��Խ�����ˣ�����Ȼ���ˡ����Ҳ����������Ū����������ֻ�Ǹ�̾�����㶮�ģ���
	add_talk #b#b�ҵ�Ȼ�����޸�����������������Ҫ����һ��СС�Ĵ��ۡ� 
	add_talk #b#b#cF93500������ã�5000����#n���Ȼ��Ѱ��������������򻨷����ң�	

		option 171 #����
	talk
endproc 
proc 171
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 5000 and @ret >= 5000
		choose 1 173 172 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 5000
		choose 1 173 172 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 173
	endif
endproc 
proc 172
	return
endproc

proc 173	
	strvar Num Retm
	GET_ITEM_NUM_INPAK 5300883 1 Num
	if Num > 0 
		strvar retmid
		GET_ROLE_RIDE_ON_MACHINE 0 retmid
		if retmid = 0
			DEC_MONEY_TEST 0 5000 21 Retm 
				
			if Retm = -1
				ADD_SYS_MSG_START
				ADD_SYS_MSG ���Ҳ��㣬����ʧ��!
				SET_SYS_MSG_MODE 3 0 0
				MSG_INFO
			else
				strvar Ret
				DEC_ITEM_NUM_INPAK 5300883 1 Ret
				if Ret = 0 
					strvar Retitem
					add_item_num_inpak 5300883 1 1 Retitem
					if Retitem = 0
						DEC_MONEY 0 5000 21 10	
					endif
				endif
			endif
		else
			ADD_SYS_MSG_START
			ADD_SYS_MSG ��е�����״̬����������
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		endif
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG ����ʧ�ܣ���������û����Ҫ�����ǿ�������ޡ�
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc 


proc 2
	strvar Retask
	get_trig_task 1 Retask
	if @Retask = 1092
		dialog 1
		add_talk $_npc$:
		add_talk #b#bÿ���˶��ᾭ������׶Σ�����һ��ɽ������֪��ɽ������ʲô���󲿷��˻���Ϊɽ���վ�����ʧ�������С��Һ���������������֪��ɽ�Ǳ���ʲô��������ȥ�����ɣ�Ҳ����������ջ�
		OPTION 160 #����֮��
		talk
	endif
	if @Retask = 1088
		dialog 1
		add_talk $_npc$:
		add_talk #b#bÿ���˶��ᾭ������׶Σ�����һ��ɽ������֪��ɽ������ʲô���󲿷��˻���Ϊɽ���վ�����ʧ�������С��Һ���������������֪��ɽ�Ǳ���ʲô��������ȥ�����ɣ�Ҳ����������ջ�
		OPTION 180 ��ʼ����
		talk
	endif
endproc

proc 180
	dialog 1
	add_talk $_npc$:
	add_talk #b#b 1�����ڷֽ�װ��������˵����ȷ���ǣ�
		option 181 A��10�����µ�װ�������Էֽ�
		option 181 B����ɫװ�������Էֽ�
		option 181 C���ֽ�װ���м��ʻ�þ�ʯ

		option 182 D������˵��ȫ��
		
	talk
endproc 

proc 181
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������˵��Ҳ��Ӵ��
	add_talk #b#b
	add_talk #b#b #c0181f9 10�����ϵ���ɫ����ɫ����ɫ����ɫװ�����Ա��ֽ⣬�ֽ�ʱһ����ò��ϣ��м��ʻ�þ�ʯ����ʯ����Ƕ����Դϵͳ�У������������ԡ�#n
		option 180 ���»ش�

	talk
endproc 
proc 182
	dialog 1
	add_talk $_npc$:
	add_talk #b#b 2����ʯ������������ģ�������ֵҲ�в��������ʣ���ʯ��������Դ��

		option 185 A��ϵͳ�������
		option 186 B���̳��˱��ֽ�ǰװ���ĸ�������
		option 185 C����ʲô��ʲô
		option 185 D����֪��
		
	talk
endproc 

proc 185
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��ȷ�Ĵ��ǣ�
	add_talk #b#b�̳��˱��ֽ�ǰװ���ĸ�������

	add_talk #b#b#c0181f9ͨ���ֽ��õľ�ʯ����̳�װ���ĸ������ԣ���ɾ�ʯ�����ԡ�#n
	add_talk #b#b
	add_talk #b#b#c0181f9���磺��ӵ��һ��װ����2���������ԣ�����+1000������+800����ô��ֽ⵽�ľ�ʯ�ĸ�������Ҫô��������Ҫô�Ƿ�����#n

		option 182 ���»ش�

	talk
endproc 
proc 186
	dialog 1
	add_talk $_npc$:
	add_talk #b#b 3����ʯ1�����˾�����ٱ�������ʯ���գ���ʯ1�����յľ���ᱻ�۵���

		option 190 A����
		option 191 #B������
		option 190 C�������
		
	talk
endproc 
proc 190
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��ȷ�Ĵ��ǣ�
	add_talk #b#b��ʯ�����յľ��鲻�ᱻ�۵�������������һ�ž�ʯ����
	add_talk #b#b
	add_talk #b#b#c0181f9��������ӵ����һ��3���Ľ��̾�ʯ��������������1ǧ�㾭��ֵ��Ϊ3�������ֵõ���һ�����Ը��õ�1���Ľ��̾�ʯ�����1����ʯ������3����ʯ�󣬻�ֱ�ӱ��3����������֮ǰ��1000�㾭��ֵ+�Ľ��̾�ʯ�ľ���ֵ����#n

		option 186 ���»ش�

	talk
endproc 

proc 191
	task_fini 1088 1
endproc 

proc 500
	task_stat 3025 1 Ret
	if Ret = 2
		strvar rum
		GET_ITEM_NUM_INPAK 5300966 1 rum
		if rum = 0
			ADD_ITEM_NUM_INPAK 5300966 1 1 20
		endif
	endif
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
			task_fini 4433 1
			
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#bлл�㣬$_player$���뽫�ҵ�ף���������ͣ�˳������лл����
			add_talk #b#bƽ��Ϊ�㿪��������������գ��Ҹ�������������������ӵ���������������֣�����Ͷ���ͱ������Ѷ�����գ�ף����һ�������⣬��������ң��
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













