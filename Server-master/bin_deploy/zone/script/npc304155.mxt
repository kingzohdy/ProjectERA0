;��ķ��

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#bս��ʵսģ�����ڽ����У������ھ���Ҫ����ʱ�����
	add_talk #b#b
	add_talk #b#b#c186506����ʱ��ɺ��޷��ٲμӸôΡ���ը��ս��#n
	
	option 100 #����ʱ���
	
	talk

endproc

proc 100
	choose 0 101 302 ���������뿪���޷��ٲμӸôΡ���ը��ս�����������Ƿ�ȷ�Ϸ���ʱ��ǣ�
endproc

proc 101
	transmit 1000 36319 34445 1
endproc
