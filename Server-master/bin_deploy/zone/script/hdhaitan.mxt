;��̲NPC�׶�˹

proc 1
	get_crayfish_num_oneday 0 10
	dialog 1
	add_talk $_npc$:
    add_talk #b#b�ף���ӭ�������⺣̲Ŷ����#004
    add_talk #b#b�ף���������ұ��ߣ�����ɳ̲��Ŷ��#003
    add_talk #b#b�ף��Ƕ���������˧����ŮӴ��#002
    add_talk #b#b�ף�����ÿ��1Сʱ�ĳ����չ�ԡ����Ӵ��#009
	task_add_option 2246 1 100
	get_role_var 6 8 0 20
	if 20 = 1
		option 200 #(����)���̵�
	endif
	if @10 < 5
		option 300 �Ͻ���ζС��Ϻ
	endif
	
	option 400 ��������
	
	talk
	
endproc 

proc 3
	GET_TRIG_TASK 0 10
	if @10 = 2246
		set_role_var 6 8 1 0
	endif
endproc

PROC 100
	task_do_option 2246 1
ENDPROC

proc 200
	OPEN_NPC_SHOP
endproc

proc 300
	get_crayfish_num_oneday 0 10
	var 11 5
	sub 11 @10
	dialog 1
	add_talk $_npc$:
    add_talk #b#bʲô��������ζС��Ϻ�����죬�Ͻ��������һ�����ѵĽ�����
    add_talk #b#b��������˵С��Ϻ�Զ��˻�����ı��Σ���Ȼ����͸���5���ɣ��ҵ���˼�ǣ�ÿ��5��Ӵ��
    add_talk #b#b
	if 10 = 5
		add_talk #b#b#c186506�Ѿ�����5����ζС��Ϻ�������ٳ�һ����Ŷ�����У�Ϊ���ҵ�ħ����ģ���ҪHoldס��
	else
		add_talk #b#b#c186506�����Ÿ�����$@10$����ʣ���$@11$�����������������
	endif
	if 11 >= 1
		option 320 �ύ1ֻ
	endif
	if 11 = 5
		option 330 �ύ5ֻ
	endif
	option 1 ����
	talk
endproc

;�ύ1ֻ
proc 320
	;����ѡ���ύ��
	var 100 1
	GET_ITEM_NUM_INPAK 5052201 1 2
	;�жϱ����е���Ʒ�� ѡ���ύ���Ĺ�ϵ
	if 2 < @100
		;������С��Ϻ���� ����0 ��ð����ʾ
		if 2 > 0 
			;npcð��
			strvar Mid
			GET_MAP_NPC_ID 307004 Mid 1
			MON_SPEECH @Mid 1 0 1 ��ƭ��Ů�����㲻���ò������ҵ���ζС��Ϻ��
		endif
		;��ʾ
		add_sys_msg_start
		add_sys_msg С��Ϻ�������㣬����ȥɳ̲��׽�����ύ
		set_sys_msg_mode 4 0 0
		msg_info
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	;��ȡ�����Ѿ��ύ������ 10
	GET_CRAYFISH_NUM_ONEDAY 0 10
	;���㵱�컹��Ҫ�ύ������ 11
	var 11 5
	sub 11 @10
	dialog 1
	add_talk $_npc$:
	;�жϵ��컹��Ҫ�ύ������11 �� ѡ������ύ���Ĺ�ϵ100
	if 100 <= @11
		commit_crayfish_num_once 0 @100 40
		DEC_ITEM_NUM_INPAK 5052201 @100 0
		ADD_EXP 0 @40 0
		;12 �˴β������컹��Ҫ�ύ������
		var 12 @11
		sub 12 @100
		add_talk #b#b �ɹ��ύ��$@100$ֻ
		add_talk #b#b ��þ��飺$@40$
		if 12 > 0 
			add_talk #b#b �����ύ��$@12$ֻ
		endif
		if 100 <= @12
			option 320 �����ύ
			option 300 ����
		else
			option 350 #�ر����
		endif
	endif
	talk
endproc

