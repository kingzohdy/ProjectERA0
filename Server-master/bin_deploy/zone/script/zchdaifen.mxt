	
proc 1
	
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���ã��װ�����ң����ｫ�ṩ������Ҷһ��Ļ������������Ķһ���Ҫ�ļ���ң��������ǻ����Щ�������Ϊ�ر�Ŷ��
	
	strvar ItmNum32 ItmNum33 ItmNum34

	var ItmNum32 0
	var ItmNum33 0
	var ItmNum34 0
	GET_ITEM_NUM_INPAK 5060017 1 ItmNum32
	GET_ITEM_NUM_INPAK 5060018 1 ItmNum33
	GET_ITEM_NUM_INPAK 5060019 1 ItmNum34
	
;	add_talk ���㡱��#c186506��$@ItmNum32$��#n��
;	add_talk ���͡���#c186506��$@ItmNum33$��#n��
;	add_talk ��Ԫ����#c186506��$@ItmNum34$��#n��	
;	
;	 option 200 ʹ���������㡱�һ��������͡�
;	 option 201 ʹ���������㡱�һ�������Ԫ��
;	 option 202 ʹ���������͡��һ��������㡱
;	 option 203 ʹ���������͡��һ�������Ԫ��
;	 option 204 ʹ��������Ԫ���һ��������㡱
;	 option 205 ʹ��������Ԫ���һ��������͡�
	talk
endproc 
proc 300
	return
endproc

proc 200
	CHOOSE 1 250 300 ȷ��ʹ���������㡱�һ��������͡���
endproc 
proc 250
	strvar ItmNum3 Num ItmNum4 Reti

	GET_ITEM_NUM_INPAK 5060017 1 ItmNum3
	GET_ITEM_NUM_INPAK 5060018 1 ItmNum4
	if ItmNum3 > 1 
		GET_PACK_SPACE 0 Num 		
;		ADD_ITEM_NUM_INPAK 5060018 2 2 Reti
;		if Reti = -2
		if ItmNum4 = 0 and Num = 0 
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 1 0 0
			msg_info
			return
		endif
		
		DEC_ITEM_NUM_INPAK 5060017 2 1
		ADD_ITEM_NUM_INPAK 5060018 2 0 0
		
		add_sys_msg_start
		add_sys_msg ��ϲ�㡱ͨ���������㡱�һ�����������͡�!
		set_sys_msg_mode 3 0 0
		msg_info
	
	else
		add_sys_msg_start
		add_sys_msg �ǳ���Ǹ�������еġ��㡱�����Զһ���
		set_sys_msg_mode 1 0 0
		msg_info
	endif

endproc
