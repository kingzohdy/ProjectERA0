;npc ����  ���

proc 1
	GET_NOW_TIME 0 0 0 0 30 0 0
	if @30 >= 9 and @30 < 24
		strvar duihuanFlag
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#bԶ�����������ã���������������û������Ҫ�Ļ��Ҳ���ҿ����㶼������Щʲô�����챦���һ���߼��չ����ǣ�
		
		strvar sta sta_2 sta_3
		task_stat 4601 1 sta
		if sta = 2
			option 200 #����Ҫʲô�����أ�
		endif
		task_stat 4602 1 sta_2
		if sta_2 = 2
			option 300 #����Ҫʲô�����أ�
		endif
		task_stat 4603 1 sta_3
		if sta_3 = 2
			option 400 #����Ҫʲô���ﵰ�أ�
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
		OPEN_PAOSHANG_UI 5 @nmemid 0
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


proc 200
	strvar ret
	
	dialog 3
	add_talk $_npc$:
	get_task_var 4601 0 8 1 ret	
	if ret = 0
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�Ұս��#n�����Բ���
	endif
	if ret = 1
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�������#n�����Բ���
	endif
	if ret = 2
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵĿ�սװ#n�����Բ���
	endif
	if ret = 3
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵĵ���װ#n�����Բ���
	endif
	if ret = 4
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵľ���ͷ��#n�����Բ���
	endif
	if ret = 5
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵķ���ͷ��#n�����Բ���
	endif
	if ret = 6
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�������ñ#n�����Բ���
	endif
	if ret = 7
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�ʮ������#n�����Բ���
	endif
	if ret = 8
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ���װ���#n�����Բ���
	endif
	if ret = 9
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�Ԫ�ػ���#n�����Բ���
	endif
	if ret = 10
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ��ȳ�����#n�����Բ���
	endif
	if ret = 11
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵĽ�ͽ���#n�����Բ���
	endif
	if ret = 12
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵľ������#n�����Բ���
	endif
	if ret = 13
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵķ��ļ��#n�����Բ���
	endif
	if ret = 14
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�Ұ��II��#n�����Բ���
	endif
	if ret = 15
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�ʮ�ּ��#n�����Բ���
	endif
	if ret = 16
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�սʿѥ#n�����Բ���
	endif
	if ret = 17
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵķ���ѥ#n�����Բ���
	endif
	if ret = 18
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�ð��ѥ#n�����Բ���
	endif
	if ret = 19
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�����ѥ#n�����Բ���
	endif
	
		add_talk #b#b����Ҫ�ύ����Ʒ�����·��Ŀ���
		
		NPC_COMMIT_OPT 201

	talk
endproc  

proc 201
	strvar ID fie gri ret
	GET_COMMIT_ITEM_INFO 1 ID
	
	GET_COMMIT_ITEM_INFO 2 fie
	;λ��
	GET_COMMIT_ITEM_INFO 3 gri
	;����
	
	get_task_var 4601 0 8 1 ret 
	if ret = 0
		if ID = 2019902
			DEC_ITEM_NUM_BYIDX 2019902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�Ұս����������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 1
		if ID = 2029902
			DEC_ITEM_NUM_BYIDX 2029902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ������£�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 2
		if ID = 2039902
			DEC_ITEM_NUM_BYIDX 2039902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵĿ�սװ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 3
		if ID = 2049902
			DEC_ITEM_NUM_BYIDX 2049902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵĵ���װ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 4
		if ID = 2119902
			DEC_ITEM_NUM_BYIDX 2119902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵľ���ͷ����������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 5
		if ID = 2129902
			DEC_ITEM_NUM_BYIDX 2129902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵķ���ͷ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 6
		if ID = 2139902
			DEC_ITEM_NUM_BYIDX 2139902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�������ñ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 7
		if ID = 2149902
			DEC_ITEM_NUM_BYIDX 2149902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�ʮ�����Σ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 8
		if ID = 2419902
			DEC_ITEM_NUM_BYIDX 2419902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ���װ��ߣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 9
		if ID = 2429902
			DEC_ITEM_NUM_BYIDX 2429902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�Ԫ�ػ��Σ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 10
		if ID = 2439902
			DEC_ITEM_NUM_BYIDX 2439902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ��ȳ����Σ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 11
		if ID = 2449902
			DEC_ITEM_NUM_BYIDX 2449902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵĽ�ͽ��ߣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 12
		if ID = 2219902
			DEC_ITEM_NUM_BYIDX 2219902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵľ�����ף�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 13
		if ID = 2229902
			DEC_ITEM_NUM_BYIDX 2229902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵķ��ļ�ף�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 14
		if ID = 1279902
			DEC_ITEM_NUM_BYIDX 1279902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�Ұ��II�ͣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 15
		if ID = 2249902
			DEC_ITEM_NUM_BYIDX 2249902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�ʮ�ּ�ף�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 16
		if ID = 2619902
			DEC_ITEM_NUM_BYIDX 2619902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�սʿѥ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 17
		if ID = 2629902
			DEC_ITEM_NUM_BYIDX 2629902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵķ���ѥ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 18
		if ID = 2639902
			DEC_ITEM_NUM_BYIDX 2639902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�ð��ѥ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 19
		if ID = 2649902
			DEC_ITEM_NUM_BYIDX 2649902 1 @fie @gri 1
			task_fini 4601 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�����ѥ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc

proc 300
	strvar ret
	
	dialog 3
	add_talk $_npc$:
	get_task_var 4602 0 8 1 ret	
	if ret = 0
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵĽ�յ�#n�����Բ���
	endif
	if ret = 1
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵľ���#n�����Բ���
	endif
	if ret = 2
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ���ʿ��#n�����Բ���
	endif
	if ret = 3
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�Ѫ�ȷ�â#n�����Բ���
	endif
	if ret = 4
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵĸ�������#n�����Բ���
	endif
	if ret = 5
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�������#n�����Բ���
	endif
	if ret = 6
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵİ׽��#n�����Բ���
	endif
	if ret = 7
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ����Ƕ�#n�����Բ���
	endif
	if ret = 8
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�ʨ��֮��#n�����Բ���
	endif
	if ret = 9
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ���������#n�����Բ���
	endif
	if ret = 10
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵĳ�����#n�����Բ���
	endif
	if ret = 11
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵĺ�ľ��#n�����Բ���
	endif
	if ret = 12
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�������#n�����Բ���
	endif
	if ret = 13
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�ս��֮��#n�����Բ���
	endif
	if ret = 14
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵĹ���Ȩ��#n�����Բ���
	endif
	if ret = 15
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵĶ�����#n�����Բ���
	endif
	if ret = 16
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�Ѫɫޱ#n�����Բ���
	endif
	if ret = 17
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ����ӡ��#n�����Բ���
	endif
	if ret = 18
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵľ�����Ԩ#n�����Բ���
	endif
	if ret = 19
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵĺϽ�ָ��#n�����Բ���
	endif
	if ret = 20
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�����II#n�����Բ���
	endif
	if ret = 21
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�����III#n�����Բ���
	endif
	if ret = 22
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�����I#n�����Բ���
	endif
	if ret = 23
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�����II#n�����Բ���
	endif
	if ret = 24
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�����III#n�����Բ���
	endif
	if ret = 25
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ��ش�II#n�����Բ���
	endif
	if ret = 26
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ��ش�III#n�����Բ���
	endif
	if ret = 27
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�����I#n�����Բ���
	endif
	if ret = 28
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�����II#n�����Բ���
	endif
	if ret = 29
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�����III#n�����Բ���
	endif
	if ret = 30
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�������#n�����Բ���
	endif
	if ret = 31
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ���ʿ��#n�����Բ���
	endif
	if ret = 32
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵĻ�Ӱ��#n�����Բ���
	endif
	if ret = 33
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ���֮��#n�����Բ���
	endif
	if ret = 34
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵķ���Ȩ��#n�����Բ���
	endif
	if ret = 35
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ��ǹ��#n�����Բ���
	endif
	if ret = 36
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�ʥ�ɵ�#n�����Բ���
	endif
	if ret = 37
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�ʥ����#n�����Բ���
	endif
	if ret = 38
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ������Ϣ#n�����Բ���
	endif
	if ret = 39
		add_talk #b#b����Ҫһ��#c186506��ɫƷ�ʵ�ʮ�ֽ���#n�����Բ���
	endif
	
		add_talk #b#b����Ҫ�ύ����Ʒ�����·��Ŀ���
		
		NPC_COMMIT_OPT 301

	talk
