proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�����#Race#�����Ƿ���Ϊ��Ƭ��������ţ��Ƿ���Ϊδ֪��Σ�ն����ģ����ģ��ҿ��԰�����ɹ�һ���ֶ�ֻҪ����ռ�������꣬�㽫����ޱ�ǿ�󣡰������꽻���ң�$_palyer$���㽫��������ױȵ�ս������
		
		strvar Loc3 Loc4
		get_pworld_var 11 8 1 Loc3 
		get_pworld_var 13 8 1 Loc4 
		if Loc3 = 0 and Loc4 = 0
			option 130 #ǩ�������Լ
			option 131 #���룡�ҵ�������޼�֮����
		endif
	talk
				
endproc

proc 130
	
	strvar Loc3 Mid
	get_pworld_var 0 8 1 50
	var 50 1
	set_pworld_var 0 8 @50 1
	
	GET_MAP_NPC_ID 282019 Mid 1
	mon_speech @Mid 2 0 1 �����˸����ǵ�ѡ��������
	
	get_pworld_var 11 8 1 Loc3 
	var Loc3 1
	set_pworld_var 11 8 @Loc3 1
	
	effect 0 0 playEffct(5551,9508,"effect\\entity\\282013_18.ent")
	EFFECT_ID 0 0 setOverlayRegion("����_4")
	
endproc 

proc 131
;	del_npc 282019 1
;	NEW_MON 282026 1 5551 9508 500 0 0 0 0
	
	strvar Loc4 Mid
	get_pworld_var 13 8 1 Loc4 
	var Loc4 1
	set_pworld_var 13 8 @Loc4 1
	
	
	GET_MAP_NPC_ID 282019 Mid 1
	mon_speech @Mid 2 0 1 �޴���#Race#���㽫������ʹ�Ĵ��ۣ�
	effect 0 0 playEffct(5551,9508,"effect\\entity\\9512_16.ent")
	
	
endproc 











