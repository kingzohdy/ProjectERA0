;70��������

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bһ���˾���������˱���ʱ��ǣ��ܶ����鲻�ǿ���Ѫ�Ϳ��Եģ������ˡ����Ž�������빫��ɣ��ŶӺ�������κ�ǿ���װ���������塣

		option 120 #���븱��
		option 121 #���ø���

	talk
endproc 
proc 120

;	enter_pworld 105 0 1
;		OPEN_PWORLD_UI 105
		OPEN_PWORLD_UI 3
endproc 
proc 121

	RESET_PWORLD 105
		
endproc
