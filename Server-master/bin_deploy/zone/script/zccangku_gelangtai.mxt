 

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ӭ���١��������С������Ǹ���ֿ����Ļ�����$_npc$���ṩ����Ʒ��Ǯ����صĴ洢����
	add_talk #b#b#c186506Ϊ�������˺ŲƲ���ȫ�������ñ�������ݼ� B ����ȫ����#n


	OPTION 104 #�򿪲ֿ�
;	option 200 #�����ҽ�����
	option 130 #�޸İ�ȫ������
	option 131 #ȡ����ȫ������
	talk

endproc 

PROC 104
	OPEN_UI 0
ENDPROC
proc 130
	OPEN_UI 55
endproc
proc 131
	open_ui 56
endproc
