
;̽��

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��~,��ȥ����������,500һ��,�����,����ʱ��Ŷ!

	
		option 150 #̽��
		
	talk
endproc 

proc 150
	
	strvar mid Moy
	DEC_MONEY_TEST 0 500 20 Moy
	if Moy = -1
		ADD_SYS_MSG_START
		ADD_SYS_MSG �������е����Ҳ���
		SET_SYS_MSG_MODE 4 0 0
		MSG_INFO
	else
		choose 2 151 152 ǰ������̽��,��Ҫ֧��500����,�Ƿ�ȷ��?
	endif
endproc
proc 151
	DEC_MONEY 0 500 20 10	
	if 10 = 0
		
		GET_PLAYER_ID 1 30
		TRANSMIT_CAMERA @30 135 1000 30200 18300 0 0
	endif
endproc 
proc 152
	return
endproc 
