;��Ƨ

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b������������ȥ��ȥ���벻Ҫ�����ҡ������Ҳϲ�������������ǿ��Ծ������ദ���������

	option 100 ��Ҫ��ս

	talk
endproc

proc 100
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��Ȼ��ܶ���θ�ڣ����ǣ�������������һ��ֻ������ǿ�ߡ��������չ�ֳ��㹻�����ŷ�������������ȻҲ�����һ��һ���ҳϡ�
	add_talk #b#b������㲻��սʤ�ң���ô���ܱ�Ǹ����û�취����һ�����Լ��������ˡ�
	add_talk #b#b$_player$����׼���ý�����ս����
	
	option 110 #��ʼ��ս
	option 111 #������ս

	talk

endproc 

proc 110
	del_npc 352006 0
	call_mon 1 237006 0 0 6806 5935 0
	get_pworld_var 12 32 1 31
	var 31 237006
	set_pworld_var 12 32 @31 1
	GET_MAP_MON_ID 237006 56 0 
	mon_speech @56 0 1 0 �ȴ������˵��
	
	add_sys_msg_start
	add_sys_msg ��ܡ�����ǹ�֡���
	set_sys_msg_mode 3 0 0
	msg_info
			
	strvar Num
	GET_ITEM_NUM_INPAK 5233705 1 Num
	if Num = 0
		ADD_ITEM_NUM_INPAK 5233705 1 1 0	
	endif
	return

endproc

proc 111
	return
endproc
