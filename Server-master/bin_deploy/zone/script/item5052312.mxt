proc 1

strvar lv
;	GET_PLAYER_BASE_ATT 1 1 lv 1
;	RONGLU_CALL_MON 1 252001 3600 @lv 1

	CHOOSE 1 100 110 ȷ��������¯��#r        #c86FF00(��ʾ�������¯�ѷ��ã��ٴη��û��������)#n
endproc 

proc 100	
	strvar lv num
	GET_ITEM_NUM_INPAK 5052312 1 num
	if num = 0
		add_sys_msg_start
		add_sys_msg ������û�пɷ��õ���¯��
		set_sys_msg_mode 3 0 0
		msg_info
	else
		GET_PLAYER_BASE_ATT 1 1 lv 1
		RONGLU_CALL_MON 1 252001 3600 @lv 1
	endif
endproc 
proc 110
	return

endproc 