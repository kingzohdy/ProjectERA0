
;С�̸�
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������������Ʒ����������ˣ�#Gender3#���������
	add_talk #b#b<��ĳ�����ô����>
	add_talk #b#bֻҪ5W���ң���û����������ô���ˣ�����������Ӵ��#003#003#003
	add_talk #b#b<ֻ��5W�����򵽵���һֻ����ֶ��ǵ���ͨ����ɣ�>
	add_talk #b#b#031#031#031����������ô��
	add_talk #b#b<������Щ�ɶ��ƭ�ӣ���Ȼ��װ�ɿɰ�СŮ�������ȣ���ƭ�������е��ģ��������������ѵ��ѵ�㣬�����Ժ󻹸Ҳ�Ҳ����ƭ�ˣ�>
		
	
	get_task_var 30011 0 8 1 20
	task_stat 30011 1 30

	if 20 = 1 and 30 = 2
		option 150 #������Щƭ�ӣ�¶������Ŀ�ɣ�
	endif
	talk
endproc

proc 150
	strvar num
	GET_MAP_MON_NUM 4400 241007 1 num 1
;	GET_MON_NUM 1 num
	if num = 0
		NEW_MON 241007 1 35200 39300 0 0 0 0 0
		add_sys_msg_start
		add_sys_msg ƭ��֮�����ܳɹ�������Ϊ�˵�̰����ֻҪ�������뷨��ƭ�Ӿ��ܳ���ֱ��
		set_sys_msg_mode 3 0 0
		msg_info
	else
		add_sys_msg_start
		add_sys_msg С��Ӵ�����Ѿ�����Ϸƭ�Ӷ����ˣ�ע��鿴���ɵ�СŮ��������Χ
		set_sys_msg_mode 3 0 0
		msg_info
	
	endif
endproc 
