

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk  #b#bӦ���и��õķ�ʽ��ʼ�µ�һ�죬������ǧƪһ�ɵ���ÿ�����綼������Ӧ���и��õķ�ʽ��ʼ�µ����Ҳ����Ӧ�úúÿ��ǵ�ʱ�����ȥ�ڸ�С̯����


	TASK_STAT 12058 1 20
	if 1 = 0 and 20 = 2
	  	option 150 (����)ѧԺ���������˽�һ�����
	endif

	TASK_STAT 12123 1 20
	if 1 = 0 and 20 = 2
	  	option 151 (����)������Ҫ������
	endif
	TASK_STAT 12124 1 20
	if 1 = 0 and 20 = 2
	  	option 152 (����)������Ҫ������
	endif
	TASK_STAT 12125 1 20
	if 1 = 0 and 20 = 2
	  	option 153 (����)������Ҫ������
	endif
	TASK_STAT 12126 1 20
	if 1 = 0 and 20 = 2
	  	option 154 (����)������Ҫ������
	endif

	TASK_STAT 12258 1 20
	if 1 = 0 and 20 = 2
	  	option 160 (����)����������Ҫ��һЩ���ϣ���Ը���æ�ռ�һЩô��
	endif

	TASK_STAT 12323 1 20
	if 1 = 0 and 20 = 2
	  	option 161 (����)��Щ�����͸��㣬��һ���ܲ����һ�Щʲô��
	endif
	TASK_STAT 12324 1 20
	if 1 = 0 and 20 = 2
	  	option 162 (����)��Щ�����͸��㣬��һ���ܲ����һ�Щʲô��
	endif
	TASK_STAT 12325 1 20
	if 1 = 0 and 20 = 2
	  	option 163 (����)��Щ�����͸��㣬��һ���ܲ����һ�Щʲô��
	endif
	TASK_STAT 12326 1 20
	if 1 = 0 and 20 = 2
	  	option 164 (����)��Щ�����͸��㣬��һ���ܲ����һ�Щʲô��
	endif
	OPTION 110 #(����)���̵�
	talk
endproc 
PROC 110
	OPEN_NPC_SHOP
ENDPROC
PROC 150
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b�����Ѿ����ٵ���Ƭ���أ��ֲ�������һ������һ��������

	OPTION 250 #(����)֪���ˣ�
	TALK
ENDPROC
PROC 250
	GET_ITEM_NUM_INPAK 5300509 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300509 1 0
	task_fini 12058 1
	endif
ENDPROC

PROC 151

    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b�ǳ���л���������Щ���ʣ����������ֿ��Լ��������ȥ�ˡ�

	OPTION 251 #(����)���ÿ�����
	TALK
	
ENDPROC
PROC 251
	GET_ITEM_NUM_INPAK 1019906 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 1019906 1 0
	task_fini 12123 1
	endif
ENDPROC

PROC 152

    	DIALOG 1
   	ADD_TALK $_npc$:      
    	ADD_TALK  #b#b�ǳ���л���������Щ���ʣ����������ֿ��Լ��������ȥ�ˡ�

	OPTION 252 #(����)���ÿ�����
	TALK
	
ENDPROC
PROC 252
	GET_ITEM_NUM_INPAK 1109906 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 1109906 1 0
	task_fini 12124 1
	endif
ENDPROC


PROC 153

    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b�ǳ���л���������Щ���ʣ����������ֿ��Լ��������ȥ�ˡ�

	OPTION 253 #(����)���ÿ�����
	TALK
	
ENDPROC
PROC 253
	GET_ITEM_NUM_INPAK 1209906 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 1209906 1 0
	task_fini 12125 1
	endif
ENDPROC

PROC 154

    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b�ǳ���л���������Щ���ʣ����������ֿ��Լ��������ȥ�ˡ�

	OPTION 254 #(����)���ÿ�����
	TALK
	
ENDPROC
PROC 254
	GET_ITEM_NUM_INPAK 1509906 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 1509906 1 0
	task_fini 12126 1
	endif
ENDPROC

PROC 160
    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK  #b#b��Σ�յĴ���֮�����ߣ�һֱ������ȥע����ʲô���������������������ʦҪ��Ļ��������һῼ��һ�¡�

	OPTION 260 #(����)лл���ˣ�
	TALK
ENDPROC
PROC 260
	GET_ITEM_NUM_INPAK 5300519 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 5300519 1 0
	task_fini 12258 1
	endif
ENDPROC

PROC 161

    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK #b#bлл�����������ʣ�������Щ�����㹻�����о��ܾá�

	OPTION 261 #(����)лл���ˣ�
	TALK
	
ENDPROC

PROC 261
	GET_ITEM_NUM_INPAK 2019906 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 2019906 1 0
	task_fini 12323 1
	endif
ENDPROC

PROC 162

    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK #b#bлл�����������ʣ�������Щ�����㹻�����о��ܾá�

	OPTION 262 #(����)лл���ˣ�
	TALK
	
ENDPROC
PROC 262
	GET_ITEM_NUM_INPAK 2029906 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 2029906 1 0
	task_fini 12324 1
	endif
ENDPROC

PROC 163

    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK #b#bлл�����������ʣ�������Щ�����㹻�����о��ܾá�

	OPTION 263 #(����)лл���ˣ�
	TALK
	
ENDPROC
PROC 263 
	GET_ITEM_NUM_INPAK 2039906 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 2039906 1 0
	task_fini 12325 1
	endif
ENDPROC

PROC 164

    	DIALOG 1
   	ADD_TALK $_npc$:       
    	ADD_TALK #b#bлл�����������ʣ�������Щ�����㹻�����о��ܾá�

	OPTION 264 #(����)лл���ˣ�
	TALK
	
ENDPROC
PROC 264
	GET_ITEM_NUM_INPAK 2049906 0 11
	if 11 < 1
		add_sys_msg_start
		add_sys_msg ������û�������Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
	ENDIF
	if 11 >= 1
	dec_item_num_inpak 2049906 1 0
	task_fini 12326 1
	endif
ENDPROC
