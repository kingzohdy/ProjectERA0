;npc ����  ��ά��

proc 1
	GET_NOW_TIME 0 0 0 0 30 0 0
	if @30 >= 9 and @30 < 24
		strvar duihuanFlag
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#bԶ�����������ã���������������û������Ҫ�Ļ��Ҳ���ҿ����㶼������Щʲô�����챦���һ���߼��չ����ǣ�
		
		strvar Ret
		task_stat 4604 1 Ret
		get_task_var 4604 4 8 1 Loc3 
		if Ret = 2 and Loc3 = 0
			option 150 ��ʼ����
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
		OPEN_PAOSHANG_UI 7 @nmemid 0
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
		SET_PAOSHANG_INFO 4 1 0
		add_sys_msg_start
		add_sys_msg �һ��������������ӵ�����㹻��ı��𣬿�ȥ����ó�����˳��۵Ļ���ɣ�
		set_sys_msg_mode 4 0 0
		msg_info
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

proc 150
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���ݼ�����ʼ��$_player$�����׼�����ˣ�
		option 151 #׼������
		option 152 #���е���ţ���������Ϣһ����
	talk
endproc 
proc 151
	strvar Loc3
	get_task_var 4604 4 8 1 Loc3 
	var Loc3 1
	set_task_var 4604 4 8 @Loc3 1
	
	;��ʱ������
	strvar Tim
	get_task_var 4604 0 8 1 Tim
	var Tim 0
	set_task_var 4604 0 8 @Tim 1	
endproc 

proc 152
	return
endproc

proc 2
	
	get_trig_task 1 2 
	if 2 = 4604
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b�������������̫����Ŀ�����ˣ�ֻ���ĵ���ǰ�����棬ȴ���������������Щ������Ϊ����ʱ����������Ҳ����Ҳû�л����ʶ��Щ�漣��
		option 150 ��ʼ����
		talk
	endif
endproc
