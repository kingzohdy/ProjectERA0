;;;;���޼�



proc 1
;	strvar Ret Map
;	get_player_mapinfo 0 Map 2 3 4
;	
;	task_stat 3003 1 Ret
;	if Ret = 2
;		if Map = 20200
		
;			del_mon 1
	
;			strvar Mid
;			GET_TARGET_INFO 3 1 Mid
;			GET_MON_HP_PERCENT @Mid Ret
;
;			if Ret > 50
;				EFFECT_ID 0 @Mid playMontion(%d,"10254_5_2")
;				
;				add_sys_msg_start
;				add_sys_msg �������ɲ�׽��������ǹ�֣�ʹ��Ѫ�����͵�800����
;				set_sys_msg_mode 3 0 0
;				msg_info
;				
;				GET_MAP_NPC_ID 317009 60 0 
;				mon_speech @60 0 1 0 ���ɲ�׽���Ĺ���Ѫ��Խ��ʱ����׽�ɹ�����Խ�ͣ�
;			else
;				del_mon 1
;			endif
;		else
;			add_sys_msg_start
;			add_sys_msg ����Ʒ�����ٱ���ͼʹ�ã�
;			set_sys_msg_mode 3 0 0
;			msg_info
;		
;		endif
;	endif
endproc
