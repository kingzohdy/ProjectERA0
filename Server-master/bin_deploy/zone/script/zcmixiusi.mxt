proc 1


	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����·��ƽ�˾��ĺ��棬�ں����̲�����������δ֪��в��һ��С�������������
	add_talk #b#b���ڴ˵�Ŀ�ľ�����ʵ���㹻����ʿί�Ը�����������Ρ�
	add_talk #b#b
	add_talk #c186506��������[50��]-����ﵽ50������ڴ˽�ȡ���������������ɺ󽫻��ϡ�н���

	task_add_option 3027 1 100
	; task_stat 3027 1 10
	task_add_option 3028 1 200
	; task_stat 3028 1 10
	task_add_option 3029 1 300
	; task_stat 3029 1 10
	task_add_option 3030 1 400
	; task_stat 3030 1 10
	task_add_option 3031 1 500
	; task_stat 3031 1 10
	
	strvar num num1 num2 num3 ret
	GET_ITEM_NUM_INPAK 5062200 0 num
	GET_ITEM_NUM_INPAK 5062201 0 num1
	GET_ITEM_NUM_INPAK 5062202 0 num2
	GET_ITEM_NUM_INPAK 5062203 0 num3
	if num > 0 or num1 > 0 or num2 > 0 or num3 > 0
		OPTION 600 #������Щ��ʧ������
	else
		OPTION 605 #��֪����ʧ��������
	endif

	talk
endproc 

proc 100
	task_do_option 3027 1
endproc 

proc 200
	task_do_option 3028 1
endproc

proc 300
	task_do_option 3029 1
endproc

proc 400
	task_do_option 3030 1
endproc

proc 500
	task_do_option 3031 1
endproc

proc 600
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c676464<����˹���ص������Žӹ���������>#n
	add_talk #b#b�����ػ���ʧ��֮����ȱ��������������
	add_talk #b#b������Щ�����ң�ϣ�����ܰ��������ø��ҡ�
		
	OPTION 601 ��������ʧ�ı��������һ�150����
	OPTION 602 ��������ʧ�Ľ��С����һ�30����
	OPTION 603 ��������ʧ�Ľ��������һ�20����
	OPTION 604 ��������ʧ�Ľ��ʡ����һ�20����
	
	talk
endproc

proc 601
	strvar num ret
	GET_ITEM_NUM_INPAK 5062200 0 num
	if num > 0
		DEC_ITEM_NUM_INPAK 5062200 1 ret
		if ret = 0
			ADD_MONEY 0 150 20 1
			
			add_sys_msg_start
			add_sys_msg ��ӹ�����˹�����ң������ˡ���ʧ�ı�����
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg ����ʧ�ı������ѱ��𻵣���ʱ�޷��һ�����
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg �������Ҳ�������ʧ�ı���������ʱ�޷��һ�����
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc 

proc 602
	strvar num ret
	GET_ITEM_NUM_INPAK 5062201 0 num
	if num > 0
		DEC_ITEM_NUM_INPAK 5062201 1 ret
		if ret = 0
			ADD_MONEY 0 30 20 1
			
			add_sys_msg_start
			add_sys_msg ��ӹ�����˹�����ң������ˡ���ʧ�Ľ��С�
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg ����ʧ�Ľ��С��ѱ��𻵣���ʱ�޷��һ�����
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg �������Ҳ�������ʧ�Ľ��С�����ʱ�޷��һ�����
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc 

proc 603
	strvar num ret
	GET_ITEM_NUM_INPAK 5062202 0 num
	if num > 0
		DEC_ITEM_NUM_INPAK 5062202 1 ret
		if ret = 0
			ADD_MONEY 0 20 20 1
			
			add_sys_msg_start
			add_sys_msg ��ӹ�����˹�����ң������ˡ���ʧ�Ľ�����
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg ����ʧ�Ľ������ѱ��𻵣���ʱ�޷��һ�����
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg �������Ҳ�������ʧ�Ľ���������ʱ�޷��һ�����
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc 

proc 604
	strvar num ret
	GET_ITEM_NUM_INPAK 5062203 0 num
	if num > 0
		DEC_ITEM_NUM_INPAK 5062203 1 ret
		if ret = 0
			ADD_MONEY 0 20 20 1
			
			add_sys_msg_start
			add_sys_msg ��ӹ�����˹�����ң������ˡ���ʧ�Ľ��ʡ�
			set_sys_msg_mode 3 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg ����ʧ�Ľ��ʡ��ѱ��𻵣���ʱ�޷��һ�����
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg �������Ҳ�������ʧ�Ľ��ʡ�����ʱ�޷��һ�����
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc 

proc 605
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��˵����ħ������ʧ��֮�������ɱ��Ϊ���ػ���������������������Ǽ���������ǰ�ߡ�
	add_talk #b#b���ź�������������ȫ����û��ֻ������Щ��������������
	add_talk #b#b#c186506����������ҵ���Щ��ʧ����������Ը�����������������£����ߵ�Ӣ��Ӧ�õ���ׯ�ϵĶԴ���#n
	add_talk #b#b
	add_talk #c005ebe��ȡ;����
	add_talk #b#bʧ��֮��Ĺ����м��ʵ��䡰ʧ��Ľ��С���ʧ��Ľ�������ʧ��Ľ��ʡ�#n
	add_talk #c6d00d2�����ϳɣ�
	add_talk #b#b��ʧ��Ľ��С���ʧ��Ľ�������ʧ��Ľ��ʡ��ɺϳɡ�ʧ��ı�����#n
	
	OPTION 1 #����

	talk
endproc
