;npc ϣ��  ��ȡǱ������

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ʲôʱ��ֻҪ���ɵ������ٴ��ܵ��ַ����ҽ���ϧһ�д��ۣ������װ��������������ʹ���Ѵ������ӣ�����Ѫ������ȥ�ػ�ÿһ���������ɵ���������
	;add_talk #b#b����ɨ�����ɷֱ�ڰ��������������������ǰ��������Ա������4�ˣ��ͻ��ٻ����߼������
	;add_talk #b#b��ɱ�߼�����ɻ��#186506Ӣ�ۼ����#n
	add_talk #b#b
	add_talk #b#b#cff0080ÿ�����ɽ���30��Ǳ�����񣬷����������Ĵ�����#n
	
		TASK_ADD_OPTION 4635 0 100
		
		TASKCYCLE_ADD_OPTION 147 1 110
	talk
endproc 

proc 100
	TASK_DO_OPTION 4635 0
endproc

PROC 110	
	TASKCYCLE_DO_OPTION 147 1
ENDPROC
