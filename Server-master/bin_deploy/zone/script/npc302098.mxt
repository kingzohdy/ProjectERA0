;npc ����  ����

proc 1
	GET_NOW_TIME 0 0 0 0 30 0 0
	if @30 >= 9 and @30 < 24
		strvar duihuanFlag
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#bԶ�����������ã���������������û������Ҫ�Ļ��Ҳ���ҿ����㶼������Щʲô�����챦���һ���߼��չ����ǣ�
		
		task_stat 4605 0 100
		task_stat 4606 0 101
		task_stat 4607 0 102
		task_stat 4608 0 103
		task_stat 4609 0 104
		task_stat 4610 0 105
		
		if 100 = 2 or 101 = 2 or 102 = 2 or 103 = 2 or 104 = 2 or 105 = 2
			option 200 #�̻�ó��-�ύ��Ʒ
		endif

		
		TASK_STAT 4613 1 70
		if @70 = 2
			option 500 ����ó����Ʒ
			GET_PAOSHANG_INFO 6 duihuanFlag 0
			if @duihuanFlag = 0
				option 501 �һ�ó�׻��ҡ���Ʊ��
			endif
		endif
			
		talk
	else
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b�簡����ƣ�����������̫���ˣ�ó���̵껹û�����أ���ÿ�춼���ڴ��ȵ�ʱ��˼������������׬����Ǯ�������������Լ��Ļ����������õļ۸񣬿�ϧ���˻�������������ʱ��û�ҵ����õİ취�������������ǲ��ǻ�������׬Ǯ�Ļ����أ�Ҫ���������ø����⣿
        add_talk #b#b
		add_talk #b#bó���̵꿪��ʱ�� ����9��00������23��59

		talk
	endif
endproc 

proc 500
	strvar nid nmemid
	GET_NPC_DEFID 0 nid
	GET_MAP_NPC_ID  @nid nmemid 10
	if @10 = 0
		;OPEN_PAOSHANG_UI ���̶�����id npc��memid ���ر�־
		OPEN_PAOSHANG_UI 9 @nmemid 0
		PLAYER_CLOSE_DIALOG1 0 0 
	endif
endproc

proc 501
	
	strvar ret pglock
	PKG_IS_UNLOCK ret pglock
	if @ret = 0 and @pglock = 0
		return
	endif
	
	strvar maxhuanyp curryihuanyp 
	strvar yihaveitemnum yinpiao ExchangeFlag
	
    GET_PAOSHANG_INFO 1 maxhuanyp 10
	GET_PAOSHANG_INFO 2 curryihuanyp 20
	GET_PAOSHANG_INFO 3 yihaveitemnum 30
	GET_PAOSHANG_INFO 4 yinpiao 40
	
	if @10 = -1 or @20 = -1 or @30 = -1 or @40 = -1 or @50 = -1
		return
	endif
	
	if @curryihuanyp > @maxhuanyp
		add_sys_msg_start
		add_sys_msg �һ��������������ӵ�����㹻��ı��𣬿�ȥ����ó�����˳��۵Ļ���ɣ�
		set_sys_msg_mode 4 0 0
		msg_info
		SET_PAOSHANG_INFO 4 1 0
	else
		DIALOG 1
		add_talk $_npc$:
		add_talk �һ�ó�׻��ҡ�����Ʊ��
		add_talk #b#b#b���ɶһ���Ʊ��$@maxhuanyp$
		add_talk #b#b#b��ǰ�Ѷһ���Ʊ��$@curryihuanyp$
		add_talk #b#b
		add_talk �һ�������
		add_talk #b#b#b#c6d00d21���� = 1��Ʊ#n
		
		option 603 ʹ�����Ҷһ�1000��Ʊ��1ǧ���ң�
		option 604 ʹ�����Ҷһ�5000��Ʊ��5ǧ���ң�
		option 605 ������һҳ
		
		talk
	endif
endproc

proc 605
	call 1
endproc

