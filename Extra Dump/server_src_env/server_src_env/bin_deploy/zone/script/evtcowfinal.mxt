;��ţ����,�����¼�
proc 1
;	MAP_NEW_NPC #���ر�־ #NpcID #x #y #dir #map $�ű� 
;	MAP_NEW_NPC 1 ::taskstartnpcid:: 18332 33387 ::taskstartnpcdir:: ::mapid:: npccowfinalstart.mac 
;	��������
	COW_PWORLD_CREATE 1
	if 1 = 0
		COW_FINAL_BEGIN 1
		add_sys_msg_start
		add_sys_msg ţ�к�æ�������ڿ�ʼ����Я���ǹ����볡ȯǰ���ʺ絺��ͨ���ƶ������ǹ������������������ǰʮ��ӵ�зǳ����Ľ���Ӵ��  
		set_sys_msg_mode 1 0 0
		svr_msg_info
	endif
endproc
proc 2
;	get_map_inst ::mapid:: 1
;	if 1 = 0
;		del_npc ::taskstartnpcid:: 1
;	endif
;	��������
	COW_FINAL_END 1
	add_sys_msg_start
	add_sys_msg ţ�к�æ�������˽��������λ����ѡ��ǰ���ʺ絺�佱�α�-���д���ѯ���Σ���ȡ������  
	set_sys_msg_mode 1 0 0
	svr_msg_info
endproc
