proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������������ڼ��������޷�������ɵ����飬��ס���Ƕ�һ�޶��ġ�
	add_talk #b#b#c186506����װ���ϵ���Щ��#cff4e00��������#c186506���������ܽ���#cff4e00��������#c186506���滻������һ��װ����#n
	add_talk #b#b#c005ebe����ı�Ŀ��װ���ϵ������κ�����Ŷ��

		option 110 #��������������ת�ơ�
		option 120 #���߼���������ת�ơ�
		option 130 #��װ���������Ը��졿
	talk

endproc 

proc 110
	open_ui 88
endproc

proc 120
	open_ui 89
endproc

proc 130
	open_ui 95
endproc
