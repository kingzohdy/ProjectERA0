;��³³

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Ҫ��#c186506��ʥ���ǹ�#n����
	add_talk #b#b�������Ҫ������Ը���˵��
	add_talk #b#b�㲻����˵������ô֪������Ҫ�ء������˵����һ�����㡣
	add_talk #b#b��ʵ��Ҫ��Ҫ�ǹ��ˣ�
	
		OPTION 200 #��Ҫ�ǹ���

	talk
	call 300
endproc

proc 200
	DIALOG 1
	STRVAR cs
	
	add_talk $_npc$:
	add_talk #b#b1.ÿ���30���ӿ������ȡ1�Ρ�
	add_talk #b#b2.������ܸ����ң���~~�ǹ�������á�
	add_talk #b#b
	
	GET_ROLE_VAR 58 8 0 cs
	if cs = 1
		add_talk #b#b֧��#cFF4e002���#n������ȡ1���ǹ���
	elseif cs = 2
		add_talk #b#b֧��#cFF4e005���#n������ȡ1���ǹ���
	elseif cs = 3
		add_talk #b#b֧��#cFF4e0010���#n������ȡ1���ǹ���
	elseif cs = 4
		add_talk #b#b֧��#cFF4e0020���#n������ȡ1���ǹ���
	elseif cs = 5
		add_talk #b#b֧��#cFF4e0050���#n������ȡ1���ǹ���
	elseif cs = 6
		add_talk #b#b֧��#cFF4e00100���#n������ȡ1���ǹ���
	elseif cs > 6
		add_talk #b#b֧��#cFF4e00200���#n������ȡ1���ǹ���
	else
	endif
	
	STRVAR sec js sj
	
	GET_TIME_SEC sec
	GET_ROLE_VAR 59 32 0 js
	
	sub sec @js
	var sj 1800
	sub sj @sec
	; dev sj 60
	if sj > 0 and sj <= 1800
		add_talk #b#b�����´������ȡ����Ҫ#c186506$@sj$#n��
	endif
	
	
		OPTION 201 #���ϸ��������ǹ�
		OPTION 1 ����

	talk
	call 300
endproc

proc 201
	STRVAR cs
	
	GET_ROLE_VAR 58 8 0 cs ;��ȡ��ȡ����
	if cs < 1
		call 202
	elseif cs = 1
		choose 1 202 203 �����ȡ��ȴʱ��δ�����Ƿ񻨷�2��Ҷһ�?
	elseif cs = 2
		choose 1 202 203 �����ȡ��ȴʱ��δ�����Ƿ񻨷�5��Ҷһ�?
	elseif cs = 3
		choose 1 202 203 �����ȡ��ȴʱ��δ�����Ƿ񻨷�10��Ҷһ�?
	elseif cs = 4
		choose 1 202 203 �����ȡ��ȴʱ��δ�����Ƿ񻨷�20��Ҷһ�?
	elseif cs = 5
		choose 1 202 203 �����ȡ��ȴʱ��δ�����Ƿ񻨷�50��Ҷһ�?
	elseif cs = 6
		choose 1 202 203 �����ȡ��ȴʱ��δ�����Ƿ񻨷�100��Ҷһ�?
	elseif cs > 6
		choose 1 202 203 �����ȡ��ȴʱ��δ�����Ƿ񻨷�200��Ҷһ�?
	else
	endif
	
	call 300
endproc

