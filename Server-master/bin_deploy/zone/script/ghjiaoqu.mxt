

proc 1
	DIALOG 1
	strvar ClanRet
	is_in_my_clan_city  ClanRet
	add_talk $_npc$:
	if ClanRet = 0
		add_talk #b#b�����ˣ�������������еõ�ʲô���ص���Ϣ����
	else
		add_talk #b#b��֪�����ᱦ�ص����ܣ�����������Ҫ�ҵ��Ļ��������µ㹦��ģ�
		
		strvar pos ret ret2 Ret3
		player_clan_pos_info ret 0 0 0 pos
		GET_EVENT_STAT ret2 66
		DYN_ACTIVE_STATUS_GET 1 19 ret3
		
		if ret = 0 and pos != 0 and ret2 = 1 and ret3 = 0
			option 150 #�����ᱦ�ء����δ������
		elseif  ret = 0 and pos != 0 and ret3 = 1
			option 150 #�����ᱦ�ء�����ѿ�����
		elseif  ret = 0 and pos != 0 and ret2 = 1 and ret3 = 2
			option 150 #�����ᱦ�ء�����ѽ�����
		else
		endif	
	endif
	talk
endproc 

proc 165
	
	ADD_EXP 0 100 0
endproc 

proc 150
	strvar time pos ret ret2 ret3
	get_now_time 1 1 1 1 time 1 1
	GET_EVENT_STAT ret 66
	DYN_ACTIVE_STATUS_GET 1 19 ret2
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�����з��ı���̽����������̽�������ڹ�����еص׵ı��ء�
	add_talk #c005ebe���۲��ߡ�����ͨ�ı���̽����#n
	add_talk #c005ebe�������š�����Ҫ�����з�������̽�⼼����#n
;	add_talk #c005ebe��̽·�ߡ�����Ҫ�����з�������̽�⼼����#n
	if ret = 1 and ret2 = 0
		add_talk #b#b
		add_talk #c186506����Ҫ��2������
		add_talk �ʱ�䣺��һ�������������1��(06:00-24:00)
		add_talk �᳤������06:00-22:00
		add_talk ��Ա������22:00-24:00��ǰ�᣺�᳤����δ������
		add_talk ���չ��δ����
		if time >= 6 and time < 24
			option 157 #�ٻ�����̽����-���۲��ߡ�
			option 158 #�ٻ�����̽����-�������š�
			; option 159 #�ٻ�����̽����-��̽·�ߡ�
		endif
	elseif ret2 = 1
			add_talk #b#b
			add_talk #c186506�ʱ�䣺��һ�������������1��(06:00-24:00)
			add_talk ���չ��������
			
			option 151 #��ȡ����е�ھ�����
	elseif ret = 1 and ret2 = 2
			add_talk #c186506�ʱ�䣺��һ�������������1��(06:00-24:00)
			add_talk �᳤������06:00-22:00
			add_talk ��Ա������22:00-24:00��ǰ�᣺�᳤����δ������
			add_talk ���չ���ѽ���#n
	else
	endif

		option 1 ����
	talk
endproc 
proc 157
	strvar time ret ret2 time2 ret3 pos ret4
	get_clan_building_lv ret 0 9
	GET_CLAN_CREATE_TIME 1 0 time
	is_same_day @time ret2
	get_now_time 1 1 1 1 time2 1 1
	PLAYER_CLAN_POS_INFO ret3 0 0 0 pos
	DYN_ACTIVE_STATUS_GET 1 19 ret4

	if  ret4 = 1
		add_sys_msg_start
		add_sys_msg �����ᱦ�ء��ѿ����������޷����ٻ�̽����
		set_sys_msg_mode 4 0 0
		msg_info
	elseif  ret4 = 2
		add_sys_msg_start
		add_sys_msg �����ᱦ�ء��ѽ����������޷����ٻ�̽����
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret < 2
		add_sys_msg_start
		add_sys_msg ����ȼ����㣬�޷����������ᱦ�ء��
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret2 = 0
		add_sys_msg_start
		add_sys_msg �������ᵱ�죬�޷����������ᱦ�ء��
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time2 < 6
		add_sys_msg_start
		add_sys_msg ���ڻ����ʱ���ڣ��޷����������ᱦ�ء�� 
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time2 >= 6 and time2 < 22
		if ret3 = 0 and ( pos = 7 or pos = 8 )
			choose 0 160 153 �Ƿ�ȷ�Ͽ��������ᱦ�ء�����ٻ����۲��ߡ���
		else
			add_sys_msg_start
			add_sys_msg 22:00��᳤��δ��������Ա�����п��������ᱦ�ء��
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time2 >= 22 and time2 < 24
		if ret3 = 0 and pos > 0
			choose 0 160 153 �Ƿ�ȷ�Ͽ��������ᱦ�ء�����ٻ����۲��ߡ���
		else
			add_sys_msg_start
			add_sys_msg �ǹ����Ա�޷����������ᱦ�ء��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc 
