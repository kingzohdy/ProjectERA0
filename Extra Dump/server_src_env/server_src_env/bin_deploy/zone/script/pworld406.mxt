;�����¼�
PROC 1					
	
	
	;��2��boss
	;����Ϊ0,����Ϊ1,ɳĮΪ2
	;Ůboss,����
	strvar bossA
	strvar bossACamp
	strvar minLevel maxLevel dynLevel
	GET_BATTLE_LEVEL_LIMIT 1 minLevel maxLevel
	var dynLevel @maxLevel
	sub dynLevel @minLevel
	dev dynLevel 2
	add dynLevel 1
	add dynLevel @minLevel

	var bossA 282071
	var bossACamp 1
	NEW_DYN_MON 1 @bossA 1 4711 4311 0 0 0 2 @bossACamp @dynLevel 1

	;��boss,ɳĮ
	var bossA 282070
	var bossACamp 2
	NEW_DYN_MON 1 @bossA 1 25911 21311 0 0 0 2 @bossACamp @dynLevel 1

	;��4���赲
	call 100
	
	;��2����̬�����

	var bossA 350046
	;var bossA 350046
	var bossACamp 0
	NEW_DYN_MON 1 @bossA 1 16011 12511 0 0 0 2 @bossACamp 0 0

	var bossA 350048
	var bossACamp 0
	NEW_DYN_MON 1 @bossA 1 21111 6811 0 0 0 2 @bossACamp 0 0

	;һ����Ÿ�����������

	var bossA 350050
	var bossACamp 0
	NEW_DYN_MON 1 @bossA 1 17277 8574 0 0 0 2 @bossACamp 0 0

	;���޷�����,��������Ч
	;NEW_CAMP_NPC #����ֵ #NpcID #����x #����y #���� #��Ӫ���� #�ű�
	;NEW_CAMP_NPC 1 350061 11607 6789 0 1 battleoasisfly.mac

	;ɳĮ������,��������Ч
	;NEW_CAMP_NPC #����ֵ #NpcID #����x #����y #���� #��Ӫ���� #�ű�
	;NEW_CAMP_NPC 1 350062 17755 18901 0 2 battledesertfly.mac

	;���޴�����
	;NEW_CAMP_NPC 1 333038 5929 2851 0 1 battleoasisout.mac
	;ɳĮ������
	;NEW_CAMP_NPC 1 333039 26107 21261 0 2 battledesertout.mac

	add_sys_msg_start
	add_sys_msg ս��1���Ӻ�ʼ������ʱ����ʱ�����һ����Ӫ����3�ˣ�ս�����رգ�
	set_sys_msg_mode 1 0 0
	map_msg_info
	
	;���ø���������10��ʱ��
	var 10 0
	set_pworld_var 0 32 @10 1
	;11��Ÿ���������Ӫ,Ĭ��0����
	var 11 0
	set_pworld_var 4 32 @11 1
	;����Ůboss��ʱ
	VAR 12 0
	set_pworld_var 8 32 @12 1
	;ɳĮ��boss��ʱ
	VAR 13 0
	set_pworld_var 12 32 @13 1
	;lock1���Ӻ�ʼ��Ѫ
	strvar Lock
	var Lock 0
	set_pworld_var 16 32 @Lock 1
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
	;��¼��ǰ��������
	strvar pworld_lv
	var pworld_lv 406
	set_pworld_var 32 32 @pworld_lv 1
ENDPROC

;�ر��¼�
proc 2
	add_sys_msg_start
	add_sys_msg ս������
	set_sys_msg_mode 4 0 0
	map_msg_info
endproc

