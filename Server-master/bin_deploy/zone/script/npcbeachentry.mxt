;NPC �ű� - ���͵���̲��ͼ

;NPC �ű�Ĭ��ִ���¼�
proc 1		
	dialog 1
	add_talk $_npc$:
	;2 ��ȡ�û��ȼ�
	GET_PLAYER_BASE_ATT 0 1 2 3
	if @2 >= 50
		add_talk #b#b �ˣ�#Gender2#�������߰ɣ����ھͳ�������ʼһ���Ҹ�֮�ðɡ�
		add_talk #b#b#c186506ÿ��ɹ1Сʱ�չ�ԡ��������Ӵ#n~#010~#002
		option 100	ɹ�չ�ԡע������
		option 300 #��Ҫ�������⺣̲
		option 500 #���������͵�
	else
		add_talk #b#b �ˣ�С#Gender2#����Ҳ��ȥ���⺣̲��Ŷ��NONONO~�����ڻ�̫С�ˣ����㵽50����ʱ�������ɣ�#006
	endif
	
	
	task_stat 4461 1 20
	if 20 = 2
		option 600 #���ҳϵ����˷�˿ϣ��ӵ��������������
	endif
	
	talk
endproc

;��ȡ����ɹ��������¼�
proc 2

endproc

;NPC �����񴥷��¼�
proc 3

endproc

proc 100
	dialog 1
	add_talk $_npc$:
	add_talk ɹ�չ�ԡע������:
	add_talk #b#b 1.ֻҪ�����⺣̲�����ڣ��ͻ��þ��飻
	add_talk #b#b 2.ÿ��ÿ����60���ӵĶ౶�չ�ԡ���飬ֻ���ڡ�ɳ̲�������ſ����ܣ�
	add_talk #b#b 3.���õľ������ĵȼ��йأ��ȼ�Խ�߻�õľ���Խ�ࣻ
	add_talk #b#b 4.���ȼ����⣬�ض���ʱװ��װ���͵��߶���Ӱ�쾭��ӳɣ�
	add_talk #b#b 5.С���з��
	option 1 ����
	talk
endproc

proc 300
	transmit 4403 29843 9300 1
	add_sys_msg_start
	add_sys_msg ��ӭ�������⺣̲�������ľ������������Ŀ�������״̬��
	set_sys_msg_mode 6 0 0
	msg_info
endproc 

proc 500
	return
endproc
proc 600

	add_item_num_inpak 5301006 1 1 0

endproc 
