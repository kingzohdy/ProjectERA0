
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ʲô���԰�����𡣡�������

	
	
;	OPTION 110 #(����)���̵� 

	TASK_STAT 12012 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (����)���������ˣ�ѧԺ��ϣ���õ���İ���
	endif

	TASK_STAT 12212 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (����)���Ǵ�����ĵ�ͼ
	endif


	talk
endproc 

proc 110
    OPEN_NPC_SHOP
endproc


PROC 150	
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b����ȥ������Ƕ�һ��Ŭ���Ļ���Ӧ�ÿ����ҵ������˵���Ϣ���ðɣ�����ѧԺ������֪���ˡ�

	OPTION 250 #(����)�õģ�
	TALK
ENDPROC
proc 250
	GET_ITEM_NUM_INPAK 5300501 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300501 1 0
    	task_fini 12012 1
	endif
endproc

PROC 160
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b��·����չ��������ԵĻ�����ϣ��ʱ��ǵĸ�λ���ܹ����ܵ��������ĺô���

	OPTION 260 #(����)лл���֧�֣�
	TALK
ENDPROC
proc 260
	GET_ITEM_NUM_INPAK 5300511 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300511 1 0
    	task_fini 12212 1
	ENDIF

endproc
