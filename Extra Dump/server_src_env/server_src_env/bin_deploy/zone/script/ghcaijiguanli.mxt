

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bһ���˾���������˱���ʱ��ǣ��ܶ����鲻�ǿ���Ѫ�Ϳ��Եģ������ˡ����Ž�������빫��ɣ��ŶӺ�������κ�ǿ���װ���������塣
	
		option 120 #�ص��������
		option 121 #�ύ��Դ(��)
		option 122 #�ύ��Դ(����)

	talk
endproc 
proc 120
;	enter_clan_city	1
	GOTO_CLAN_CITY 0 13000 15000
endproc 
proc 121

	LAYDOWN_CLAN_ORE 0 0
endproc 
proc 122
	COMMIT_CLAN_ITEM 1 0 0

endproc
