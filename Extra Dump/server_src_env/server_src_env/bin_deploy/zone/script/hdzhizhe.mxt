
;����
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b��嫵������У��������ͬ��΢�����ǳ���
	add_talk #b#b������ϣ��̽������İ��أ�Ȼ���ܰ������ǵ�ֻ���޾����ǻۡ�
	
	option 151 ���������롿�
	option 150 ������������˵��
	
	talk
endproc

proc 150

	dialog 1
	add_talk $_npc$:
	add_talk #b#b�μӾ�����ʱ�򣬽���ǰ����ȫ�����������������ʧ��
	add_talk #b#b�������ֺʹ����ٶȵĿ����йأ�ѡ����ȷ�𰸵��ٶ�Խ�죬�÷�Խ�ߡ�
	add_talk #b#b���ģʽ���������ڼ䣬������֮�ǣ����Կ������ģʽ���ڴ���ʱ�Զ�ѡ��𰸡�
	add_talk #b#b��������������������ȼ��йأ�ͬʱ����Խ�ߣ�����Խ�ߣ� 
	talk
endproc 

proc 151
	dialog 1
	add_talk $_npc$:
	add_talk #c186506�ʱ�䣺
	add_talk #b#bÿ���κ�ʱ��#n
	add_talk #c005ebe��淨��
	add_talk #b#bʱ���ڰ�������ȷ�ĺ���˳��������ƺ���#n
	add_talk #b#b
	add_talk #c6d00d2�������
	add_talk #b#b1. ˳�����10����Ŀ��������ȡ���ҽ���
	add_talk #b#b2. �ɹ�����Խ�࣬��������Խ��#n
	add_talk #b#b
	add_talk #c186506�μ�������
	add_talk #b#bÿ�βμӻ��Ҫ����3��������Կ�ס�#n
	
	option 154 �μӡ��������롿�
	option 155 �����ҵ�������Կ�ס�
	option 1 ����
	
	talk
endproc 

proc 152
	strvar num ret
	GET_ITEM_NUM_INPAK 5063100 0 num
	if num >= 3
		enter_pworld 250 1 ret
		if ret = 0
			DEC_ITEM_NUM_INPAK 5063100 3 0
		endif
	else
		add_sys_msg_start
		add_sys_msg �������Ҳ���3��������Կ�ס�����ʱ�޷��μӡ��������롿�
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc

proc 154
	;enter_pworld 250 1 1
	choose 2 152 153 ���롰�����ؾ�����Ҫ����3��������Կ�ס�\n�Ƿ�ȷ��ǰ���������ؾ�����
endproc 

proc 155
	dialog 1
	add_talk $_npc$:
	add_talk Կ����;��
	add_talk #b#bÿ�βμӻ��Ҫ����3��������Կ�ס�
	add_talk #b#b
	add_talk #c186506��ȡ;����
	add_talk #b#b1. ��ɱ60������ר�Ҹ���������ͷĿ����
	add_talk #b#b2. �ɼ������ּ��껪����ı����м��ʵ���
	add_talk #b#b3. ��ɹ��ỷ��������#n
	
	option 151 ����
	
	talk
endproc
