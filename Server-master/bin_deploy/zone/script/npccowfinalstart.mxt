;��ţ����,����2
proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�����������ң�����·����ͨ���ɹ������Ŀ�ׯ������������룬��ٰ����������Ҹ��������Դ��������ҵ��𰸡�
	task_add_option 30023 1 90
	task_stat 30023 1 2
	if 2 != 0 and 2 != 3
		option 100 ѡ������
	endif
	option 89  ţ�к�æ����ٵ����յ����
	talk
endproc
proc 2
	get_trig_task 1 2 
	if 2 = 30023 
		call 100
	endif
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
	task_do_option 30023  1 90
endproc
proc 100
	dialog 1
	add_talk #b#b������Դ���ޣ��ҿ����ṩ��һͷ�ɰ���С�������������ţ�к�æ��������Ӵ��
	add_talk #b#b����㼤������������ѡ��ҽ����㲻Ҫѡ�����ṩ���С������Ȼ�ɰ���ȴ����Ұ��ɳĮ����������Ҳ���罣�ݻ��������Ա��ٶȿ졣
	add_talk #b#b�ر�������ѡ��Ϊ���ü���Ӵ��
	add_talk #b#bע����Ҫ�ڴ��˷�̫��ʱ��Ӵ����Ѿ���ʼ��ʱ��Ӵ��
	
	task_stat 30023 1 2
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
	option 1 ����
	talk
endproc
proc 110
	GET_TASK_VAR 30023 0 32 1 2
	if 2 != 0
		add_sys_msg_start
		add_sys_msg ��ͬ����һ�����㲻�ܺ�ڣ���Ȼѡ���ˣ��ͼ�ֵ��װɣ�  
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	NEW_CONVOY_MON 241015 17936 33838 200 0 0 0 1800 30023 1 2
	SET_TASK_VAR 30023 0 32 @2 1	
	call 111	
endproc
proc 111
	add_sys_msg_start
	add_sys_msg ������ʹ����Ʒ�޲�֮��ʹ���ƶ�
	set_sys_msg_mode 3 0 0
	msg_info
	var 11 0
	get_time_sec 11
	;timebegin11,��ʼ��ţʱ��
	
	set_task_var 30023 4 32 @11 1
endproc
proc 120
	GET_TASK_VAR 30023 0 32 1 2
	if 2 != 0
		add_sys_msg_start
		add_sys_msg ��ͬ����һ�����㲻�ܺ�ڣ���Ȼѡ���ˣ��ͼ�ֵ��װɣ�  
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	NEW_CONVOY_MON 241016 17936 33838 200 0 0 0 1800 30023 1 2
	SET_TASK_VAR 30023 0 32 @2 1	
	call 111	
endproc
proc 130
	GET_TASK_VAR 30023 0 32 1 2
	if 2 != 0
		add_sys_msg_start
		add_sys_msg ��ͬ����һ�����㲻�ܺ�ڣ���Ȼѡ���ˣ��ͼ�ֵ��װɣ�  
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	NEW_CONVOY_MON 241017 17936 33838 200 0 0 0 1800 30023 1 2	
	SET_TASK_VAR 30023 0 32 @2 1
	
	call 111	
endproc
proc 140
	GET_TASK_VAR 30023 0 32 1 2
	if 2 != 0
		add_sys_msg_start
		add_sys_msg ��ͬ����һ�����㲻�ܺ�ڣ���Ȼѡ���ˣ��ͼ�ֵ��װɣ�  
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	NEW_CONVOY_MON 241018 17936 33838 200 0 0 0 1800 30023 1 2	
	SET_TASK_VAR 30023 0 32 @2 1
	call 111
endproc
proc 150
	GET_TASK_VAR 30023 0 32 1 2
	if 2 != 0
		add_sys_msg_start
		add_sys_msg ��ͬ����һ�����㲻�ܺ�ڣ���Ȼѡ���ˣ��ͼ�ֵ��װɣ�  
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	NEW_CONVOY_MON 241019 17936 33838 200 0 0 0 1800 30023 1 2
	SET_TASK_VAR 30023 0 32 @2 1	
	call 111	
endproc
