;����֮�䣨��ǿ����  ����  302043   ����ID��51500

proc 1
	DIALOG 1
	add_talk �ۼ��ֿ���ϣ� 
	add_talk #b#b���ٷֱ�����ˣ�С���ѡ�·�ˣ������ˣ�ǿ����ආ������������ɱ���ˣ����ܷ�����
	add_talk #b#b
	add_talk #c108650��������
	add_talk #b#b#c108650С����    -3��
	add_talk #b#b#c108650·��      -5��
	add_talk #b#b#c108650ǿ��      +5��
	add_talk #b#b#c108650ආ�      +3��
	add_talk #b#b#c108650���      +6��#n
	add_talk #b#b
	add_talk ʤ����������ɱǿ�������ܵķ�����100
	
	strvar sta1 sta4 ret
	TASK_STAT 4615 ret 11
	if @11 = 2 and @ret = 0
		GET_PWORLD_VAR 0 32 20 sta1
		if @sta1 = 0
			option 200 #��ʼ��ս
		endif	
	endif

	option 201 #лл���ָ��
	
	talk
endproc 

proc 200
	;0λ��ʾ��ʼˢ�����ı�־   ����ֵΪ1��ʾ��ʼ��սѡ�������
	set_pworld_var 0 32 1 1
	add_sys_msg_start
	add_sys_msg ������ǰ����Ӱ����ʱ�����ٷֱ�����뻵�ˣ���ɱ���˵÷֣�
	set_sys_msg_mode 4 0 0
	msg_info
	
	;������0
	set_pworld_var 8 32 0 1
endproc

proc 201
	PLAYER_CLOSE_DIALOG1 0 0
endproc