proc 158
	strvar time ret ret2 time2 ret3 pos
	get_clan_building_lv ret 0 9
	GET_CLAN_CREATE_TIME 1 0 time
	is_same_day @time ret2
	get_now_time 1 1 1 1 time2 1 1
	PLAYER_CLAN_POS_INFO ret3 0 0 0 pos
	DYN_ACTIVE_STATUS_GET 1 19 ret4

	if  ret4 = 1
		add_sys_msg_start
		add_sys_msg �����ᱦ�ء��ѿ����������޷����ٻ�̽����
		set_sys_msg_mode 4 0 0
		msg_info
	elseif  ret4 = 2
		add_sys_msg_start
		add_sys_msg �����ᱦ�ء��ѽ����������޷����ٻ�̽����
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret < 2
		add_sys_msg_start
		add_sys_msg ����ȼ����㣬�޷����������ᱦ�ء��
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret2 = 0
		add_sys_msg_start
		add_sys_msg �������ᵱ�죬�޷����������ᱦ�ء��
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time2 < 6
		add_sys_msg_start
		add_sys_msg ���ڻ����ʱ���ڣ��޷����������ᱦ�ء�� 
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time2 >= 6 and time2 < 22
		if ret3 = 0 and ( pos = 7 or pos = 8 )
			choose 0 161 153 �Ƿ�ȷ�Ͽ��������ᱦ�ء�����ٻ��������š���
		else
			add_sys_msg_start
			add_sys_msg 22:00��᳤��δ��������Ա�����п��������ᱦ�ء��
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time2 >= 22 and time2 < 24
		if ret3 = 0 and pos > 0
			choose 0 161 153 �Ƿ�ȷ�Ͽ��������ᱦ�ء�����ٻ��������š���
		else
			add_sys_msg_start
			add_sys_msg �ǹ����Ա�޷����������ᱦ�ء��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc 
proc 159
	strvar time ret ret2 time2 ret3 pos
	get_clan_building_lv ret 0 9
	GET_CLAN_CREATE_TIME 1 0 time
	is_same_day @time ret2
	get_now_time 1 1 1 1 time2 1 1
	PLAYER_CLAN_POS_INFO ret3 0 0 0 pos
	DYN_ACTIVE_STATUS_GET 1 19 ret4

	if  ret4 = 1
		add_sys_msg_start
		add_sys_msg �����ᱦ�ء��ѿ����������޷����ٻ�̽����
		set_sys_msg_mode 4 0 0
		msg_info
	elseif  ret4 = 2
		add_sys_msg_start
		add_sys_msg �����ᱦ�ء��ѽ����������޷����ٻ�̽����
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret < 2
		add_sys_msg_start
		add_sys_msg ����ȼ����㣬�޷����������ᱦ�ء��
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret2 = 0
		add_sys_msg_start
		add_sys_msg �������ᵱ�죬�޷����������ᱦ�ء��
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time2 < 6
		add_sys_msg_start
		add_sys_msg ���ڻ����ʱ���ڣ��޷����������ᱦ�ء�� 
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time2 >= 6 and time2 < 22
		if ret3 = 0 and ( pos = 7 or pos = 8 )
			choose 0 162 153 �Ƿ�ȷ�Ͽ��������ᱦ�ء�����ٻ���̽·�ߡ���
		else
			add_sys_msg_start
			add_sys_msg 22:00��᳤��δ��������Ա�����п��������ᱦ�ء��
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time2 >= 22 and time2 < 24
		if ret3 = 0 and pos > 0
			choose 0 162 153 �Ƿ�ȷ�Ͽ��������ᱦ�ء�����ٻ���̽·�ߡ���
		else
			add_sys_msg_start
			add_sys_msg �ǹ����Ա�޷����������ᱦ�ء��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc 

