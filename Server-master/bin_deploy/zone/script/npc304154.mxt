;�����

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���ٸ���а����������в��ս�����ɳ����ȱ����ı��ս������
	add_talk #b#bΪ�˷���ս��100%�����ܣ��������˳�ľ��ʿ�չʵսģ�⣬#c186506������ר���ĶԿ����أ�Ͷ���˼�ʮ������ս��#n��
	add_talk #b#b������㽫���鵽ɳ����ս�����ڻ��ķ��Կ���
	
	option 100 #��ը��ս
	
	talk

endproc

proc 100
	dialog 1
	add_talk $_npc$:
	add_talk #c186506�ʱ�䣺
	add_talk #b#bÿ��12:10-12:20��16:40-16:50#n
	add_talk #b#b
	add_talk #c005ebe��淨��
	add_talk #b#b�����ܻ��ٵз���Ӫ�����ս��#n
	add_talk #b#b
	add_talk #c6d00d2�������
	add_talk #b#b1. ֻҪ���뼴�ɻ�ô������飡
	add_talk #b#b1. ֻҪ���뼴�͡������������
	add_talk #b#b2. ����ս��������ǰ60%�ɻ�ó�ֵ���ң�#n
	
	option 200 #�μӡ���ը��ս��
	
	talk

endproc 

proc 200 
		GET_EVENT_STAT 10 107		
	
		IF 10 = 1
			enter_pworld 302 0 0
		ELSE
			add_sys_msg_start
			add_sys_msg ά��Ա���ڼ���ս���У���ʱ�޷��μӡ���ը��ս�������Ժ�����
			set_sys_msg_mode 4 0 0
			msg_info
		ENDIF
endproc