endproc  

proc 301
	strvar ID fie gri ret
	GET_COMMIT_ITEM_INFO 1 ID
	
	GET_COMMIT_ITEM_INFO 2 fie
	;λ��
	GET_COMMIT_ITEM_INFO 3 gri
	;����
	
	get_task_var 4602 0 8 1 ret 
	if ret = 0
		if ID = 1019902
			DEC_ITEM_NUM_BYIDX 1019902 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵĽ�յ���������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 1
		if ID = 1019903
			DEC_ITEM_NUM_BYIDX 1019903 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵľ��𵶣�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 2
		if ID = 1019904
			DEC_ITEM_NUM_BYIDX 1019904 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ���ʿ����������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 3
		if ID = 1019905
			DEC_ITEM_NUM_BYIDX 1019905 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�Ѫ�ȷ�â��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 4
		if ID = 1019906
			DEC_ITEM_NUM_BYIDX 1019906 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵĸ������У�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 5
		if ID = 1069902
			DEC_ITEM_NUM_BYIDX 1069902 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ������ܣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 6
		if ID = 1069903
			DEC_ITEM_NUM_BYIDX 1069903 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵİ׽�ܣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 7
		if ID = 1069904
			DEC_ITEM_NUM_BYIDX 1069904 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ����Ƕܣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 8
		if ID = 1069905
			DEC_ITEM_NUM_BYIDX 1069905 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�ʨ��֮�ܣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 9
		if ID = 1069906
			DEC_ITEM_NUM_BYIDX 1069906 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ��������ӣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 10
		if ID = 1109902
			DEC_ITEM_NUM_BYIDX 1109902 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵĳ����ȣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 11
		if ID = 1109903
			DEC_ITEM_NUM_BYIDX 1109903 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵĺ�ľ�ȣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 12
		if ID = 1109904
			DEC_ITEM_NUM_BYIDX 1109904 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ������ȣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 13
		if ID = 1109905
			DEC_ITEM_NUM_BYIDX 1109905 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�ս��֮�ȣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 14
		if ID = 1109906
			DEC_ITEM_NUM_BYIDX 1109906 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵĹ���Ȩ�ȣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 15
		if ID = 1189902
			DEC_ITEM_NUM_BYIDX 1189902 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵĶ����򣿲�����ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 16
		if ID = 1189903
			DEC_ITEM_NUM_BYIDX 1189903 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�Ѫɫޱ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 17
		if ID = 1189904
			DEC_ITEM_NUM_BYIDX 1189904 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ����ӡ�ǣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 18
		if ID = 1189905
			DEC_ITEM_NUM_BYIDX 1189905 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵľ�����Ԩ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 19
		if ID = 1189906
			DEC_ITEM_NUM_BYIDX 1189906 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵĺϽ�ָ�ƣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 20
		if ID = 1209902
			DEC_ITEM_NUM_BYIDX 1209902 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�����II��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 21
		if ID = 1209903
			DEC_ITEM_NUM_BYIDX 1209903 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�����III��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 22
		if ID = 1209904
			DEC_ITEM_NUM_BYIDX 1209904 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�����I��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 23
		if ID = 1209905
			DEC_ITEM_NUM_BYIDX 1209905 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�����II��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 24
		if ID = 1209906
			DEC_ITEM_NUM_BYIDX 1209906 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�����III��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 25
		if ID = 1289902
			DEC_ITEM_NUM_BYIDX 1289902 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ��ش�II��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 26
		if ID = 1289903
			DEC_ITEM_NUM_BYIDX 1289903 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ��ش�III��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 27
		if ID = 1289904
			DEC_ITEM_NUM_BYIDX 1289904 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�����I��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 28
		if ID = 1289905
			DEC_ITEM_NUM_BYIDX 1289905 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�����II��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 29
		if ID = 1289906
			DEC_ITEM_NUM_BYIDX 1289906 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�����III��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 30
		if ID = 1509902
			DEC_ITEM_NUM_BYIDX 1509902 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ������ȣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 31
		if ID = 1509903
			DEC_ITEM_NUM_BYIDX 1509903 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ���ʿ�ȣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 32
		if ID = 1509904
			DEC_ITEM_NUM_BYIDX 1509904 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵĻ�Ӱ�ȣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 33
		if ID = 1509905
			DEC_ITEM_NUM_BYIDX 1509905 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ���֮�ȣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 34
		if ID = 1509906
			DEC_ITEM_NUM_BYIDX 1509906 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵķ���Ȩ�ȣ�������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 35
		if ID = 1589902
			DEC_ITEM_NUM_BYIDX 1589902 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ��ǹ�磿������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 36
		if ID = 1589903
			DEC_ITEM_NUM_BYIDX 1589903 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�ʥ�ɵ䣿������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
	endif
	endif
	if ret = 37
		if ID = 1589904
			DEC_ITEM_NUM_BYIDX 1589904 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�ʥ���󣿲�����ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 38
		if ID = 1589905
			DEC_ITEM_NUM_BYIDX 1589905 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ������Ϣ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 39
		if ID = 1589906
			DEC_ITEM_NUM_BYIDX 1589906 1 @fie @gri 1
			task_fini 4602 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǰ�ɫƷ�ʵ�ʮ�ֽ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc

proc 400
	strvar ret
	
	dialog 3
	add_talk $_npc$:
	get_task_var 4603 0 8 1 ret	
	if ret = 0
		add_talk #b#b����Ҫһֻ#c186506������ﵰ#n�����Բ���
	endif
	if ret = 1
		add_talk #b#b����Ҫһֻ#c186506Ұ����ﵰ#n�����Բ���
	endif
	if ret = 2
		add_talk #b#b����Ҫһֻ#c186506�����˳��ﵰ#n�����Բ���
	endif
	if ret = 3
		add_talk #b#b����Ҫһֻ#c186506����������ﵰ#n�����Բ���
	endif
	if ret = 4
		add_talk #b#b����Ҫһֻ#c186506ͺ�ճ��ﵰ#n�����Բ���
	endif
	if ret = 5
		add_talk #b#b����Ҫһֻ#c186506�Ĺ��������ﵰ#n�����Բ���
	endif
	if ret = 6
		add_talk #b#b����Ҫһֻ#c186506���ݻ����ﵰ#n�����Բ���
	endif
	if ret = 7
		add_talk #b#b����Ҫһֻ#c186506Ы�ӳ��ﵰ#n�����Բ���
	endif
	
		add_talk #b#b����Ҫ�ύ����Ʒ�����·��Ŀ���
		
		NPC_COMMIT_OPT 401

	talk
endproc  

proc 401
	strvar ID fie gri ret
	GET_COMMIT_ITEM_INFO 1 ID
	
	GET_COMMIT_ITEM_INFO 2 fie
	;λ��
	GET_COMMIT_ITEM_INFO 3 gri
	;����
	
	get_task_var 4603 0 8 1 ret 
	if ret = 0
		if ID = 5610100
			DEC_ITEM_NUM_BYIDX 5610100 1 @fie @gri 1
			task_fini 4603 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ�������������ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 1
		if ID = 5610101
			DEC_ITEM_NUM_BYIDX 5610101 1 @fie @gri 1
			task_fini 4603 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ�������Ұ����ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 2
		if ID = 5610102
			DEC_ITEM_NUM_BYIDX 5610102 1 @fie @gri 1
			task_fini 4603 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������������˳��ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 3
		if ID = 5611116
			DEC_ITEM_NUM_BYIDX 5611116 1 @fie @gri 1
			task_fini 4603 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ�����������������ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 4
		if ID = 5610104
			DEC_ITEM_NUM_BYIDX 5610104 1 @fie @gri 1
			task_fini 4603 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ�������ͺ�ճ��ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 5
		if ID = 5610106
			DEC_ITEM_NUM_BYIDX 5610106 1 @fie @gri 1
			task_fini 4603 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ��������Ĺ��������ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 6
		if ID = 5610107
			DEC_ITEM_NUM_BYIDX 5610107 1 @fie @gri 1
			task_fini 4603 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ������ǽ��ݻ����ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	if ret = 7
		if ID = 5610105
			DEC_ITEM_NUM_BYIDX 5610105 1 @fie @gri 1
			task_fini 4603 1
			call 101
		else
			add_sys_msg_start
			add_sys_msg ��ȷ�������Ы�ӳ��ﵰ��������ô�����˵ģ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc

proc 101
	add_sys_msg_start
;	add_sys_msg �ɹ��Ͻ���Ʒ���������
	set_sys_msg_mode 1 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc 