;�ύ5ֻ
proc 330
	;����ѡ���ύ��
	var 100 5
	GET_ITEM_NUM_INPAK 5052201 1 2
	;�жϱ����е���Ʒ�� ѡ���ύ���Ĺ�ϵ
	if 2 < @100
		;������С��Ϻ���� ����0 ��ð����ʾ
		if 2 > 0
			;npcð��
			strvar Mid
			GET_MAP_NPC_ID 307004 Mid 1
			MON_SPEECH @Mid 1 0 1 ��ƭ��Ů�����㲻���ò������ҵ���ζС��Ϻ��
		endif
		;��ʾ
		add_sys_msg_start
		add_sys_msg С��Ϻ�������㣬����ȥɳ̲��׽�����ύ
		set_sys_msg_mode 4 0 0
		msg_info
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	;��ȡ�����Ѿ��ύ������ 10
	GET_CRAYFISH_NUM_ONEDAY 0 10
	;���㵱�컹��Ҫ�ύ������ 11
	var 11 5
	sub 11 @10
	dialog 1
	add_talk $_npc$:
	;�жϵ��컹��Ҫ�ύ������11 �� ѡ������ύ���Ĺ�ϵ100
	if 100 <= @11
		commit_crayfish_num_once 0 @100 40
		DEC_ITEM_NUM_INPAK 5052201 @100 0
		ADD_EXP 0 @40 0
		;12 �˴β������컹��Ҫ�ύ������
		var 12 @11
		sub 12 @100
		add_talk #b#b �ɹ��ύ��$@100$ֻ
		add_talk #b#b ��þ��飺$@40$
		if 12 > 0 
			add_talk #b#b �����ύ��$@12$ֻ
		endif
		if 100 <= @12
			option 320 �����ύ
		endif
	endif
	option 350 #�ر����
	talk
endproc

proc 350
	;PLAYER_CLOSE_DIALOG 0 0
endproc


proc 400
	strvar Ret Sex Level Job
	dialog 1
	add_talk $_npc$:
    add_talk #b#b���ж���������𣿸��Ұɣ���Ը����߼۹���Ӵ����Ҫ����û��Ǯ��ֻ����û�л���
    add_talk #b#b�����ͨ���������������ʢ������������С������Ȼ�����������ͻ��úö�ö����飡
	
	 get_player_base_att  Ret Sex Level Job
	 
		option 401 #��Ө���� 1000����/��
		
	if @Level >= 80
		option 402 #Ө������ 1200����/��
		option 403 #�������� 1400����/��
		option 404 #ů������ 1600����/��
	elseif @Level >= 70
		option 402 #Ө������ 1200����/��
		option 403 #�������� 1400����/��
	elseif @Level >= 60
		option 402 #Ө������ 1200����/��
	else
	
	endif	
	talk
endproc

proc 401
	GET_ITEM_NUM_INPAK 5060053 1 40
	if @40 < 1
		add_sys_msg_start
		add_sys_msg ������û�о�Ө����
		set_sys_msg_mode 4 0 0
		msg_info
		return 
    endif
	
	strvar num
	var num @40
	MUL num 1000
	ADD_MONEY 0 @num 20 123 
	if 123 < 0
		add_sys_msg_start
		add_sys_msg �������е������Ѿ��ﵽ����
		set_sys_msg_mode 3 0 0
		msg_info
	else
		DEC_ITEM_NUM_INPAK 5060053 @40 1
		
		add_sys_msg_start
		add_sys_msg ������Ө����$@40$�����������$@num$
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 402
	GET_ITEM_NUM_INPAK 5060054 1 40
	if @40 < 1
		add_sys_msg_start
		add_sys_msg ������û��Ө������
		set_sys_msg_mode 4 0 0
		msg_info
		return 
    endif
	
	strvar num
	var num @40
	MUL num 1200
	ADD_MONEY 0 @num 20 123 
	if 123 < 0
		add_sys_msg_start
		add_sys_msg �������е������Ѿ��ﵽ����
		set_sys_msg_mode 3 0 0
		msg_info
	else
		DEC_ITEM_NUM_INPAK 5060054 @40 1
		
		add_sys_msg_start
		add_sys_msg ����Ө������$@40$�����������$@num$
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 403
	GET_ITEM_NUM_INPAK 5060055 1 40
	if @40 < 1
		add_sys_msg_start
		add_sys_msg ������û�к�������
		set_sys_msg_mode 4 0 0
		msg_info
		return 
    endif
	
	strvar num
	var num @40
	MUL num 1400
	ADD_MONEY 0 @num 20 123 
	if 123 < 0
		add_sys_msg_start
		add_sys_msg �������е������Ѿ��ﵽ����
		set_sys_msg_mode 3 0 0
		msg_info
	else
		DEC_ITEM_NUM_INPAK 5060055 @40 1
		
		add_sys_msg_start
		add_sys_msg ������������$@40$�����������$@num$
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 404
	GET_ITEM_NUM_INPAK 5060056 1 40
	if @40 < 1
		add_sys_msg_start
		add_sys_msg ������û��ů������
		set_sys_msg_mode 4 0 0
		msg_info
		return 
    endif
	
	strvar num
	var num @40
	MUL num 1600
	ADD_MONEY 0 @num 20 123 
	if 123 < 0
		add_sys_msg_start
		add_sys_msg �������е������Ѿ��ﵽ����
		set_sys_msg_mode 3 0 0
		msg_info
	else
		DEC_ITEM_NUM_INPAK 5060056 @40 1
		
		add_sys_msg_start
		add_sys_msg ����ů������$@40$�����������$@num$
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc
