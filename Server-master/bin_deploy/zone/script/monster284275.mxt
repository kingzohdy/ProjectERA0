

proc 1

;	add_sys_msg_start
;	add_sys_msg ѹ���Ե�����������ʹ�������������������ǰ��#L�ռ䴫����@@(60100,16700,19000)#n���ˡ������ߡ�����ر����ߵı��䣡
;	set_sys_msg_mode 6 0 0
;	map_msg_info
	strvar mid
	GET_MON_INFO 0 0 mid 0 0 0 0 0
	TARGET_ADD_STATUS 3 @mid 1095 1 25920000 0
endproc  

proc 2
	strvar x y 
	GET_MON_INFO 0 0 0 0 0 0 x y
	NEW_NPC 1 307022 @x @y 0 ruqin_npc.mac
	
	strvar ClanID
	GET_ID_PLAYER_INFO 0 18 ClanID
	add_sys_msg_start
	add_sys_msg ����ԡѪ��ս���ɹ��ѡ������ߡ����ˣ�����˱��䣡
	set_sys_msg_mode 14 0 0
	CLAN_MSG_INFO 1 @ClanID
	
	SET_CLAN_EVERYPWORLD_BOSS_DIE 1
endproc 