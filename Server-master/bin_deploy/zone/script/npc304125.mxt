;npc����    ���������˶�

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ƽ��ϲ�����˶��ˣ��˶����Ҵ������������Ȥ����Ը�����һ�����˶���
	add_talk #b#b�����ڼ䣬ÿ���Ҷ���ٰ����������������˶������ϣ�������ո�һ�յ�������һ�����˶�Ӵ��
	add_talk #b#b
	add_talk #b#b#c186506�ʱ�䣺#n
	add_talk #b#b#b#b#c18650614��00 - 14��10#n
	add_talk #b#b#b#b#c18650619��00 - 19��10#n

	option 201 �����
	option 200 #�˽⣬�һ�׼ʱ���ģ�
	
	strvar rrRet
	get_role_var 57 8 rrRet 99
	if @rrRet = 0 and @99 = 1
		option 202 #��ȡ����
	endif
		
	talk
endproc 

proc 200
	PLAYER_CLOSE_DIALOG1 0 0
endproc

proc 201
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���ʼ���һ��Ƚ�������
	add_talk #b#b��ʽ��ʼ���һ�ָ��ĳ����������
	add_talk #b#b����#c186506�۹�������������Ӧ�Ķ�������#n����
	add_talk #b#bÿ������һ���������飬��ͻ��ý���
	add_talk #b#b��������������п��ܻ���Ů�������
	add_talk #b#b����������ȻҲ���ܵ���СС�ĳͷ�
	
	option 1 ������
	talk
endproc

proc 202
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣬�޷���ȡ
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	ADD_ITEM_NUM_INPAK 5043267 1 1 10
	if @10 = 0
		set_role_var 57 8 0 1
	endif
endproc
