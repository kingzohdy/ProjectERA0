;���֮��NPC

proc 1

	
	DIALOG 1
	add_talk $_npc$:	
	add_talk #b#b���������С��ţһ����һ�����紵�ݶ����ϾͶ���������ô������ᵽ�������Ȥ�أ�
	add_talk #b#b����Ӧ�ò���ȥ̽�����¸ҵĽ����������ս������������˶�ʵ�������
	add_talk #b#b���ھ���һ�������������ǰ��Ը�����ҽ���һ�δ�Խ���ĵ�̽������ô��
	add_talk #b#b
	add_talk #c186506�ر���ʾ��
	add_talk #b#b������뿪���֮�ģ����Զ��������ϵ�״̬
	
	task_add_option 3341 1 100
	task_add_option 3342 1 101
	task_add_option 3170 1 102
	task_add_option 3171 1 103
	
	task_stat 3410 1 30
	if 30 = 1
		task_add_option 3410 1 301
	endif
	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300927 1 num1
	if num1 > 0
		TASK_STAT 3305 1 Ret
		if Ret = 2
			option 700 �����뻻����
		endif
	endif
	task_stat 3401 1 2
	task_stat 3410 1 3
	if @2 = 2 or @3 = 2
		option 230 #������֮��
		
		option 240 #���޿˽�������ȥ�ˣ�
		option 225 15��̫���ˣ����������취��
		
	talk
	else
		option 230 #������֮��
	endif
	
	
	talk	
endproc 

PROC 100
	task_do_option 3341 1
ENDPROC
PROC 101
	task_do_option 3342 1
ENDPROC
PROC 102
	task_do_option 3170 1
ENDPROC
PROC 103
	task_do_option 3171 1
ENDPROC
proc 2
	get_trig_task 1 2 
	if 2 = 3171
		task_fini 3171 1
	endif

endproc 
proc 225
	DIALOG 1
	add_talk $_npc$:	
	add_talk #b#b��Ȼ�У�Ҫ���㿪��֤��Ҳ����ʲô���£�ֻ�������
	add_talk #b#b#c676464<�����ɰ������۾��������µش������㣬���־�����ǰ����ָ��ʳָ����ָ������Ķ�����Ȼ����ƮƮ��˵>#n
	add_talk #b#b���������ð�˺ܴ���յġ������ң��һ���Ҫ���´���㡭��
	
		option 226 #��Ը���2ǧ���ң�ֻ��һֽ֤��
		option 300 #���������취��
	talk
endproc
proc 226
	;strvar Moy
	;DEC_MONEY_TEST 0 20 20 Moy
	;if Moy = -1
		;ADD_SYS_MSG_START
		;ADD_SYS_MSG �ܱ�Ǹ���㱳��û����ô��Ǯ��
		;SET_SYS_MSG_MODE 1 0 0
		;MSG_INFO
		;return
	;else
		strvar Num
		get_pack_space 0 Num
		if Num > 0
		
			DEC_MONEY_TEST 0 2000 20 30
			if 30 = 0
				ADD_ITEM_NUM_INPAK 5300522 1 0 10
				if 10 = 0
					DEC_MONEY 0 2000 20 10
				endif
			else
				ADD_SYS_MSG_START
				ADD_SYS_MSG �����û����ô��Ǯ��
				SET_SYS_MSG_MODE 4 0 0
				MSG_INFO
			endif
		else
			ADD_SYS_MSG_START
			ADD_SYS_MSG ����ʣ��ռ䲻�㣡
			SET_SYS_MSG_MODE 1 0 0
			MSG_INFO
		endif
	;endif
	
endproc

proc 230
	OPEN_UI 36
endproc

proc 240
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���޿˽���Ϊ���֮�������˷ǳ��޴�Ĺ��ף�
	add_talk #b#bΪ�˴����֮�ܹ�������ת�����������ڵ�15������������ά����������к���Ҫ�����飬�͵�����ȥ�����ɡ�
	

		option 250 #�����ҽ�����֮��
		option 260 #���Ҹ�ǿ������ȥ����
	talk

endproc

proc 250
	OPEN_UI 36
endproc

proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�׻�˵����Ǯ�ĳ�Ǯ�������ĳ���������Ҹպ��м�С�������æ�����ý��ţ�����ʲô���¡�
	add_talk #b#bǰ���죬��ȥԽ��ͨ������ʱ�������ս��Ϯ���ˣ����һ���ʱ���������Ų�Ʊ�����ˣ�Ȼ���ҽӵ�֪ͨ��˵���Ų�Ʊ�����صȽ���
	add_talk #b#bֻҪ���ܰ��ҰѲ�Ʊ�һ������Ҿͻ���������������һ�Ų��޿˽��ڵ�֤����
	add_talk #b#b��������㣬�����ھͲ���վ�����﷢����
	
	add_talk #b#b ʱ����ǽ�Ǯ��$_Career$��
	
		task_add_option 3410 1 301
	talk
endproc

PROC 301
	task_do_option 3410 1
ENDPROC

proc 700
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ˣ���λͬѧ���þò��������ҽ����ġ����ɸǶٴ��桷��������û���������ʲô�����ǻ��ǻ���
	add_talk #b#b�ðɣ������ﻹ�������½ڣ�������һ�ҡ�

		option 701 #����

	talk
	
endproc
proc 701
	
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300927 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300927 1 1
			
			rand Rad 100
			if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300928 1 0 0
			endif
			if @Rad >= 80 and @Rad < 95
				ADD_ITEM_NUM_INPAK 5300929 1 0 0
			endif
			if @Rad >= 95 and @Rad < 99
				ADD_ITEM_NUM_INPAK 5300930 1 0 0
			endif
			if @Rad = 99
				ADD_ITEM_NUM_INPAK 5300931 1 0 0
			endif
		else
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg ������û�п��Խ�������Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc 

