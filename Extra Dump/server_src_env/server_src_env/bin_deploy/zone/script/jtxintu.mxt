
;��ͽ
proc 1
	DIALOG 1
	add_talk $_npc$:
	strvar randtalk
	rand randtalk 6
	switch randtalk
		case 0
			add_talk #b#b����ĩ�մ��٣�ֻ���ŷ�ݶ�˹����ܵþȣ�
		endcase
		case 1
			add_talk #b#b���Ҳ���������ʥ������λ�������οࡣ
		endcase
		case 2
			add_talk #b#b���ݶ�˹����Ⱦ��ң��������ҳϵ���ͽ��
		endcase
		case 3
			add_talk #b#b�ҵ��������ҵ�����׸������𾴵İ��ݶ�˹��
		endcase
		case 4
			add_talk #b#b��Ѫ������ϴ���������õ�ҩ����
		endcase
		case 5
			add_talk #b#b���ݶ�˹����ն�����һ�п��ƺͲ��ҵĸ�Դ���������ǰɣ�
		endcase
	endswitch	
	
	
	strvar Ret
	TASK_STAT 1561 1 Ret
	get_task_var 1561 4 32 1 lock 
	if Ret = 2 and lock != 10
	
		option 99 ��ã��ҳϵ���ͽ��

	endif
	strvar Ret1
	TASK_STAT 1569 1 Ret1
	get_task_var 1569 0 8 1 55
	if Ret1 = 2 and 55 = 0
		option 500 #����ǣ������ߣ��ɴ���ȥ��
	endif
	
	talk
endproc 
proc 99
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b������ʲô����

		option 100 �м�����������æ��
		option 200 ÷����Ůʿ����������ġ�
	talk
endproc 

proc 100
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ʲô�¾������˵�ɣ��һ�Ҫ������÷����Ůʿ�Ľ̻塣
		option 101 ���ǻ��ܣ��ܷ��һ��˵����
		option 102 �Ǳ��и�Ư����ü���㡣
	talk
endproc
proc 101
;;;;;;;;;;;;;return
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���ܣ��ҵ�����Ѿ��׸����ݶ�˹������֪�������ܶ����������������ĸ���÷����Ůʿ����ȥ��������ȵȡ���
		option 300 #����
	talk
endproc
proc 102 
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bŶ��Ư����ü��
		option 103 �ǵģ�˵����Ľ���Ѿ�
		
	talk
endproc
proc 103
;;;;;;;;;;;;;return
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ҵ�����Ѿ��׸����ݶ�˹���ҵ������Ѿ��׸��˰��ݶ�˹�������е�һ�ж�ֻ���ڰ��ݶ�˹���벻Ҫ����Ҷ�����ֵ��ҳϣ�
		option 300 #����
		
	talk
endproc
 

proc 300
	return
endproc


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
proc 200
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b÷����Ůʿ�������ڽ�������ô�ֻ����������ң�
		option 201 �������ɫ�ı��֣�Ůʿ�������������
		option 210 ÷����Ůʿ����˼���㲻��Ҫ֪��
		
	talk

endproc

proc 201
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bʲô���᣿��ɫ�ı��֣�

		option 202 ��ѽ��Ůʿ������ȥִ����������
	talk
endproc
proc 202
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���Ǹղ�÷����Ůʿ��˵����������һȺ���ң���Ҫ�������ܹ���Ϊ���ݶ�˹��Ч����
		option 300 #�Ǹ��������Ҵ����ˡ���
	talk
endproc
proc 210
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���٣�����÷����Ůʿ��ʲô�Ը���

		option 211 #�ϻ���˵�������ߣ�
	talk
endproc


proc 211
	strvar Mid lock 
	PLAYER_CALL_BUB 213033 180 Mid
	set_task_var 1561 0 32 @Mid 1
	
	var lock 10
	set_task_var 1561 4 32 @lock 1
endproc


proc 500
	strvar Mid1 Mid2 Mid3 
	PLAYER_CALL_BUB 213034 240 Mid1
	PLAYER_CALL_BUB 213034 240 Mid2
	PLAYER_CALL_BUB 213034 240 Mid3
	
	set_task_var 1569 0 32 @Mid1 10
	set_task_var 1569 4 32 @Mid2 11
	set_task_var 1569 8 32 @Mid3 12
	
	if Mid1 != -1 and Mid2 != -1 and Mid3 != -1
		get_task_var 1569 0 8 1 10
		var 10 1
		set_task_var 1569 0 8 @10 1
		add_sys_msg_start
		add_sys_msg ��ͽ��չ���������ĵ�ʱ�̵��ˣ�
		set_sys_msg_mode 3 0 0
		msg_info
		
		
	endif
	
endproc 










