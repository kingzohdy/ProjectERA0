;�����¼�
PROC 1					
	
	;NEW_DYN_MON #����ֵ #����DefID #�������� #����x #����y #����뾶 #Ѳ��ID #�Ƿ�������� #��һֻ�����ʵ��ID #��Ӫ���� #�Ѷ� #����
	;��2��boss
	;����Ϊ0,����Ϊ1,ɳĮΪ2
	;Ůboss,����
	strvar bossA
	strvar bossACamp
	var bossA 282036
	var bossACamp 1
	NEW_DYN_MON 1 @bossA 1 5838 5400 0 0 0 2 @bossACamp 0 0

	;��boss,ɳĮ
	var bossA 282035
	var bossACamp 2
	NEW_DYN_MON 1 @bossA 1 25957 19875 0 0 0 2 @bossACamp 0 0

	;��4���赲
	;����
	;NEW_MON #����id #�������� #����x #����y #����뾶 #Ѳ��ID #�Ƿ�������� #���ر�־ #��һֻ�����ʵ��ID
	;NEW_MON 230014 4 7003 3532 0 0 0 1 2
	new_mon_by_dir 294008 1 7003 3532 0 0 0 0 1 2

	new_mon_by_dir 294008 1 5048 6978 0 0 0 90 1 2

	;ɳĮ

	new_mon_by_dir 294008 1 25478 17812 0 0 0 0 1 2

	new_mon_by_dir 294008 1 25100 17799 0 0 0 0 1 2

	new_mon_by_dir 294008 1 23500 19998 0 0 0 90 1 2

	new_mon_by_dir 294008 1 23441 20404 0 0 0 90 1 2
	
	;��2����̬�����

	var bossA 350046
	;var bossA 350046
	var bossACamp 0
	NEW_DYN_MON 1 @bossA 1 12754 14761 0 0 0 2 @bossACamp 0 0

	var bossA 350048
	var bossACamp 0
	NEW_DYN_MON 1 @bossA 1 20947 7188 0 0 0 2 @bossACamp 0 0

	;һ����Ÿ�����������

	var bossA 350050
	var bossACamp 0
	NEW_DYN_MON 1 @bossA 1 15221 8873 0 0 0 2 @bossACamp 0 0

	;���޷�����
	var bossA 350047
	;NEW_CAMP_NPC #����ֵ #NpcID #����x #����y #���� #��Ӫ���� #�ű�
	NEW_CAMP_NPC 1 350047 11483 5793 0 1 battleoasisfly.mac

	;ɳĮ������
	var bossA 350049
	;NEW_CAMP_NPC #����ֵ #NpcID #����x #����y #���� #��Ӫ���� #�ű�
	NEW_CAMP_NPC 1 350049 18100 18479 0 2 battleoasisfly.mac

	;���޴�����
	NEW_CAMP_NPC 1 333038 5929 2851 0 1 battledesertout.mac
	;ɳĮ������
	NEW_CAMP_NPC 1 333039 26107 21261 0 2 battledesertout.mac

	add_sys_msg_start
	add_sys_msg ս��������ʼ
	set_sys_msg_mode 4 0 0
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
	ADD 10 1
	set_pworld_var 0 32 @10 1
	;һ���Ӻ�,ɾ�赲,ս����ʼ
	if 10 = 30
		;ɾ�赲
		delete_mon_by_defid 294008 1

		add_sys_msg_start
		add_sys_msg ս����ʼ
		set_sys_msg_mode 4 0 0
		map_msg_info

	endif


ENDPROC

PROC 4
	

ENDPROC

;����ս��,������
PROC 10
	pworld_again
ENDPROC

;����Ϊ��ʱ��������Ľű�

;�����������Ľű�zcflyblue,
;---��Ҫ����·��
proc 11
	autofly 106 1
endproc

;�췽�������Ľű�zcflyred,��ʱ����
;---��Ҫ����·��
proc 12
	autofly 107 1
endproc

;���͵�ű�,outbattle
PROC 13
        choose 2 10 11 �Ƿ�Ҫ�뿪��ǰս����
ENDPROC

proc 14
        OUT_PWORLD_BY_SINGLE
endproc

proc 15

endproc