;2��һ��
PROC 3
	get_pworld_var 0 32 1 10
	if 10 < 0
		return
	endif
	ADD 10 1
	set_pworld_var 0 32 @10 1
	strvar num
	;һ���Ӻ�,ɾ�赲,ս����ʼ
	if 10 = 30
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
		;ɾ�赲
		delete_mon_by_defid 294008 1
		;���ƿ�ʼ��Ѫ�ı���
		get_pworld_var 16 32 1 Lock 1
		var Lock 10
		set_pworld_var 16 32 @Lock 1
		add_sys_msg_start
		add_sys_msg ս����ʼ��ָ�ӹٻ��ܵ��з���Ϯ�˺���ռ��������ɸ��ŵз���Ϯ�ķ�����
		set_sys_msg_mode 4 0 0
		map_msg_info
		return
	endif
	if 10 > 30
		var 40 @10
		mod 40 30
		if 40 = 0
			add_sys_msg_start
			add_sys_msg ռ���ͼ�����������ʹ������Ӫָ�ӹ����ڿ�Ϯ��
			set_sys_msg_mode 3 0 0
			map_msg_info
		endif
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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
		if Num != 0
			var TimC 0
			set_pworld_var 20 32 @TimC 1
			add_sys_msg_start
			add_sys_msg ս������Ԯ�����룬ս���رյ���ʱ��ͣ������ս���ɣ�
			set_sys_msg_mode 1 0 0
			map_msg_info
			return
		endif
			
		;;;;;5;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;1����֮�ڣ�ÿ10����ʾһ��
	
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
		;ս��һ��û��ʱ5���Ӻ�ر�
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
		;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
	
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
	add_sys_msg ��ɱ�Է���Ӫָ�ӹټ��ɻ��ʤ��
	set_sys_msg_mode 7 0 0
	msg_info
	
	EFFECT 2 1 SetMessageTips(200)

ENDPROC

proc 100
	;����
	;NEW_MON #����id #�������� #����x #����y #����뾶 #Ѳ��ID #�Ƿ�������� #���ر�־ #��һֻ�����ʵ��ID
	;NEW_MON 230014 4 7003 3532 0 0 0 1 2
	var 10 49
	new_mon_by_dir 294008 1 6464 3871 0 0 0 @10 1 2
	new_mon_by_dir 294008 1 6410 3532 0 0 0 @10 1 2
	
	var 10 0
	new_mon_by_dir 294008 1 5194 6413 0 0 0 @10 1 2
	new_mon_by_dir 294008 1 4822 6435 0 0 0 @10 1 2 
	new_mon_by_dir 294008 1 5004 6437 0 0 0 @10 1 2

	;ɳĮ
	var 10 0
	new_mon_by_dir 294008 1 24949 17835 0 0 0 @10 1 2
	new_mon_by_dir 294008 1 25306 17840 0 0 0 @10 1 2
	new_mon_by_dir 294008 1 25694 17867 0 0 0 @10 1 2
	new_mon_by_dir 294008 1 25504 17850 0 0 0 @10 1 2
	new_mon_by_dir 294008 1 25169 17859 0 0 0 @10 1 2
	;new_mon_by_dir 294008 1 24718 17832 0 0 0 @10 1 2
	var 10 45
	new_mon_by_dir 294008 1 23435 20293 0 0 0 @10 1 2
	new_mon_by_dir 294008 1 23436 19893 0 0 0 @10 1 2
	new_mon_by_dir 294008 1 23442 20066 0 0 0 @10 1 2
endproc

;����Ϊ������,��ɾ��
;����ս��,������
PROC 10
	pworld_again
ENDPROC

;���޷������Ľű�zcflyblue,
;---��Ҫ����·��
proc 11
	autofly 114 1
endproc

;ɳĮ�������Ľű�zcflyred,��ʱ����
;---��Ҫ����·��
proc 12
	autofly 115 1
endproc

;���͵�ű�,outbattle
PROC 13
        choose 2 14 15 �Ƿ�Ҫ�뿪��ǰս����
ENDPROC

proc 14
        OUT_PWORLD_BY_SINGLE
endproc

proc 15

endproc

proc 16
	;Ůboss,����
	strvar bossA
	strvar bossACamp
	var bossA 282071
	var bossACamp 1
	NEW_DYN_MON 1 @bossA 1 25957 19875 0 0 0 2 @bossACamp 0 0

	;��boss,ɳĮ
	var bossA 282070
	var bossACamp 2
	NEW_DYN_MON 1 @bossA 1 5838 5400 0 0 0 2 @bossACamp 0 0
endproc


proc 101
	delete_mon_by_defid 294008 1
endproc

proc 102
	var 10 45
	new_mon_by_dir 294008 1 5480 4687 0 0 0 @10 1 2
endproc

proc 103
		
	del_npc 350062 1
	del_npc 350061 1

	del_npc 350047 1
	del_npc 350049 1
endproc