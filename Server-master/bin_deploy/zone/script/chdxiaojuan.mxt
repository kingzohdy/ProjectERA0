
;С��
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b���Ҹ���С�����Ѿ��м�ǧ�����أ�
	add_talk #b#b�㿴�ҵ��۾������ӡ����䡢��ͣ���һ����С�����ˣ��ҿ��������ۻ軨�ˡ���
	
	strvar ret
	task_stat 3316 1 ret
	if ret = 2
		option 150 #���������èè�ɣ������㣬�����ң���Σ�
	endif
	talk
endproc

proc 150
	strvar tmpid ret
	GET_ROLE_TMP_ITEM 0 tmpid
	HAS_STATUS 3171 ret
	if ret = 0
;	if tmpid = 1460083

	
		get_player_id 1 10
		TARGET_ADD_STATUS 1 @10 3171 1 0 0
	;	DEL_ROLE_TMP_ITEM  0 0
		task_fini 3316 1
	else
		add_sys_msg_start
		add_sys_msg �㲻��С�ᣬ�ҲŲ�Ҫ�����棡
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc 