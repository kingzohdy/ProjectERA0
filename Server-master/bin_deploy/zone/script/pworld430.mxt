;�����¼�
PROC 1
	strvar bossA
	var bossA 294050
	var 11 135
	var 12 45
	new_mon_by_dir @bossA 1 22821 12815 0 0 0 @11 1 2
	new_mon_by_dir @bossA 1 3559 12803 0 0 0 @12 1 2

	add_sys_msg_start
	add_sys_msg ս��1���Ӻ�ʼ������ʱ����ʱ�����һ����Ӫ����3�ˣ�ս�����رգ�
	set_sys_msg_mode 1 0 0
	map_msg_info
	
	;���ø���������10��ʱ��
	var 10 0
	set_pworld_var 0 32 @10 1

	;;�˳�ս��
	strvar TimC
	var TimC 0
	set_pworld_var 20 32 @TimC 1
	;�˳�ս��2
	strvar TimLock
	var TimLock 5
	set_pworld_var 24 32 @TimLock 1
	;�˳�ս��3
	strvar TimPover
	var TimPover 0
	set_pworld_var 28 32 @TimPover 1	
ENDPROC

;�ر��¼�
proc 2
endproc

;2��һ��
PROC 3
	get_pworld_var 0 32 1 10
	if 10 < 0
		return
	endif
	ADD 10 1
	set_pworld_var 0 32 @10 1

	;һ���Ӻ�,ɾ�赲,ս����ʼ
	if 10 = 30
		call 150
		
		;ɾ�赲
		delete_mon_by_defid 294050 1
		add_sys_msg_start
		add_sys_msg �����ɣ����꣡�ƶ����ӿ������ݻٵз�Ҫ����
		set_sys_msg_mode 3 0 0
		map_msg_info
		return
	endif
	
	if 10 > 30
		strvar Pnum
		GET_PWORLD_ROLE_NUM Pnum 1
		;ս��û��ʱ�ر�
		if Pnum = 0
			pworld_over
			COMPEL_CLOSE_PWORLD 1			
		endif
		get_pworld_var 20 32 1 TimC 
		if TimC = 2
		;ս��һ��û��ʱ��ʼ5���Ӽ�ʱ
		
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;5���Ӽ�ʱ�����У����˽������жϵ���ʱ	
			BATTLE_MIN_CAMP_PLAYER_NUM 1 num	
			if num != 0
				var TimC 0
				set_pworld_var 20 32 @TimC 1
				add_sys_msg_start
				add_sys_msg ս������Ԯ�����룬ս���رյ���ʱ��ͣ������ս���ɣ�
				set_sys_msg_mode 1 0 0
				map_msg_info
				return
			endif
			
			;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;1����֮�ڣ�ÿ10����ʾһ��
			get_pworld_var 24 32 1 TimLock
			if TimLock = 1
				var 30 @10
				mod 30 5
				if 30 = 0
					get_pworld_var 28 32 1 TimPover
					add TimPover 10
					set_pworld_var 28 32 @TimPover 1
					var 50 60
					sub 50 @TimPover
					if 50 <= 0
						pworld_over
						COMPEL_CLOSE_PWORLD 1	
					endif
					
					add_sys_msg_start
					add_sys_msg ���ڵз���Ӫ��ɫȫ���˳�ս����$@50$���ս������رգ�
					set_sys_msg_mode 4 0 0
					map_msg_info
					
				endif
				return
			endif
		
			var 15 @10
			mod 15 30
			if 15 = 0
				sub TimLock 1
				set_pworld_var 24 32 @TimLock 1
				
				add_sys_msg_start
				add_sys_msg ���ڵз���Ӫ��ɫȫ���˳�ս����$@TimLock$���Ӻ�ս������رգ�
				set_sys_msg_mode 4 0 0
				map_msg_info
			endif	
			return
		endif
		
		;ս��һ��û��ʱ5���Ӻ�رգ�ÿ�������һ��
		BATTLE_MIN_CAMP_PLAYER_NUM 1 num	
		if num = 0
			add_sys_msg_start
			add_sys_msg ���ڵз���Ӫ��ɫȫ���˳�ս����5���Ӻ�ս������رգ�
			set_sys_msg_mode 4 0 0
			map_msg_info
			
			get_pworld_var 20 32 1 TimC
			var TimC 2
			set_pworld_var 20 32 @TimC 1
			
		endif
		return
	endif

	var 11 30
	sub 11 @10

	mod 10 5
	if 10 = 0
		mul 11 2
		add_sys_msg_start
		add_sys_msg ս��$@11$���ʼ������ʱ����ʱ�����һ����Ӫ����3�ˣ�ս�����رգ�
		set_sys_msg_mode 4 0 0
		map_msg_info
	endif
ENDPROC

PROC 5
	;��ɫ���븱��
	add_sys_msg_start
	add_sys_msg �ڵ�ͼ��Ѱ����Դ�ᾧ���ɼ����������������ӿ�����ǰ��
	set_sys_msg_mode 3 0 0
	MSG_INFO
	
	EFFECT 2 1 SetMessageTips(210)
ENDPROC


PROC 150
	strvar num
	BATTLE_MIN_CAMP_PLAYER_NUM 1 num
	if num < 3
		add_sys_msg_start
		add_sys_msg ����һ����Ӫ����3��,ս���رգ������±�����
		set_sys_msg_mode 4 0 0
		map_msg_info

		pworld_over
		COMPEL_CLOSE_PWORLD 1
		return
	endif
ENDPROC
