;npc ����

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ʮ�£�����Ʈ�㣬�������õ�ʱ�̣�ʱ�����һƬ����Ц���ʱ�˿̣��Ҳ��������п�����л��Щ��ȥ��������˽���׵�Ӣ�ۣ��������ǽ�̻Ի͵Ĺ���ʢ����
	
		option 200 ������Ϊ���ɡ�δ�����ܶ������¾�
		option 300 ���ǻԱ��꡿�һ������
		
		TASK_ADD_OPTION 4634 0 100
		TASK_ADD_OPTION 4636 0 101
	talk
endproc 

proc 100
	TASK_DO_OPTION 4634 0
endproc

proc 101
	TASK_DO_OPTION 4636 0
endproc


proc 200
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bʱ����������ھ��ѣ�ħ������������֯���Ѹ���ɱ䡣
	add_talk #b#b��ӭǰ������Ϊ����ȥ��Ӣ�ۣ�Ϊ������Ϊ���ɡ�δ�����ܶ�����ʿ��Ϊ���������䰮���ˣ�
	add_talk #b#b
	add_talk #b#b#c186506���ɻ�� ���Ǹ��գ����������ϡ���ߣ� #n
  
		GET_PLAYER_DAILY_FLAG 0 14 22
		if @22 = 0
			option 204 ����ÿ��һ�Σ���Ҫ99Ӣ�ۼ���ң�
		endif
		option 202 ������Ҫ99��ң�	
		option 203 ������Ҫ999��ң�
		option 1 ����
	talk
endproc

proc 202
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg ���ı���������������������Ԥ��һ������
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
		
	choose 18 208 200 ����Ҫ#cb496ff����99���#n�������ȷ��������\n�ɹ���ɻ��9���������գ�
endproc

proc 203
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg ���ı���������������������Ԥ��һ������
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
		
	choose 18 209 200 ����Ҫ#cb496ff����999���#n�������ȷ��������\n�ɹ���ɻ��95���������գ�
endproc

proc 204
	GET_PLAYER_DAILY_FLAG 0 14 11
	;0��ʾδ����
	if 11 = 1
		PLAYER_CLOSE_DIALOG1 0 0
		return
	endif
	
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg ���ı���������������������Ԥ��һ������
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
		
	choose 18 210 200 ����Ҫ#cb496ff����99Ӣ�ۼ����#n�������ȷ��������\n�ɹ���ɻ��1�����Ǹ��գ�
endproc

proc 208
	DEC_MONEY_TEST 0 99 60 11
	if @11 = 0
		DEC_MONEY 0 99 60 10
		if @10 = 0
			add_sys_msg_start
			add_sys_msg ������������Ը�㼪�����գ�
			set_sys_msg_mode 3 0 0
			msg_info
			
			ADD_ITEM_NUM_INPAK 5060051 9 1 14 
			if @14 = 0
				add_sys_msg_start
				add_sys_msg ���������Ѿ��������ı����У�ף��������֣�
				set_sys_msg_mode 3 0 0
				msg_info
				
				add_sys_msg_start
				add_sys_msg $_player$ �����еõ���Ӧ������˼�������
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg ��Ҳ���
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
endproc

proc 209
	DEC_MONEY_TEST 0 999 60 11
	if @11 = 0
		DEC_MONEY 0 999 60 10
		if @10 = 0
			add_sys_msg_start
			add_sys_msg ������������Ը�㼪�����գ�
			set_sys_msg_mode 3 0 0
			msg_info
			
			ADD_ITEM_NUM_INPAK 5060051 95 1 14 
			if @14 = 0
				add_sys_msg_start
				add_sys_msg ���������Ѿ��������ı����У�ף��������֣�
				set_sys_msg_mode 3 0 0
				msg_info
				
				add_sys_msg_start
				add_sys_msg $_player$ �����еõ���Ӧ������˼�������
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg ��Ҳ���
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
endproc

proc 210
	GET_ITEM_NUM_INPAK 5060050 13 15
	if @13 = 0 and @15 >= 99
		DEC_ITEM_NUM_INPAK 5060050 99 16
		if @16 = 0
			add_sys_msg_start
			add_sys_msg ��ĵ���ֱ��������Ը�㼪�Ǹ���
			set_sys_msg_mode 3 0 0
			msg_info
			
			strvar Ret 
			SET_PLAYER_DAILY_FLAG Ret 14 1
			if Ret = 0
				ADD_BIND_ITEM_NUM_INPAK 5060052 1 1 14 
				if @14 = 0
					add_sys_msg_start
					add_sys_msg ���Ǹ����Ѿ��������ı����У�ף��������֣�
					set_sys_msg_mode 3 0 0
					msg_info
					
					add_sys_msg_start
					add_sys_msg $_player$ �����еõ���Ӧ������˼��Ǹ���
					set_sys_msg_mode 18 0 0
					svr_msg_info
				endif
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg Ӣ�ۼ������������
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	PLAYER_CLOSE_DIALOG1 0 0
endproc

proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bֻҪ�����㹻�ġ�#c005ebe�ǻԱ���#n�������ܸ��Ҷһ�ͬ�ȼ�ֵ�ġ�#cff4e00���#n����
	add_talk #b#b
	add_talk #c186506�һ����ʣ�
	add_talk #b#b1�����ǻԱ��꡿�һ�10�����#n
	
		option 301 1�����ǻԱ��꡿�һ�10�����
		option 302 10�����ǻԱ��꡿�һ�100�����
	talk
endproc

proc 301
	strvar ret0 num0 ret9 pmid
	GET_ITEM_NUM_INPAK 5200020 ret0 num0
	if ret0 = 0 and num0 > 0
		DEC_ITEM_NUM_INPAK 5200020 1 ret9
		if ret9 = 0
			GET_PLAYER_ID 1 pmid
			add_money @pmid 10 61 1
			
			add_sys_msg_start
			add_sys_msg ���ѳɹ��һ���10���[��ݼ�V]�ɴ��̳ǲ鿴
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg �����ϵ��ǻԱ��겻�㣬��ʱ�޷��һ�
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 302
	strvar ret0 num0 ret9 pmid
	GET_ITEM_NUM_INPAK 5200020 ret0 num0
	if ret0 = 0 and num0 >= 10
		DEC_ITEM_NUM_INPAK 5200020 10 ret9
		if ret9 = 0
			GET_PLAYER_ID 1 pmid
			add_money @pmid 100 61 1
			
			add_sys_msg_start
			add_sys_msg ���ѳɹ��һ���100���[��ݼ�V]�ɴ��̳ǲ鿴
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg �����ϵ��ǻԱ��겻�㣬��ʱ�޷��һ�
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc
