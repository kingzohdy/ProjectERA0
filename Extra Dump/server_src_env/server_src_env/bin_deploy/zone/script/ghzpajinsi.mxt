

proc 1


	DIALOG 1
	strvar Ret
	ENTER_WORLD_CITY_CHECK Ret
	if Ret = -1
		add_talk $_npc$:
		add_talk #b#b�ɾͰ�����ʱ�̼�����������������ֵܽ�������սɳ���ɡ���ʱ��ǵ��п�����ռ�죬���Ǿͽ���������󶼳ǵİ�����
		talk
		return
	endif

		add_talk $_npc$:
		add_talk #b#b�ɾͰ�����ʱ�̼�����������������ֵܽ�������սɳ���ɡ���ʱ��ǵ��п�����ռ�죬���Ǿͽ���������󶼳ǵİ�����

			option 200 ս������
			option 201 ռ�콱��
			option 202 #�����п����ĵ�ͼ
			option 203 #�ر�
				
	talk
endproc 
proc 200
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#bս������
	add_talk ս��ʱ�䣺
	add_talk #b#b��������ս����ÿ������20��00��ʼ��21��30�ֽ�����
	add_talk ս������
	add_talk #b#b1.ֻ�оݵ�ռ�칫������п����ĵ�ͼ�����Ὠ������Ȩ��
	add_talk #b#b2.��ս����ͼ�У�����ռ�졢�ݻٻ�ɹ��ػ��Ƿ��������ɻ��ս�����֡�
	add_talk ʤ��������
	add_talk #b#b��ս������ʱ�����ݸ�����ս�������ж�ʤ��������������1���ᣬ�������ʤ�������������
	add_talk #b#b 1.����ط��������������������ͬ������ط�ʤ����
	add_talk #b#b 2.�����������֮�����ƽ�֣����շ����ﵽʱ�䣬��ǰ�߻�ʤ��
	add_talk #b#b��Ҫ��ʾ�����п������У�����3���ն˿���������Ȩ�ɻ�ô���ս�����֣�
	
	option 1 ����
	
	talk
endproc
proc 201
	DIALOG 1
	add_talk #b#b#b#b#bռ��ʱ��ǽ���
	add_talk 1.��ǰռ�칫�ᣬÿ��᳤����ȡһ��ռ���ʽ��������ң���
	add_talk 2.ռ���������ʱϵͳ�Զ���������һ��������Դ��
	add_talk 3.���е�ǰ�����Ա����ʱ���NPC��������Ʒ��ά��װ����������һ���ۿ��Żݡ�
	add_talk 4.���е�ǰ�����Ա����[������������]���纽�ߴ����У�ӵ�и���ĵ�ͼ���͵�
	add_talk 5.ʱ��ǳ�����ռ�칫��᳤�����˵���
	add_talk 6.��ǰռ�칫��Ĺ��������⻯��
	add_talk 7.ע������Ϊ��Ҫ��Ȩ�뽱����������ռ��ʱ��ǵ�ʱ��Խ�ã�ӵ�е���Ȩ�뽱��Խ�ࣩ��

	option 1 ����
	
	talk
endproc
proc 203
	return
endproc



proc 202

	strvar Player
	IS_WORLD_CITY_OWNER Player
	;������������ʱ�����Խ���
	if Player = 1
		call 204
		return
	endif
	
	
	strvar Ret
	GET_EVENT_STAT Ret 45
	;�ǳ��������߿�սһСʱǰ���ɽ���
	if Ret = 1
		add_sys_msg_start
		add_sys_msg ս��������ʼ����ǰʱ����ڽ�ֹ���ڣ�ս����ʼ��ſɽ���
		set_sys_msg_mode 4 0 0
		msg_info
		return		
	else
		call 204
	endif

endproc 

proc 204
	strvar Rad
	rand Rad 21
	switch Rad
		case 0
			TRANSMIT 4404 21616	7372 0
		endcase
		case 1
			TRANSMIT 4404 21544	7135 0
		endcase
		case 2
			TRANSMIT 4404 21234	6717 0
		endcase
		case 3
			TRANSMIT 4404 21063	6398 0
		endcase
		case 4
			TRANSMIT 4404 21179	6227 0
		endcase
		case 5
			TRANSMIT 4404 21465	5915 0
		endcase
		case 6
			TRANSMIT 4404 22142	5839 0
		endcase
		case 7
			TRANSMIT 4404 22484	6122 0
		endcase
		case 8
			TRANSMIT 4404 22514	6967 0
		endcase
		case 9
			TRANSMIT 4404 22527	6582 0
		endcase
		case 10
			TRANSMIT 4404 21938	6549 0
		endcase
		case 11
			TRANSMIT 4404 21629	6316 0
		endcase
		case 12
			TRANSMIT 4404 21777	6234 0
		endcase
		case 13
			TRANSMIT 4404 22139	7076 0
		endcase
		case 14
			TRANSMIT 4404 22103	7352 0
		endcase
		case 15
			TRANSMIT 4404 21678	6757 0
		endcase
		case 16
			TRANSMIT 4404 21149	6983 0
		endcase
		case 17
			TRANSMIT 4404 20929	6701 0
		endcase
		case 18
			TRANSMIT 4404 21797	5800 0
		endcase
		case 19
			TRANSMIT 4404 22774	6447 0
		endcase
		case 20
			TRANSMIT 4404 22234	6257 0
		endcase
	endswitch
endproc 










