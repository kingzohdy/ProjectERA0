;�侲

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���ʣ��㵽�������ʲô�أ�����������ң��ҿ��Ժܸ����εĸ����㣬��ֻϲ������Ʈ������Ī�ⷨ�������ħ��ʦ��

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
	del_npc 352004 0
	call_mon 1 237004 0 0 6806 5935 0
	get_pworld_var 12 32 1 31
	var 31 237004
	set_pworld_var 12 32 @31 1
	GET_MAP_MON_ID 237004 54 0 
	mon_speech @54 0 1 0 �ȴ������˵��
	
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
