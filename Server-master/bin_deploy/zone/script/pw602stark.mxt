;�����ػ���Ŧ�    ,����buff
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ã��ҽ������¼�������ʵ������֧��������Ӧ��û�˱�����Ҫ���������ˡ�
	option 100 #�򿪲ֿ�
	option 200 #�򿪹�����Դ����
	talk
endproc
proc 100
	 OPEN_UI 0
endproc
proc 200
	OPEN_UI 47
endproc