proc 160
	strvar time ret ret2 time2 ret3 pos
	get_clan_building_lv ret 0 9
	GET_CLAN_CREATE_TIME 1 0 time
	is_same_day @time ret2
	get_now_time 1 1 1 1 time2 1 1
	PLAYER_CLAN_POS_INFO ret3 0 0 0 pos
	DYN_ACTIVE_STATUS_GET 1 19 ret4

	if  ret4 = 1
		add_sys_msg_start
		add_sys_msg �����ᱦ�ء��ѿ����������޷����ٻ�̽����
		set_sys_msg_mode 4 0 0
		msg_info
	elseif  ret4 = 2
		add_sys_msg_start
		add_sys_msg �����ᱦ�ء��ѽ����������޷����ٻ�̽����
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret < 2
		add_sys_msg_start
		add_sys_msg ����ȼ����㣬�޷����������ᱦ�ء��
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret2 = 0
		add_sys_msg_start
		add_sys_msg �������ᵱ�죬�޷����������ᱦ�ء��
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time2 < 6
		add_sys_msg_start
		add_sys_msg ���ڻ����ʱ���ڣ��޷����������ᱦ�ء�� 
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time2 >= 6 and time2 < 22
		if ret3 = 0 and ( pos = 7 or pos = 8 )
			strvar rets rad 
			rand rad 3
			switch rad
				case 0
					NEW_CLAN_MON 307012 1 12800 29500 0 500 0 0 1200 rets 1
				endcase
				case 1
					NEW_CLAN_MON 307012 1 12800 29500 0 501 0 0 1200 rets 1
				endcase
				case 2
					NEW_CLAN_MON 307012 1 12800 29500 0 502 0 0 1200 rets 1
				endcase
			endswitch
			if rets = 0
				call 200
				;��¼�ٻ���̽�������ĸ�
				strvar Locg
				get_pworld_var 64 8 1 Locg
				var Locg 10
				set_pworld_var 64 8 @Locg 1
			endif
		else
			add_sys_msg_start
			add_sys_msg 22:00��᳤��δ��������Ա�����п��������ᱦ�ء��
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time2 >= 22 and time2 < 24
		if ret3 = 0 and pos > 0
			rand rad 3
			switch rad
				case 0
					NEW_CLAN_MON 307012 1 12800 29500 0 500 0 0 1200 rets 1
				endcase
				case 1
					NEW_CLAN_MON 307012 1 12800 29500 0 501 0 0 1200 rets 1
				endcase
				case 2
					NEW_CLAN_MON 307012 1 12800 29500 0 502 0 0 1200 rets 1
				endcase
			endswitch
			if rets = 0
				call 200
				;��¼�ٻ���̽�������ĸ�
				strvar Locg
				get_pworld_var 64 8 1 Locg
				var Locg 10
				set_pworld_var 64 8 @Locg 1
			endif
		else
			add_sys_msg_start
			add_sys_msg �ǹ����Ա�޷����������ᱦ�ء��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc

proc 161
	strvar time ret ret2 time2 ret3 pos
	get_clan_building_lv ret 0 9
	GET_CLAN_CREATE_TIME 1 0 time
	is_same_day @time ret2
	get_now_time 1 1 1 1 time2 1 1
	PLAYER_CLAN_POS_INFO ret3 0 0 0 pos
	DYN_ACTIVE_STATUS_GET 1 19 ret4

	if  ret4 = 1
		add_sys_msg_start
		add_sys_msg �����ᱦ�ء��ѿ����������޷����ٻ�̽����
		set_sys_msg_mode 4 0 0
		msg_info
	elseif  ret4 = 2
		add_sys_msg_start
		add_sys_msg �����ᱦ�ء��ѽ����������޷����ٻ�̽����
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret < 2
		add_sys_msg_start
		add_sys_msg ����ȼ����㣬�޷����������ᱦ�ء��
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret2 = 0
		add_sys_msg_start
		add_sys_msg �������ᵱ�죬�޷����������ᱦ�ء��
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time2 < 6
		add_sys_msg_start
		add_sys_msg ���ڻ����ʱ���ڣ��޷����������ᱦ�ء�� 
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time2 >= 6 and time2 < 22
		if ret3 = 0 and ( pos = 7 or pos = 8 )
			strvar rets rad TechLv
			GET_CLAN_TECH_LEVEL 1 0 25 TechLv
			if TechLv >= 1
				rand rad 3
				switch rad
					case 0
						NEW_CLAN_MON 307013 1 12800 29500 0 500 0 0 1500 rets 1
					endcase
					case 1
						NEW_CLAN_MON 307013 1 12800 29500 0 501 0 0 1500 rets 1
					endcase
					case 2
						NEW_CLAN_MON 307013 1 12800 29500 0 502 0 0 1500 rets 1
					endcase
				endswitch
				if rets = 0
					call 200
					strvar Locg
					get_pworld_var 64 8 1 Locg
					var Locg 20
					set_pworld_var 64 8 @Locg 1
				endif
			else
				add_sys_msg_start
				add_sys_msg ����δ�з�������̽�⼼�����Ƽ����޷��ٻ��������š�̽����
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg 22:00��᳤��δ��������Ա�����п��������ᱦ�ء��
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time2 >= 22 and time2 < 24
		if ret3 = 0 and pos > 0
			GET_CLAN_TECH_LEVEL 1 0 25 TechLv
			if TechLv >= 1
				rand rad 3
				switch rad
					case 0
						NEW_CLAN_MON 307013 1 12800 29500 0 500 0 0 1500 rets 1
					endcase
					case 1
						NEW_CLAN_MON 307013 1 12800 29500 0 501 0 0 1500 rets 1
					endcase
					case 2
						NEW_CLAN_MON 307013 1 12800 29500 0 502 0 0 1500 rets 1
					endcase
				endswitch
				if rets = 0
					call 200
					strvar Locg
					get_pworld_var 64 8 1 Locg
					var Locg 20
					set_pworld_var 64 8 @Locg 1
				endif
			else
				add_sys_msg_start
				add_sys_msg ����δ�з�������̽�⼼�����Ƽ����޷��ٻ��������š�̽����
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg �ǹ����Ա�޷����������ᱦ�ء��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc

proc 162
	strvar time ret ret2 time2 ret3 pos
	get_clan_building_lv ret 0 9
	GET_CLAN_CREATE_TIME 1 0 time
	is_same_day @time ret2
	get_now_time 1 1 1 1 time2 1 1
	PLAYER_CLAN_POS_INFO ret3 0 0 0 pos
	DYN_ACTIVE_STATUS_GET 1 19 ret4

	if  ret4 = 1
		add_sys_msg_start
		add_sys_msg �����ᱦ�ء��ѿ����������޷����ٻ�̽����
		set_sys_msg_mode 4 0 0
		msg_info
	elseif  ret4 = 2
		add_sys_msg_start
		add_sys_msg �����ᱦ�ء��ѽ����������޷����ٻ�̽����
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret < 2
		add_sys_msg_start
		add_sys_msg ����ȼ����㣬�޷����������ᱦ�ء��
		set_sys_msg_mode 4 0 0
		msg_info
	elseif ret2 = 0
		add_sys_msg_start
		add_sys_msg �������ᵱ�죬�޷����������ᱦ�ء��
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time2 < 6
		add_sys_msg_start
		add_sys_msg ���ڻ����ʱ���ڣ��޷����������ᱦ�ء�� 
		set_sys_msg_mode 4 0 0
		map_msg_info
	elseif time2 >= 6 and time2 < 22
		if ret3 = 0 and ( pos = 7 or pos = 8 )
			strvar rets rad TechLv
			GET_CLAN_TECH_LEVEL 1 0 25 TechLv
			if TechLv = 2
				rand rad 3
				switch rad
					case 0
						NEW_CLAN_MON 307014 1 12800 29500 0 500 0 0 1800 rets 1
					endcase
					case 1
						NEW_CLAN_MON 307014 1 12800 29500 0 501 0 0 1800 rets 1
					endcase
					case 2
						NEW_CLAN_MON 307014 1 12800 29500 0 502 0 0 1800 rets 1
					endcase
				endswitch
				if rets = 0
					call 200
					strvar Locg
					get_pworld_var 64 8 1 Locg
					var Locg 30
					set_pworld_var 64 8 @Locg 1
				endif
			else
				add_sys_msg_start
				add_sys_msg ����δ�з�������̽�⼼�����Ƽ����޷��ٻ���̽·�ߡ�̽����
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg 22:00��᳤��δ��������Ա�����п��������ᱦ�ء��
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	elseif time2 >= 22 and time2 < 24
		if ret3 = 0 and pos > 0
			GET_CLAN_TECH_LEVEL 1 0 25 TechLv
			if TechLv = 2
				rand rad 3
				switch rad
					case 0
						NEW_CLAN_MON 307014 1 12800 29500 0 500 0 0 1800 rets 1
					endcase
					case 1
						NEW_CLAN_MON 307014 1 12800 29500 0 501 0 0 1800 rets 1
					endcase
					case 2
						NEW_CLAN_MON 307014 1 12800 29500 0 502 0 0 1800 rets 1
					endcase
				endswitch
				if rets = 0
					call 200
					strvar Locg
					get_pworld_var 64 8 1 Locg
					var Locg 30
					set_pworld_var 64 8 @Locg 1
				endif
			else
				add_sys_msg_start
				add_sys_msg ����δ�з�������̽�⼼�����Ƽ����޷��ٻ���̽·�ߡ�̽����
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg �ǹ����Ա�޷����������ᱦ�ء��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc

proc 200

		strvar Pos
		player_clan_pos_info 1 0 0 0 Pos
		DYN_ACTIVE_STATUS_SET 1 19 1
		
		add_sys_msg_start
		if Pos = 7
			add_sys_msg [$_ClanName$]���ḱ�᳤[$_player$]�������ᱦ�ػ����Ѱ�̲��ڹ�����еײ����صı��ء�
		endif
		if Pos = 8
			add_sys_msg [$_ClanName$]����᳤[$_player$]�������ᱦ�ػ����Ѱ�̲��ڹ�����еײ����صı��ء�
		endif
		if Pos != 7 and Pos != 8
			add_sys_msg [$_ClanName$]�����Ա[$_player$]�������ᱦ�ػ����Ѱ�̲��ڹ�����еײ����صı��ء�
		endif
		set_sys_msg_mode 1 0 0
		svr_msg_info
		
		
		;��¼����ID ���ڹ���Ƶ���㲥
		strvar ClanID
		get_pworld_var 0 32 1 ClanID		
		GET_ID_PLAYER_INFO 0 18 ClanID
		set_pworld_var 0 32 @ClanID 1
	    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

		add_sys_msg_start
		if Pos = 7		
			add_sys_msg ���ḱ�᳤��$_player$��������Ѱ�ҹ��ᱦ�ػ��
		endif
		if Pos = 8
			add_sys_msg ����᳤��$_player$��������Ѱ�ҹ��ᱦ�ػ��
		endif
		if Pos != 7 and Pos != 8
			add_sys_msg �����Ա��$_player$��������Ѱ�ҹ��ᱦ�ػ��
		endif 
		set_sys_msg_mode 14 0 0
		CLAN_MSG_INFO 1 @ClanID

endproc

proc 151

	strvar RetNum inum
	GET_PACK_SPACE 0 RetNum
	GET_ITEM_NUM_INPAK 5052101 1 inum
	if inum = 0
		if RetNum >= 1			
			
			ADD_BIND_ITEM_NUM_INPAK 5052101 1 1 0
		else
			add_sys_msg_start
			add_sys_msg �����������޷���ȡ��Ʒ��
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg ���Ѿ��������ھ����ˣ������ظ���ȡ
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc 

;proc 152
;	strvar hp
;	GET_ID_PLAYER_INFO 0 2 hp
;;	if hp <= 0
;		add_sys_msg_start
;		add_sys_msg ����״̬���ɿ������
;		set_sys_msg_mode 1 0 0
;		msg_info
;	endif
;endproc