PROC 202

	STRVAR ret ret1 ret2 kg cs js jb
	
	GET_PACK_SPACE 0 kg ;��ȡ����ʣ�����
	
	if kg < 1
		add_sys_msg_start
		add_sys_msg �ճ�1������������������Ŷ
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	
	CALL 300
	
	GET_ROLE_VAR 58 8 0 cs ;��ȡ��ȡ����	
	if cs < 1
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012136 1 1 ret2
			if ret2 = 0
				GET_TIME_SEC js ;��ȡ��ǰʱ��
					
				; ��¼��ǰʱ��
				SET_ROLE_VAR 59 32 @js 0
					
				; ��¼��ȡ����
				add cs 1
				SET_ROLE_VAR 58 8 @cs 0
				CALL 200
			endif
	elseif cs = 1
		var jb 2
		DEC_MONEY_TEST 0 @jb 60 ret ;���Я���������
		if ret = 0 
			DEC_MONEY 0 @jb 60 ret1 ;�۳���Ӧ�������
			if ret1 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012136 1 1 ret2
					if ret2 = 0
						; ��¼��ȡ����
						add cs 1
						SET_ROLE_VAR 58 8 @cs 0
						CALL 200
					endif
			endif
		else
			add_sys_msg_start
			add_sys_msg ����еĽ�Ҳ��㣬ÿ30����ֻ����ȡ1���ǹ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	elseif cs = 2
		var jb 5
		DEC_MONEY_TEST 0 @jb 60 ret ;���Я���������
		if ret = 0
			DEC_MONEY 0 @jb 60 ret1 ;�۳���Ӧ�������
			if ret1 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012136 1 1 ret2
					if ret2 = 0
						; ��¼��ȡ����
						add cs 1
						SET_ROLE_VAR 58 8 @cs 0
						CALL 200
					endif
			endif
		else
			add_sys_msg_start
			add_sys_msg ����еĽ�Ҳ��㣬ÿ30����ֻ����ȡ1���ǹ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	elseif cs = 3
		var jb 10
		DEC_MONEY_TEST 0 @jb 60 ret ;���Я���������
		if ret = 0
			DEC_MONEY 0 @jb 60 ret1 ;�۳���Ӧ�������
			if ret1 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012136 1 1 ret2
					if ret2 = 0
						; ��¼��ȡ����
						add cs 1
						SET_ROLE_VAR 58 8 @cs 0
						CALL 200
					endif
			endif
		else
			add_sys_msg_start
			add_sys_msg ����еĽ�Ҳ��㣬ÿ30����ֻ����ȡ1���ǹ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	elseif cs = 4
		var jb 20
		DEC_MONEY_TEST 0 @jb 60 ret ;���Я���������
		if ret = 0
			DEC_MONEY 0 @jb 60 ret1 ;�۳���Ӧ�������
			if ret1 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012136 1 1 ret2
					if ret2 = 0
						; ��¼��ȡ����
						add cs 1
						SET_ROLE_VAR 58 8 @cs 0
						CALL 200
					endif
			endif
		else
			add_sys_msg_start
			add_sys_msg ����еĽ�Ҳ��㣬ÿ30����ֻ����ȡ1���ǹ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	elseif cs = 5
		var jb 50
		DEC_MONEY_TEST 0 @jb 60 ret ;���Я���������
		if ret = 0
			DEC_MONEY 0 @jb 60 ret1 ;�۳���Ӧ�������
			if ret1 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012136 1 1 ret2
					if ret2 = 0
						; ��¼��ȡ����
						add cs 1
						SET_ROLE_VAR 58 8 @cs 0
						CALL 200
					endif
			endif
		else
			add_sys_msg_start
			add_sys_msg ����еĽ�Ҳ��㣬ÿ30����ֻ����ȡ1���ǹ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	elseif cs = 6
		var jb 100
		DEC_MONEY_TEST 0 @jb 60 ret ;���Я���������
		if ret = 0
			DEC_MONEY 0 @jb 60 ret1 ;�۳���Ӧ�������
			if ret1 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012136 1 1 ret2
					if ret2 = 0
						; ��¼��ȡ����
						add cs 1
						SET_ROLE_VAR 58 8 @cs 0
						CALL 200
					endif
			endif
		else
			add_sys_msg_start
			add_sys_msg ����еĽ�Ҳ��㣬ÿ30����ֻ����ȡ1���ǹ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	elseif cs > 6
		var jb 200
		DEC_MONEY_TEST 0 @jb 60 ret ;���Я���������
		if ret = 0
			DEC_MONEY 0 @jb 60 ret1 ;�۳���Ӧ�������
			if ret1 = 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012136 1 1 ret2
				CALL 200
			endif
		else
			add_sys_msg_start
			add_sys_msg ����еĽ�Ҳ��㣬ÿ30����ֻ����ȡ1���ǹ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
	endif
ENDPROC

proc 203

endproc

proc 300
	STRVAR sec cs js
	
	GET_TIME_SEC sec
	GET_ROLE_VAR 58 8 0 cs
	GET_ROLE_VAR 59 32 0 js
	
	sub sec @js
	if sec > 1800
		var cs 0
		SET_ROLE_VAR 58 8 @cs 0
		var js 0
		SET_ROLE_VAR 59 32 @js 0
	endif
endproc

