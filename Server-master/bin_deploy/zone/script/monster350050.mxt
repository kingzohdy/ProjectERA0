;������
proc 2

	strvar x y
	GET_MON_INFO 0 0 0 0 0 0 x y
	effect 0 0 playEffct($@x$,$@y$,"effect\\entity\\9535_16.ent")

	;��������
;	del_npc 350062 1
;	del_npc 350061 1
;
;	del_npc 350047 1
;	del_npc 350049 1

	add_sys_msg_start
	strvar camp
	GET_ATTACKER_CAMP 1 camp
	if camp = 1
		add_sys_msg ��Ÿ����������޷����$_Player$ͻϮռ���ˣ����޷�ָ�ӹٽ����ڿ�Ϯ��

;		;���޷�����
;		NEW_CAMP_NPC 1 350047 11607 6789 0 1 battleoasisfly.mac
;
;		;ɳĮ������,��������Ч
;		NEW_CAMP_NPC 1 350062 17755 18901 0 2 battledesertfly.mac
	elseif camp = 2
		
		add_sys_msg ��Ÿ�������ɳĮ�����$_Player$ͻϮռ���ˣ�ɳĮ��ָ�ӹٽ����ڿ�Ϯ��
		
;		;ɳĮ������
;		NEW_CAMP_NPC 1 350049 17755 18901 0 2 battledesertfly.mac
;
;		;���޷�����,��������Ч
;		;NEW_CAMP_NPC #����ֵ #NpcID #����x #����y #���� #��Ӫ���� #�ű�
;		NEW_CAMP_NPC 1 350061 11607 6789 0 1 battleoasisfly.mac
		
	else
		add_sys_msg δ֪�����$_Player$ͻϮռ������Ÿ�������

;		;���޷�����,��������Ч
;		;NEW_CAMP_NPC #����ֵ #NpcID #����x #����y #���� #��Ӫ���� #�ű�
;		NEW_CAMP_NPC 1 350061 11607 6789 0 1 battleoasisfly.mac
;		;ɳĮ������,��������Ч
;		NEW_CAMP_NPC 1 350062 17755 18901 0 2 battledesertfly.mac
	endif
	set_sys_msg_mode 3 0 0
	MAP_MSG_INFO

	set_pworld_var 4 32 @camp 1

	NEW_DYN_MON 1 350050 1 17277 8574 0 0 0 2 @camp 0 0

;	SET_REVIVE_POS_CAMP 1 2 @camp

	

endproc
	