;���Ҷһ�1000��Ʊ��1ǧ���ң�
proc 603
	strvar maxhuanyp currhuanyp 
	strvar yinpiao tempnum jiaopiao
	strvar iflag
	var iflag 0
	
	GET_PAOSHANG_INFO 1 maxhuanyp 10
	GET_PAOSHANG_INFO 2 curryihuanyp 20
	GET_PAOSHANG_INFO 4 yinpiao 40
	
	GET_PAOSHANG_INFO 5 jiaopiao 50
	
	if @10 = -1 or @20 = -1 or @40 = -1 or @50 = -1
		return
	endif
	
	if @curryihuanyp >= @maxhuanyp
		add_sys_msg_start
		add_sys_msg �һ��������������ӵ�����㹻��ı��𣬿�ȥ����ó�����˳��۵Ļ���ɣ�
		set_sys_msg_mode 4 0 0
		msg_info
		call 1
		return
	endif
	
	var tempnum @curryihuanyp
	add tempnum 1000
	if @tempnum > @maxhuanyp
		add_sys_msg_start
		add_sys_msg �ܱ�Ǹ������Ҫ�󲻺Ϲ�أ���ʣ��Ķ�Ȳ���
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	DEC_MONEY_TEST 0 1000 20 15
	if 15 = 0
		DEC_MONEY 0 1000 20 12
		if 12 = 0		
			add yinpiao 1000
			SET_PAOSHANG_INFO 2 @yinpiao 60
			add curryihuanyp 1000
			SET_PAOSHANG_INFO 1 @curryihuanyp 70
			
			PAOSHANG_DUIHUAN_REFRESH 1 0

			if @curryihuanyp >= @maxhuanyp
				SET_PAOSHANG_INFO 4 1 0
				add_sys_msg_start
				add_sys_msg �һ��������������ӵ�����㹻��ı��𣬿�ȥ����ó�����˳��۵Ļ���ɣ�
				set_sys_msg_mode 4 0 0
				msg_info
				var iflag 1
			endif
			
			if @jiaopiao <= @yinpiao
				TASK_STAT 4613 1 80
				if @80 = 2
					TASK_FINI 4613 0
					var iflag 1
				endif
			endif
			
			if @iflag = 1
				call 1
			else
				call 501
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg �������Ҳ��㡣
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif			
endproc

;���Ҷһ�5000��Ʊ��5ǧ���ң�
proc 604
	strvar maxhuanyp currhuanyp 
	strvar yinpiao tempnum jiaopiao
	strvar iflag
	var iflag 0
	
	GET_PAOSHANG_INFO 1 maxhuanyp 10
	GET_PAOSHANG_INFO 2 curryihuanyp 20
	GET_PAOSHANG_INFO 4 yinpiao 40
	
	GET_PAOSHANG_INFO 5 jiaopiao 50
	
	if @10 = -1 or @20 = -1 or @40 = -1 or @50 = -1
		return
	endif
	
	if @curryihuanyp >= @maxhuanyp
		add_sys_msg_start
		add_sys_msg �һ��������������ӵ�����㹻��ı��𣬿�ȥ����ó�����˳��۵Ļ���ɣ�
		set_sys_msg_mode 4 0 0
		msg_info
		call 1
		return
	endif
	
	var tempnum @curryihuanyp
	add tempnum 5000
	if @tempnum > @maxhuanyp
		add_sys_msg_start
		add_sys_msg �ܱ�Ǹ������Ҫ�󲻺Ϲ�أ���ʣ��Ķ�Ȳ���
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	DEC_MONEY_TEST 0 5000 20 15
	if 15 = 0
		DEC_MONEY 0 5000 20 12
		if 12 = 0		
			add yinpiao 5000
			SET_PAOSHANG_INFO 2 @yinpiao 60
			add curryihuanyp 5000
			SET_PAOSHANG_INFO 1 @curryihuanyp 70
			
			PAOSHANG_DUIHUAN_REFRESH 1 0
			
			if @curryihuanyp >= @maxhuanyp
				SET_PAOSHANG_INFO 4 1 0
				add_sys_msg_start
				add_sys_msg �һ��������������ӵ�����㹻��ı��𣬿�ȥ����ó�����˳��۵Ļ���ɣ�
				set_sys_msg_mode 4 0 0
				msg_info
				var iflag 1
			endif
			
			if @jiaopiao <= @yinpiao
				TASK_STAT 4613 1 80
				if @80 = 2
					TASK_FINI 4613 0
					var iflag 1
				endif
			endif
			
			if @iflag = 1
				call 1
			else
				call 501
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg �������Ҳ��㡣
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif			
endproc


