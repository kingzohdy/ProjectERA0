

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���������������򲻵�����ƷŶ��
	add_talk #b#b��ͨ��#c186506��˹��#n��#c186506��³³#n�����ʥ���ǹ��һ���
	add_talk #b#b#c186506��˹����#n#c0181f908:00-24:00#n��ÿ10�������������ĳЩ��ͼ�У��ҵ������á���ʥ�ǹ���ȯ���뽻�������7�첮��ʱװ������ʥ���ǹ���
	add_talk #b#b#cFF4e00�������������ѯ��˹�����ֵ�λ��Ŷ��#n	
	add_talk #b#b#c186506��³³��#n#c0181f919:00-23:00#n���ֱ���ʱ��ǻ�ϸ�����ꡢ����������ɳĮ�����ջ�Ұ��Խ�����ֵİ�ȫ����ÿ��Сʱ�����������ȡ1����ʥ���ǹ���
	add_talk #b#b#cFF4e00ÿһ����³³�����ǹ�����ȡ������#n
	
		OPTION 110 #(����)���̵�
		
	strvar ret
	
		GET_EVENT_STAT ret 104
		if ret = 1
			OPTION 200 #��˹������ȥ�ˣ�
		endif

	talk
endproc 


PROC 110
	OPEN_NPC_SHOP
ENDPROC

PROC 200
	strvar sj tk ql bd xy sm hy yl ld lg ms

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c186506��˹��#nÿ10�������������ĳЩ��ͼ�У��ҵ������á���ʥ�ǹ���С��������������7�첮��ʱװ������ʥ���ǹ���
	add_talk #c186506����˹����λ�ã�#n
	
	GET_EVENT_VAR_BY_ID 104 0 32 0 sj
	GET_EVENT_VAR_BY_ID 104 4 32 0 tk
	GET_EVENT_VAR_BY_ID 104 8 32 0 ql
	GET_EVENT_VAR_BY_ID 104 12 32 0 bd
	GET_EVENT_VAR_BY_ID 104 16 32 0 xy
	GET_EVENT_VAR_BY_ID 104 20 32 0 sm
	GET_EVENT_VAR_BY_ID 104 24 32 0 hy
	GET_EVENT_VAR_BY_ID 104 28 32 0 yl
	GET_EVENT_VAR_BY_ID 104 32 32 0 ld
	GET_EVENT_VAR_BY_ID 104 36 32 0 lg
	GET_EVENT_VAR_BY_ID 104 40 32 0 ms
	
		if @sj = 1000
			add_talk #b#b����#c186506ʱ���#n
		endif
		if @bd = 2202
			add_talk #b#b����#c186506����#n
		endif
		if @xy = 2200
			add_talk #b#b����#c186506����ѩԭ#n
		endif
		if @sm = 2300
			add_talk #b#b����#c186506����ɳĮ#n
		endif
		if @hy = 2400
			add_talk #b#b����#c186506���ջ�Ұ#n
		endif
		if @yl = 2500
			add_talk #b#b����#c186506Խ������#n
		endif
		if @ld = 2104
			add_talk #b#b����#c186506���ֵ�#n
		endif
		if @lg = 2201
			add_talk #b#b����#c186506����ѹ�#n
		endif
		if @ms = 3404
			add_talk #b#b����#c186506��ɳ�ݵ�#n
		endif
		if @sj = 0 and @tk = 0 and @ql = 0 and @bd = 0 and @xy = 0 and @sm = 0 and @hy = 0 and @yl = 0 and @ld = 0 and @lg = 0 and @ms = 0
			add_talk #b#b#c186506û��֪����˹��ȥ���ˣ�Ҳ�������ٴ�̤�����µ���;#
		endif
	
	OPTION 1 #����
	
	TALK
ENDPROC

