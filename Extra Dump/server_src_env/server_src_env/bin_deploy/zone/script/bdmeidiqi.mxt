


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b������Ҳ��һ�����룬�뿪�������ĵط��������ﻨ����Ѱ��һ�������磬���������е��ζ����ˣ������ˣ�ֻʣ������ۿڣ�����������ⴣ�ţ���˵�����뿪���Ҳ����뿪��Ϊʲô��
	add_talk #b#b��Ϊ�ҵĸ��ף��Ҹ��׵ĸ��ף����Ƕ�����������Ǵ�С�ͽ̵����˲��������������׼ҡ����ҵĸ���Ϊ���Ҷ�ɥ�����ҵ�����Ϊ���Ҷ�����Ʊ���Ҳ��ܾ�����һ����֮��
		task_add_option 1368 1 100
		task_add_option 1369 1 101
;		task_add_option 1370 1 102
;		task_add_option 1371 1 103
		task_add_option 1372 1 104
		task_add_option 1373 1 105
		task_add_option 1374 1 106
;		task_add_option 1375 1 107
		task_add_option 1378 1 108

		strvar Ret
		task_stat 1372 1 Ret
		if Ret = 2
			option 150 ˵��������

		endif
	
	talk
	
endproc 

PROC 100
	task_do_option 1368 1
ENDPROC
PROC 101
	task_do_option 1369 1
ENDPROC
;PROC 102
;	task_do_option 1370 1
;ENDPROC
;PROC 103
;	task_do_option 1371 1
;ENDPROC
PROC 104
	task_do_option 1372 1
ENDPROC
PROC 105
	task_do_option 1373 1
ENDPROC
PROC 106
	task_do_option 1374 1
ENDPROC
;PROC 107
;	task_do_option 1375 1
;ENDPROC
PROC 108
	task_do_option 1378 1
ENDPROC

proc 2

	get_trig_task 1 2
	if 2 = 1378
		task_fini 1378 1
	endif
	if 2 = 1372
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b������Ҳ��һ�����룬�뿪�������ĵط��������ﻨ����Ѱ��һ�������磬���������е��ζ����ˣ������ˣ�ֻʣ������ۿڣ�����������ⴣ�ţ���˵�����뿪���Ҳ����뿪��Ϊʲô��
		add_talk #b#b��Ϊ�ҵĸ��ף��Ҹ��׵ĸ��ף����Ƕ�����������Ǵ�С�ͽ̵����˲��������������׼ҡ����ҵĸ���Ϊ���Ҷ�ɥ�����ҵ�����Ϊ���Ҷ�����Ʊ���Ҳ��ܾ�����һ����֮��
			option 150 ˵��������
		talk
	endif
endproc 
proc 190
	return
endproc 
proc 150
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��л���ĺ��⣬���Լ������ã����Լ����ȣ�����Ҫ������ġ�

		option 151 ������ô����ȥ��
		option 200 �����ñ����߶���ˣ�

	talk
endproc 
	
proc 151
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�Ҳ���ð�գ��Ҳ����ٳ���ʧ�ܣ�����ٴ�ʧ�ܣ������þͿ���������Σ�գ�

		option 152 �ٴ�ʧ�ܣ���֮ǰ����ʧ�ܹ���
		option 160 Ҳ�������ھ��Ѿ�������Σ���ˣ�

	talk
endproc 
proc 152
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�ǣ����������˺ܶ���ȥ���ҵ����ã������ǰ�������ת�Ƶ���һ�����صĵط��������޷��ҵ�����

		option 153 ���������Ѿ�ɱ������
		option 170 ���Ǿ���ΪʲôҪץ����ɴ��

	talk
endproc 
proc 153
	dialog 1
	add_talk $_npc$:
	add_talk #b#b����ģ�����ɴû������
		option 154 ����ô֪����û����
	talk
endproc 
proc 154
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���������ã���������ˣ��Ҳ����Ӧ���������������˷��ҵ�ʱ���ˣ������ֻ�����Ҹ���˵���Ļ����Ի������ۣ�������޵�һ��������������̸��
		option 190 #������
	talk
endproc 
	
proc 160
	dialog 1
	add_talk $_npc$:
	add_talk #b#b����ģ���ȷ������ɴ�����ţ����ܸо����������ں�Х���壬��������ߣ������Һܿ�ͻ�ȳ�������л���İ�æ���ټ���
		option 190 #�ټ�
	talk
endproc 
proc 170
	dialog 1
	add_talk $_npc$:
	add_talk #b#b����������Ҫʲô���Ҷ�����ȥ��������ɴ��������������û�����������š�
		option 171 ��ȥ�ҹ�����
	talk
	
endproc
proc 171
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��Ȼ����Х������ô��Ҫ��һ���˿�ν�Ǵ����룬̸�����ף�
		option 172 ������Ҫ��ô���أ�
	talk

endproc
proc 172
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�ȡ�
		option 190 #����
	talk
endproc
	
	
proc 200
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�����ǵڶ�ʮ���졣

		option 201 ����ô����һ��Ҳ�����Ű���
	talk
endproc
proc 201
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���ţ������ܰ������þȻ����𣿽��������ҵĸ��������𣿽���������ӵ�����ϵ�������
		option 202 ���ԣ���ͳ������Ĺ��ả����
	talk
endproc
proc 202
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��û�У����˵�˵�����û�й�������.��������û�У�
		option 203 ����˵�㹴������.������
	talk
endproc
	
proc 203
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�ҡ������������˶�֪������.�����Ǻ���ͷ�ӡ�
		option 204 �������Ǹ���ɫħ����������ɴ����
	talk
endproc
	
proc 204
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��ɴ������ô���ģ�����Ӧ���ң����ᶯ��ɴһ����ë��
		option 205 #��˵ʵ������
	talk
endproc
proc 205
	
	task_fini 1372 1
endproc
	
	
	