;��ţ����,�ƶ�
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���Һ�С��ʱ��үү����ȥ��̲���棬����������һ��ɳ����ɳ̲�ϣ������ܷ�����ո��ӵ�������ɳ���һ�����
	add_talk #b#b����������һ��ʲô����ɳ�ӣ�
	add_talk #b#b��˵����һ���������������ɳ�ӡ�
	add_talk #b#b�����˺ܳ�ʱ�䣬����û���ҵ�����
	add_talk #b#b���ܶ����ȥ�ˣ����ڲ�֪������Ҳ�����ɳ̲�е�һ��ɳ��
	add_talk #c186506ţ�к�æ-�������ʱ�䰲�����£�
	add_talk #b#b#c186506ÿ��һ������20��30����21��00
	add_talk #b#b#c186506ÿ����������20��30����21��00#n
	add_talk #b#b�μ�ţ�к�æ������Ҫ�ǹ����볡ȯ��ֻ���ڳ�������������˲��ܻ�ô�ȯ����ý����ǹ������ʸ�
	
	option 100 #�μ�ţ�к�æ����
	
	talk
endproc
proc 100
	cow_pworld_staus 1
	if 1 != 1
		add_sys_msg_start
		add_sys_msg �ܱ�Ǹ�������ڻ����ܽ�ȥ����Ϊ���ڲ��ǻʱ�䣡  
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	strvar Ret
	GET_VAILD_ITEM_NUM_INPAK 5052207 1 Ret
	if Ret < 1
		add_sys_msg_start
		add_sys_msg �����ź���ֻ��ӵ���ǹ����볡ȯ���˲��ܽ������в��������  
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	COW_PWORLD_ENTER 1
	
	if 1 = 0
		DEC_VAILD_ITEM_NUM_INPAK 5052207 1 Ret
	else
		
	endif
endproc
