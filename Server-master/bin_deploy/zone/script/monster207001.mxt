

;;;;60��BOSS
proc 1
	strvar mid
	GET_MON_INFO 0 0 mid 0 0 0 0 0
	TARGET_ADD_STATUS 3 @mid 1095 1 25920000 1

endproc 

proc 2


	; strvar tim
	; GET_TIME_SEC tim
	; set_map_val 1 32 @tim 1
;;;;;;	������ʱ���¼ʱ��
	; get_map_val 0 8 1 50
	; var 50 0
	; SET_MAP_VAL 0 8 @50 1
;;;;;;;;;;;;;;;2.28ע��
	
	
;	add_sys_msg_start
;	add_sys_msg ��װ���ɻ�������ʮ���ſ�,������ȡ�����Ͻ������Դ��
;	set_sys_msg_mode 1 0 0
;	svr_msg_info
	strvar x y id
	GET_MON_INFO 1 0 0 0 0 0 x y
	NEW_LIFE_NPC 0 307035 @x @y 0 180 npc307035.mac
;	NEW_MON 200031 1 @x @y 0 0 0 1 0
	;�ӳƺ�
;	MON_GET_KILLER_ID 1 id
;	if 1 = 0
;		set_player_curr @id 1		
;	endif
;	set_desig 1 125 
;
;	;����ս���
;	strvar time sec
;	get_world_boss_first_kill_time 1 time
;	if time = 0
;		set_world_boss_first_kill_time 1
;		;����Ʒ
;		ADD_ITEM_ON_MAP 1 0 ::itemid:: 1 0 0 0 0
;		;����ʱ��,10����20��
;		get_time_sec sec
;		dev sec 86400
;		add sec 10
;		mul sec 86400
;;		add sec 72000
	;	set_world_city_enable_time 1 @sec
;	endif
endproc  

