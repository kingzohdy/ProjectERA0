proc 1
	dialog 1
	add_talk $_npc$:
	; add_talk #b#b�װ���������ã�����$_npc$��
	add_talk #b#bΪ�˴�л��Ҷԡ����Ԫ���ĺ񰮺�֧�֣������Դ�#c005ebe������Ȩ��#n���ռ�3��#c005ebe�ƽ���ȯ#n���Ҷһ�#c005ebe�ƽ����#n�����ҰĦ������Ͽ��ж��ɣ�
	; add_talk #b#b
	add_talk #b#b��Ȼ��Ҳ���Դ�#c005ebe������Ȩ��#n���ռ�4��#c005ebe�׽�#n���Ҷһ�#c005ebe�׽𸣴�#n���߼������ȯ����ʯ�ȸ������ⱦ�
	add_talk #b#b
	add_talk #b#b#c186506����ȯ��������ʱ�ʯ������
	add_talk #b#b����ȯ�������׽𸣴��л�����
	add_talk #b#bʱװȯ��������ʱ�ʯ�����׽𸣴����#n
		
		option 400 #(����)���̵�
		
;		option 500 #��Ҫ�һ��ƽ����
;		option 520 ��Ҫ�����˹ι��ָ���
		
;		option 600 #��Ҫ�һ��׽𸣴�
	talk
endproc

proc 500
	strvar it1
	GET_ITEM_NUM_INPAK 5054006 1 it1
	
	if it1 >= 3
		strvar num
		GET_PACK_SPACE 0 num 
		if num > 0
			DEC_ITEM_NUM_INPAK 5054006 3 15
			if 15 = 0
				ADD_BIND_ITEM_NUM_INPAK 5052910 1 1 0                   
			endif 
		else                  
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 4 0 0
			msg_info
		
		endif
	else
		add_sys_msg_start
		add_sys_msg ��Я���Ļƽ���ȯ�������㣡
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc


proc 520
	dialog 1
	add_talk $_npc$:
	add_talk #b#b������ͨ�����Źι��������Ҷһ��ι������
	add_talk #b#b
;	add_talk #b#b��һ�֣�����+17173+����+��Ѷ
;	add_talk #b#b�ڶ��֣�����+17173+����+����
;	add_talk #b#b�����֣�����+17173+����+����
;	add_talk #b#b�����֣�����+��Ѷ+����+����
		
		option 501 #���õ�һ����ϻ�����
		option 502 #���õڶ�����ϻ�����
		option 503 #���õ�������ϻ�����
		option 504 #���õ�������ϻ�����
		
	talk
endproc

proc 501
	strvar it1 it2 it3 it4 it5 it6
	GET_ITEM_NUM_INPAK 5054000 1 it1
	GET_ITEM_NUM_INPAK 5054003 1 it4
	GET_ITEM_NUM_INPAK 5054004 1 it5
	GET_ITEM_NUM_INPAK 5054005 1 it6
	
	if it1 > 0 and it4 > 0 and it5 > 0 and it6 > 0
		strvar num
		GET_PACK_SPACE 0 num 
		if num > 0
			DEC_ITEM_NUM_INPAK 5054000 1 1
			DEC_ITEM_NUM_INPAK 5054003 1 1
			DEC_ITEM_NUM_INPAK 5054004 1 1
			DEC_ITEM_NUM_INPAK 5054005 1 1
			                  
			ADD_BIND_ITEM_NUM_INPAK 5052911 1 1 0  
		else                  
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 4 0 0
			msg_info
		
		endif
		
	else
		add_sys_msg_start
		add_sys_msg ��Я������Ʒ�������㣡
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc 

proc 502
	strvar it1 it2 it3 it4 it5 it6
	GET_ITEM_NUM_INPAK 5054000 1 it1
	GET_ITEM_NUM_INPAK 5054003 1 it4
	GET_ITEM_NUM_INPAK 5054004 1 it5
	GET_ITEM_NUM_INPAK 5054002 1 it6
	
	if it1 > 0 and it4 > 0 and it5 > 0 and it6 > 0
		strvar num
		GET_PACK_SPACE 0 num 
		if num > 0
			DEC_ITEM_NUM_INPAK 5054000 1 1
			DEC_ITEM_NUM_INPAK 5054003 1 1
			DEC_ITEM_NUM_INPAK 5054004 1 1
			DEC_ITEM_NUM_INPAK 5054002 1 1
			                  
			ADD_BIND_ITEM_NUM_INPAK 5052911 1 1 0  
		else                  
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 4 0 0
			msg_info
		
		endif
		
	else
		add_sys_msg_start
		add_sys_msg ��Я������Ʒ�������㣡
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc 

proc 503
	strvar it1 it2 it3 it4 it5 it6
	GET_ITEM_NUM_INPAK 5054000 1 it1
	GET_ITEM_NUM_INPAK 5054003 1 it4
	GET_ITEM_NUM_INPAK 5054004 1 it5
	GET_ITEM_NUM_INPAK 5054001 1 it6
	
	if it1 > 0 and it4 > 0 and it5 > 0 and it6 > 0
		strvar num
		GET_PACK_SPACE 0 num 
		if num > 0
			DEC_ITEM_NUM_INPAK 5054000 1 1
			DEC_ITEM_NUM_INPAK 5054003 1 1
			DEC_ITEM_NUM_INPAK 5054004 1 1
			DEC_ITEM_NUM_INPAK 5054001 1 1
			                  
			ADD_BIND_ITEM_NUM_INPAK 5052911 1 1 0  
		else                  
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 4 0 0
			msg_info
		
		endif
		
	else
		add_sys_msg_start
		add_sys_msg ��Я������Ʒ�������㣡
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc 



proc 504
	strvar it1 it2 it3 it4 it5 it6
	GET_ITEM_NUM_INPAK 5054000 1 it1
	GET_ITEM_NUM_INPAK 5054001 1 it4
	GET_ITEM_NUM_INPAK 5054002 1 it5
	GET_ITEM_NUM_INPAK 5054005 1 it6
	
	if it1 > 0 and it4 > 0 and it5 > 0 and it6 > 0
		strvar num
		GET_PACK_SPACE 0 num 
		if num > 0
			DEC_ITEM_NUM_INPAK 5054000 1 1
			DEC_ITEM_NUM_INPAK 5054001 1 1
			DEC_ITEM_NUM_INPAK 5054002 1 1
			DEC_ITEM_NUM_INPAK 5054005 1 1
			                  
			ADD_BIND_ITEM_NUM_INPAK 5052911 1 1 0  
		else                  
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 4 0 0
			msg_info
		
		endif
		
	else
		add_sys_msg_start
		add_sys_msg ��Я������Ʒ�������㣡
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc 

proc 600
	strvar it1
	GET_ITEM_NUM_INPAK 5052930 1 it1
	
	if it1 >= 4
		strvar num
		GET_PACK_SPACE 0 num 
		if num > 0
			DEC_ITEM_NUM_INPAK 5052930 4 15
			if 15 = 0
				ADD_BIND_ITEM_NUM_INPAK 5052912 1 1 0                   
			endif 
		else                  
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 4 0 0
			msg_info
		
		endif
	else
		add_sys_msg_start
		add_sys_msg ��Я���İ׽��������㣡
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc 

proc 400
	OPEN_NPC_SHOP
endproc





