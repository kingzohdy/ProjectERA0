;��ķ

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Ư����뱲�ӣ����޶�������������Ҫ�ľ���һ���ȶ��ļ�԰�����Ը�������ݹ�����
	add_talk #b#b#Name#������ƴ����ʱ������˻ؼң���Ȼ���������̫���ʣ�������ʼ������ıܷ�ۣ��������������˲�����
	add_talk #b#b��������ô��
		option 100 �����԰
		option 200 ��԰��ѯ������
		option 300 ��԰����
		option 400 �𶹶��һ�
	
	talk
endproc

proc 100
	strvar ret
	home_enter ret
	if ret != 0 
		add_sys_msg_start
		add_sys_msg ��û�й����԰����ʱ�޷�����
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	; OPEN_UI 96
endproc

proc 200
	home_ui 0
endproc

proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�����⹺��һ�׼�԰���㲻��Ҫ��Բп����ɱ��
	add_talk #b#b#c005ebe���ѿ���200�׼�԰��ֻҪ�ṩ����������һ����װ�޷Ѽ���ӵ��һ�׼�԰��#n
	add_talk #b#bÿ��ֻ��#c186506�޹�һ��#n���ȵ��ȵã�
	add_talk #b#b#cF80B0B�����10��δ�����԰��ϵͳ���Զ����ռ�԰������װ�Ρ���԰��С�����ѳɳ��ĵȼ���#n
		option 301 ��԰סլ����
		option 302 ��԰�˵ؽ���
		option 303 ��԰��԰����
		option 1 ����
	
	talk
endproc

proc 301
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bסլ�ڿɰڷŸ������δ��ȼҾߣ�ʹ�տ��ļ�԰��ø�����ܰ��

		option 300 ����
	
	talk
endproc

proc 302
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�˵ؿ�����ֲ�߹�����׬ȡ�����������ջ��԰�ȳ�������������ʣ�������ȡ��ϡ�еĽ𶹶���
	add_talk #b#b�𶹶�������ʱ���-��԰����Ա����ķ�����һ�ʱװ�������Ʒ
		option 300 ����
	
	talk
endproc

proc 303
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��԰������������С�����С����ι����10���󣨲�����10���������᲻��ʱ�Ĳ������ﵰ��
		option 300 ����
	
	talk
endproc

proc 400
	OPEN_NPC_SHOP
endproc

