;��ţԤ��,����
proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�����������ң�����·����ͨ���ɹ������Ŀ�ׯ������������룬��ٰ����������Ҹ��������Դ��������ҵ��𰸡�
;	add_talk #c186506ţ�к�æ����ٵ����յ���ϣ�
	task_add_option 30022 1 90
	task_stat 30022 1 2
	if 2 != 0 and 2 != 3
		option 100 ѡ������
	endif
	get_id_player_info 0 8 10
	task_stat 30024 1 2
	task_stat 30025 1 3
	task_stat 30026 1 4
	task_stat 30027 1 5
	if 2 = 3 and 3 = 3 and 4 = 3 and 5 = 3 and 10 >= 45
	else
		option 88 ��������ѡ��������б�
	endif
	
	option 89 ţ�к�æ����ٵ����յ����
	talk
endproc
proc 2
	get_trig_task 1 2 
	if 2 = 30022 
		call 100 
	endif
endproc
proc 88
	dialog 1
	add_talk $_npc$:
;	add_talk #b#b�����������ң�����·����ͨ���ɹ������Ŀ�ׯ������������룬��ٰ����������Ҹ��������Դ��������ҵ��𰸡�
	add_talk #b#b��Ҫ�ڳ����бȱ��˸�ʤһ������Ҫӵ���ٶ����ϵĽ��ݻ����Ա�������������Ұ���ɳĮ����������
	add_talk #b#bֻҪ����������񣬾ͻἤ������������е�����ѡ������ڱ�����ӵ����ʤ����Ȩ��
	add_talk #b#b#c186506�ر�����������ѡ��Ϊ���ü���Ӵ��
	
	task_add_option 30024 1 91
	task_add_option 30025 1 92
	task_add_option 30026 1 93
	task_add_option 30027 1 94
	option 1 ����
	talk
endproc
proc 89
	dialog 1
	add_talk $_npc$:
;	add_talk #b#b�����������ң�����·����ͨ���ɹ������Ŀ�ׯ������������룬��ٰ����������Ҹ��������Դ��������ҵ��𰸡�
	add_talk #b#b#c1865061����ȡ���񣬻������
	add_talk #b#b#c1865062�������������������˳�ֵ�λ��
	add_talk #b#b#c1865063�����ηǶ�����ң���ݼ�F11��F12
	add_talk #b#b#c1865064�����ҶԻ���ѡ������
	add_talk #b#b#c1865065��ѡ��������裨�����Ҹ�����
	add_talk #b#b#c1865066�������յ��վ������|����|�յ�
	add_talk #b#b#c1865067��������ʹ������
	add_talk #b#b#c1865068�����ϵ�������#n
	
	option 1 ����
	talk
endproc
	
proc 90
	task_do_option 30022  1
endproc
proc 91
	task_do_option 30024  1
endproc
proc 92
	task_do_option 30025  1
endproc
proc 93
	task_do_option 30026  1
endproc
proc 94
	task_do_option 30027  1
endproc
proc 100
	dialog 1
	add_talk #b#b������Դ���ޣ��ҿ����ṩ��һͷ�ɰ���С�������������ţ�к�æ��������Ӵ��
	add_talk #b#b����㼤������������ѡ��ҽ����㲻Ҫѡ�����ṩ���С������Ȼ�ɰ���ȴ����Ұ��ɳĮ����������Ҳ���罣�ݻ��������Ա��ٶȿ졣
	add_talk #b#b�ر�������ѡ��Ϊ���ü���Ӵ��
	add_talk #b#bע����Ҫ�ڴ��˷�̫��ʱ��Ӵ����Ѿ���ʼ��ʱ��Ӵ��
	
	task_stat 30022 1 2
	if 2 != 0 and 2 != 3
		option 110 #����һͷ�ɰ���С��
	endif
	task_stat 30024 1 2
	if 2 = 3 
		option 120 #��ͷ������������Ұ��
	endif
	task_stat 30025 1 2
	if 2 = 3 	
		option 130 #���ݻ�һ��˭�Ҿ���
	endif
	task_stat 30026 1 2
	if 2 = 3 
		option 140 #��ͷ������·��ɳĮ����
	endif
	task_stat 30027 1 2
	if 2 = 3 
		option 150 #�����Ա��ٶ�����
	endif
	task_stat 30024 1 2
	task_stat 30025 1 3
	task_stat 30026 1 4
	task_stat 30027 1 5
	if 2 = 3 and 3 = 3 and 4 = 3 and 5 = 3
	else
		option 88 ��������ѡ��������б�
	endif
	option 1 ����
	talk
endproc
proc 110
	GET_TASK_VAR 30022 0 32 1 2
	if 2 != 0
		add_sys_msg_start
		add_sys_msg ��ͬ����һ�����㲻�ܺ�ڣ���Ȼѡ���ˣ��ͼ�ֵ��װɣ�  
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	NEW_CONVOY_MON 241010 20600 31800 200 0 0 0 1800 30022 1 2
	SET_TASK_VAR 30022 0 32 @2 1	
	call 111	
endproc
proc 111
	add_sys_msg_start
	add_sys_msg ������ʹ����Ʒ��̢֮��ʹ���ƶ�
	set_sys_msg_mode 3 0 0
	msg_info
	var 11 0
	get_time_sec 11
	;timebegin11,��ʼ��ţʱ��
	
	set_task_var 30022 4 32 @11 1
endproc
proc 120
	GET_TASK_VAR 30022 0 32 1 2
	if 2 != 0
		add_sys_msg_start
		add_sys_msg ��ͬ����һ�����㲻�ܺ�ڣ���Ȼѡ���ˣ��ͼ�ֵ��װɣ�  
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	NEW_CONVOY_MON 241011 20600 31800 200 0 0 0 1800 30022 1 2
	SET_TASK_VAR 30022 0 32 @2 1	
	call 111	
endproc
proc 130
	GET_TASK_VAR 30022 0 32 1 2
	if 2 != 0
		add_sys_msg_start
		add_sys_msg ��ͬ����һ�����㲻�ܺ�ڣ���Ȼѡ���ˣ��ͼ�ֵ��װɣ�  
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	NEW_CONVOY_MON 241012 20600 31800 200 0 0 0 1800 30022 1 2	
	SET_TASK_VAR 30022 0 32 @2 1
	
	call 111	
endproc
proc 140
	GET_TASK_VAR 30022 0 32 1 2
	if 2 != 0
		add_sys_msg_start
		add_sys_msg ��ͬ����һ�����㲻�ܺ�ڣ���Ȼѡ���ˣ��ͼ�ֵ��װɣ�  
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	NEW_CONVOY_MON 241013 20600 31800 200 0 0 0 1800 30022 1 2	
	SET_TASK_VAR 30022 0 32 @2 1
	call 111
endproc
proc 150
	GET_TASK_VAR 30022 0 32 1 2
	if 2 != 0
		add_sys_msg_start
		add_sys_msg ��ͬ����һ�����㲻�ܺ�ڣ���Ȼѡ���ˣ��ͼ�ֵ��װɣ�  
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	NEW_CONVOY_MON 241014 20600 31800 200 0 0 0 1800 30022 1 2
	SET_TASK_VAR 30022 0 32 @2 1	
	call 111	
endproc
