proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Ҫ���£�$_player$�������ҵ���ʶ���磬�ҵ���걻�������߳����ص�ج��֮�У����������޷����ѡ���
    add_talk #b#b#c676464<���ɸǶ�ָ�ŵ������ŵ��Լ�>#n
    add_talk #b#b���Ǳ��յ����ʿ��������������ξ����밴���ҵ�ָ��������������ֻ��һ�λ��ᣬһ��ʧ�ܣ�ֻ�ܴ�ͷ������
	
	task_add_option 3411 1 100
		option 120 #�һ�û����׼�����´������ɣ�
	talk
	
endproc
proc 100

	task_do_option 3411 1

endproc 
proc 120

	TRANSMIT 4402 18500 8300 1

endproc

