proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#bõ������һ������Ĺ�ִ�������Σ���ϣ����˵������ȴû��һ�γɹ�����Ȼ��ˣ������Ǿʹ�ɱ��ɣ�ɱ�����������ߣ�
	add_talk #b#b#c6d00d2�ڰ�����Ա��ӿ��л���ӿ�����������ǣ�
	add_talk #b#b#c6d00d2���������ͨ���϶½������ߡ�
	add_talk #b#b#cff0080����õ������Ҫ�����ܵ��κ��˺���
	
	OPTION 100 #ֻҪ������˭Ҳ����Խ�׳ذ벽��Ϊ��õ����
	
	task_add_option 3317 1 101
	
	talk
endproc 

PROC 101
	task_do_option 3317 1
ENDPROC