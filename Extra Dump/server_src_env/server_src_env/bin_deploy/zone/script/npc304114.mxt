
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#bHEY��$_player$�������Ļ������ˣ���Ҫ��ø�ǿ�𣿻��ڲ�ͣ��Ūװ������������ֻҪ��μӻ����Ϳ��Ա�÷ǳ�ǿ�󣬲�����ǿ��Ҳ����������Σ��Ŷ�����зḻ�Ľ����أ�
	add_talk #b#b#c186506��Ӱ֮ս����ʱ�䣺
	add_talk #b#b#c0181f912:00-12:10��16:30-16:40
	
	OPTION 202 ��Ӱ֮ս

	talk
	
endproc


proc 202

	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������кöྭ��Ľ���ѽ�����Ҽ򵥴ֱ����Ͻ������μӰɣ�

	OPTION 300 #�μӹ�Ӱ֮ս
	talk
endproc
proc 300 
		GET_EVENT_STAT 401 80		
	
		IF 401 = 1
			enter_pworld 221 0 12
		ELSE
			add_sys_msg_start
			add_sys_msg ���û��ʼ���޷����룡
			set_sys_msg_mode 4 0 0
			msg_info
		ENDIF
endproc