proc 200
	dialog 3
	add_talk $_npc$:
	add_talk #b#b�����������������ʲô��
	add_talk #b#b
	
	task_stat 4605 0 100
	task_stat 4606 0 101
	task_stat 4607 0 102
	task_stat 4608 0 103
	task_stat 4609 0 104
	task_stat 4610 0 105
	
	if 100 = 2
		add_talk #b#b#c186506�ռ���ͬ��5�������Ƽ�����Ļ�е����Ʒ#n
	endif
	if 101 = 2
		add_talk #b#b#c186506�ռ������2��ֽ�׿Խװ����õ�ˮ������#n
	endif
	if 102 = 2
		add_talk #b#b#c186506�ռ������2ƿ����ҩˮ#n
	endif
	if 103 = 2
		add_talk #b#b#c186506�ռ������1�Ÿ�ħ��������ɫƷ�ʸ�ħ����#n
	endif
	if 104 = 2
		add_talk #b#b#c186506�ռ������1ƿ���������Ļָ�ҩˮ#n
	endif
	if 105 = 2
		add_talk #b#b#c186506�ռ���ͬ��10ƿ�������Ƶ�ҩƬ����#n
	endif
	
		NPC_COMMIT_OPT 300
	
	talk
endproc

proc 300

	strvar ID Pos_1 Pos_2 ret
	GET_COMMIT_ITEM_INFO 1 ID
	GET_COMMIT_ITEM_INFO 2 Pos_1
	GET_COMMIT_ITEM_INFO 3 Pos_2
	
	if @ID = -1
		add_sys_msg_start
		add_sys_msg �㻹û���ύ��Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info		
		return 
	endif

	task_stat 4605 0 100
	if 100 = 2
		if @ID = 5051008 or @ID = 5051009 or @ID = 5051098 or @ID = 5051101 or @ID = 5051002 or @ID = 5051003
			switch @ID
				case 5051008
					DEC_ITEM_NUM_BYIDX 5051008 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4605 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5051009
					DEC_ITEM_NUM_BYIDX 5051009 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4605 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5051098
					DEC_ITEM_NUM_BYIDX 5051098 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4605 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5051101
					DEC_ITEM_NUM_BYIDX 5051101 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4605 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5051002
					DEC_ITEM_NUM_BYIDX 5051002 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4605 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5051003
					DEC_ITEM_NUM_BYIDX 5051003 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4605 0
							call 400
						ENDIF
					ENDIF
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg ��Ʒ��������Ҫ��δ�ܳɹ��Ͻ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	
	task_stat 4606 0 101
	if 101 = 2
		if @ID = 5103021 or @ID = 5103022 or @ID = 5103023
			switch @ID
				case 5103021
					DEC_ITEM_NUM_BYIDX 5103021 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4606 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5103022
					DEC_ITEM_NUM_BYIDX 5103022 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4606 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5103023
					DEC_ITEM_NUM_BYIDX 5103023 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4606 0
							call 400
						ENDIF
					ENDIF
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg ��Ʒ��������Ҫ��δ�ܳɹ��Ͻ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	

	task_stat 4607 0 102
	if 102 = 2
		if @ID = 5006100 or @ID = 5006101 or @ID = 5006102 or @ID = 5006103 or @ID = 5006106 or @ID = 5006108 or @ID = 5006109 or @ID = 5006105 or @ID = 5006104 or @ID = 5006107 or @ID = 5006110 or @ID = 5006111 or @ID = 5006112 or @ID = 5006113
			switch @ID
				case 5006100
					DEC_ITEM_NUM_BYIDX 5006100 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4607 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5006101
					DEC_ITEM_NUM_BYIDX 5006101 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4607 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5006102
					DEC_ITEM_NUM_BYIDX 5006102 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4607 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5006103
					DEC_ITEM_NUM_BYIDX 5006103 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4607 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5006106
					DEC_ITEM_NUM_BYIDX 5006106 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4607 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5006108
					DEC_ITEM_NUM_BYIDX 5006108 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4607 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5006109
					DEC_ITEM_NUM_BYIDX 5006109 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4607 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5006105
					DEC_ITEM_NUM_BYIDX 5006105 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4607 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5006104
					DEC_ITEM_NUM_BYIDX 5006104 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4607 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5006107
					DEC_ITEM_NUM_BYIDX 5006107 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4607 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5006110
					DEC_ITEM_NUM_BYIDX 5006110 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4607 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5006111
					DEC_ITEM_NUM_BYIDX 5006111 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4607 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5006112
					DEC_ITEM_NUM_BYIDX 5006112 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4607 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5006113
					DEC_ITEM_NUM_BYIDX 5006113 2 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4607 0
							call 400
						ENDIF
					ENDIF
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg ��Ʒ��������Ҫ��δ�ܳɹ��Ͻ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	
	task_stat 4608 0 103
	if 103 = 2
		if @ID = 5240301 or @ID = 5240311 or @ID = 5240321 or @ID = 5240331 or @ID = 5241001 or @ID = 5241101 or @ID = 5241011 or @ID = 5241111 or @ID = 5241021 or @ID = 5241121
			switch @ID
				case 5240301
					DEC_ITEM_NUM_BYIDX 5240301 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5240311
					DEC_ITEM_NUM_BYIDX 5240311 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5240321
					DEC_ITEM_NUM_BYIDX 5240321 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5240331
					DEC_ITEM_NUM_BYIDX 5240331 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5241001
					DEC_ITEM_NUM_BYIDX 5241001 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase	
				case 5241101
					DEC_ITEM_NUM_BYIDX 5241101 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5241011
					DEC_ITEM_NUM_BYIDX 5241011 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5241111
					DEC_ITEM_NUM_BYIDX 5241111 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5241021
					DEC_ITEM_NUM_BYIDX 5241021 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5241121
					DEC_ITEM_NUM_BYIDX 5241121 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
			endswitch
		elseif @ID = 5241131 or @ID = 5241031 or @ID = 5240001 or @ID = 5240101 or @ID = 5240011 or @ID = 5240111 or @ID = 5240021 or @ID = 5240121 or @ID = 5240131 or @ID = 5240031
			switch @ID
				case 5241131
					DEC_ITEM_NUM_BYIDX 5241131 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5241031
					DEC_ITEM_NUM_BYIDX 5241031 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5240001
					DEC_ITEM_NUM_BYIDX 5240001 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5240101
					DEC_ITEM_NUM_BYIDX 5240101 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5240011
					DEC_ITEM_NUM_BYIDX 5240011 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5240111
					DEC_ITEM_NUM_BYIDX 5240111 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase	
				case 5240021
					DEC_ITEM_NUM_BYIDX 5240021 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5240121
					DEC_ITEM_NUM_BYIDX 5240121 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5240131
					DEC_ITEM_NUM_BYIDX 5240131 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5240031
					DEC_ITEM_NUM_BYIDX 5240031 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
			endswitch
		elseif @ID = 5240701 or @ID = 5240711 or @ID = 5240721 or @ID = 5240931 or @ID = 5240901 or @ID = 5240911 or @ID = 5240921 or @ID = 5240731 or @ID = 5240501 or @ID = 5240511
			switch @ID
				case 5240701
					DEC_ITEM_NUM_BYIDX 5240701 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5240711
					DEC_ITEM_NUM_BYIDX 5240711 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5240721
					DEC_ITEM_NUM_BYIDX 5240721 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5240931
					DEC_ITEM_NUM_BYIDX 5240931 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5240901
					DEC_ITEM_NUM_BYIDX 5240901 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5240911
					DEC_ITEM_NUM_BYIDX 5240911 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase	
				case 5240921
					DEC_ITEM_NUM_BYIDX 5240921 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5240731
					DEC_ITEM_NUM_BYIDX 5240731 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5240501
					DEC_ITEM_NUM_BYIDX 5240501 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5240511
					DEC_ITEM_NUM_BYIDX 5240511 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
			endswitch
		elseif @ID = 5240521 or @ID = 5240531 or @ID = 5241301 or @ID = 5241201 or @ID = 5241311 or @ID = 5241211 or @ID = 5241321 or @ID = 5241221 or @ID = 5241231 or @ID = 5241331
			switch @ID
				case 5240521
					DEC_ITEM_NUM_BYIDX 5240521 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
						case 5240531
					DEC_ITEM_NUM_BYIDX 5240531 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5241301
					DEC_ITEM_NUM_BYIDX 5241301 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5241201
					DEC_ITEM_NUM_BYIDX 5241201 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5241311
					DEC_ITEM_NUM_BYIDX 5241311 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5241211
					DEC_ITEM_NUM_BYIDX 5241211 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase	
				case 5241321
					DEC_ITEM_NUM_BYIDX 5241321 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5241221
					DEC_ITEM_NUM_BYIDX 5241221 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5241231
					DEC_ITEM_NUM_BYIDX 5241231 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5241331
					DEC_ITEM_NUM_BYIDX 5241331 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4608 0
							call 400
						ENDIF
					ENDIF
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg ��Ʒ��������Ҫ��δ�ܳɹ��Ͻ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	
	task_stat 4609 0 104
	if 104 = 2
		if @ID = 5001002 or @ID = 5000002 or @ID = 5001003 or @ID = 5000003 or @ID = 5001004 or @ID = 5000004 or @ID = 5001001 or @ID = 5000001
			switch @ID
				case 5001002
					DEC_ITEM_NUM_BYIDX 5001002 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4609 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5000002
					DEC_ITEM_NUM_BYIDX 5000002 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4609 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5001003
					DEC_ITEM_NUM_BYIDX 5001003 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4609 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5000003
					DEC_ITEM_NUM_BYIDX 5000003 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4609 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5001004
					DEC_ITEM_NUM_BYIDX 5001004 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4609 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5000004
					DEC_ITEM_NUM_BYIDX 5000004 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4609 0
							call 400
						ENDIF
					ENDIF
				endcase	
				case 5001001
					DEC_ITEM_NUM_BYIDX 5001001 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4609 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5000001
					DEC_ITEM_NUM_BYIDX 5000001 1 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4609 0
							call 400
						ENDIF
					ENDIF
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg ��Ʒ��������Ҫ��δ�ܳɹ��Ͻ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	
	task_stat 4610 0 105
	if 105 = 2
		if @ID = 5011101 or @ID = 5010101 or @ID = 5011102 or @ID = 5010102 or @ID = 5011103 or @ID = 5010103 or @ID = 5010106 or @ID = 5011106
			switch @ID
				case 5011101
					DEC_ITEM_NUM_BYIDX 5011101 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4610 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5010101
					DEC_ITEM_NUM_BYIDX 5010101 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4610 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5011102
					DEC_ITEM_NUM_BYIDX 5011102 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4610 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5010102
					DEC_ITEM_NUM_BYIDX 5010102 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4610 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5011103
					DEC_ITEM_NUM_BYIDX 5011103 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4610 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5010103
					DEC_ITEM_NUM_BYIDX 5010103 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4610 0
							call 400
						ENDIF
					ENDIF
				endcase	
				case 5010106
					DEC_ITEM_NUM_BYIDX 5010106 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4610 0
							call 400
						ENDIF
					ENDIF
				endcase
				case 5011106
					DEC_ITEM_NUM_BYIDX 5011106 10 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 401
					else
						if @ret = 0
							task_fini 4610 0
							call 400
						ENDIF
					ENDIF
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg ��Ʒ��������Ҫ��δ�ܳɹ��Ͻ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	
endproc

proc 400
	add_sys_msg_start
	add_sys_msg �ɹ��Ͻ���Ʒ���������
	set_sys_msg_mode 3 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc

proc 401
	add_sys_msg_start
	add_sys_msg ��Ʒ����Ҫ��������δ�ܳɹ��Ͻ�
	set_sys_msg_mode 3 0 0
	msg_info
endproc
