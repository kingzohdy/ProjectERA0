;;;;;;

proc 1
	strvar val
	var val 0
	set_task_var 1363 0 8 @val 1

	strvar time
	var time 0
	set_task_var 1363 1 8 @time 1
	
	strvar mid1 mid2
	var mid1 0
	set_task_var 1363 4 32 @mid1 1
	var mid2 0
	set_task_var 1363 8 32 @mid2 1	
endproc 
proc 4
	DEL_ROLE_TMP_ITEM 0 1
	
	strvar mid
	get_player_id 1 mid
	TARGET_ADD_STATUS 1 @mid 3099 1 0 1

endproc 
proc 5
	
	DEL_ROLE_TMP_ITEM 0 1
	
	strvar mid
	get_player_id 1 mid
	TARGET_ADD_STATUS 1 @mid 3099 1 0 1
endproc 

proc 6

	get_task_var 1363 0 8 1 val
	if val = 1
	
		get_task_var 1363 1 8 1 time
		add time 1
		set_task_var 1363 1 8 @time 1
		
		strvar mid1 mid2		
		get_task_var 1363 4 32 1 mid1 		
		get_task_var 1363 8 32 1 mid2		

		
;		GET_MAP_NPC_ID 304075 mid1 1
;		GET_MAP_NPC_ID 304076 mid2 1
		
		
		if time = 2
			mon_speech @mid1 10 0 1 ����ô���ˣ�
		endif
		if time = 6
			mon_speech @mid2 0 0 1 �������ˣ�Ŷ�������峤���ˣ��������İ��ʯ�ѳ�����֪����Ӧ�ҵġ���
		endif
		if time = 10
			mon_speech @mid1 0 0 1 ��Ȼ��ɭ�ɡ��߶��������������Ĳв����ڡ���
		endif
		if time = 14
			mon_speech @mid2 0 0 1 �Ⲣ�������ǵ�Э��֮�У��������ˣ�
		endif
		if time = 18
			mon_speech @mid2 0 0 1 ���Ѿ�����չʾ���ҵĳ��⣬���ɱ�����ʧ���ģ�

		endif
		if time = 22
			mon_speech @mid1 0 0 1 �ðɣ��һᾡ�������ṩ�������˵����¶�̬��
		endif
		if time = 22
			mon_speech @mid2 0 0 1 ϣ������̫�ã�
			
		endif
		if time = 28
			task_fini 1363 1
			
			DEL_ROLE_TMP_ITEM 0 1
			var val 0
			set_task_var 1363 0 8 @val 1		

			strvar mid
			get_player_id 1 mid
			TARGET_ADD_STATUS 1 @mid 3099 1 0 1			
		endif
	endif
endproc

















