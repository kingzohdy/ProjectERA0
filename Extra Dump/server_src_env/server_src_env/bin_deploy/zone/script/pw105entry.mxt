;70����,���npc,����
proc 1
	strvar l
	GET_PLAYER_BASE_ATT 1 0 l 0
	if l < 68
		dialog 1
		ADD_TALK $_npc$:
		ADD_TALK #b#b�ף�������С#Race#�������Σ�գ��쵽����ȥ��������Ұ����¡�
		talk
		
	else
		dialog 1
		ADD_TALK $_npc$:
		ADD_TALK #b#b���������ˣ�ޱޱ���Ѿ�����������غܾ��ˣ������ڻ�û�г�����������������ʲô�£���Ȼ��һ�ٱ�֤�ܰ�ȫ��������
		ADD_TALK #b#b�ݿɿ���Ϣ��ά��������ڽ���һ�����ܵ�ʵ�飬���ʵ��ɹ����������ֵض�����������Ͻ�ȥ������ϰɣ���˵���������������������Լ�ע��ͻᷢ�֡�
		option 100 #���븱��
		talk
		
	endif
	
endproc
proc 100
;	enter_pworld 105 0 1
	
	OPEN_PWORLD_UI 3
endproc
