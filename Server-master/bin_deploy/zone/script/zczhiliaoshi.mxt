proc 1
	strvar LVL Cost
	;GET_ID_PLAYER_INFO #��ɫʵ��ID��0��ʾ�Լ��� #AttrFlag  #����ֵ
	GET_ID_PLAYER_INFO 0 8 LVL
	var Cost @LVL
;	mul Cost 2
	
	strvar PetMaxHP PetHP
	GET_ID_PLAYER_INFO 0 12 PetHP
	GET_ID_PLAYER_INFO 0 13 PetMaxHP
	
	
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���ã����ѡ�����֮�����ģ��ὫΪ������Ԯ��֮�֣��Բ���֮�ĸл����ˣ����š�
	add_talk #c186506��ʥף�������ָ���������ֵ��ħ��ֵ���ޣ�����������ֵ����#n
;	add_talk #c186506�����ꡪ�������������ϵĺ���״̬#n
;	add_talk #c186506������ӡ��������ﾭ��Ӹ�ֵ�ָ�����ֵ#n
	
	if LVL < 30
		OPTION 105 #��ʥ��ף����30������������ƣ�
	else
		OPTION 98 #��ʥ��ף����������$@Cost$������
	;OPTION 104 #���Ƶ�ǰ���������$@PetMaxHP$���ң�
	endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		strvar Lv2 Mu2
		GET_ID_PLAYER_INFO 0 8 Lv2	
		var Mu2 @Lv2
		mul Mu2 @Lv2
		mul Mu2 2	
;		option 150 #�����꣨������$@Mu2$���ң�
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		strvar Lvs Mu1
		GET_ID_PLAYER_INFO 0 8 Lvs	
		
		var Mu1 @Lvs
		mul Mu1 @Lvs
		dev Mu1 2	
;		option 151 #������ӣ�������$@Mu1$��ȯ��

	talk
endproc
proc 150
	strvar Mid Ret Lv2 Mu2 Moy
	GET_PLAYER_ID 1 Mid
	GET_ID_PLAYER_INFO 0 8 Lv2	
	var Mu2 @Lv2
	mul Mu2 @Lv2
	mul Mu2 2
	DEC_MONEY_TEST 0 @Mu2 20 Moy
	if Moy = -1	
		add_sys_msg_start
		add_sys_msg �������е����Ҳ��㣡
		set_sys_msg_mode 4 0 0
		msg_info
	else
		strvar Rete
		GET_PLAYER_PKVAL_TYPE @Mid Rete
		if Rete = 1
			add_sys_msg_start
			add_sys_msg ����ʱ�Բ���Ҫ���������Ը�������������ҡ�
			set_sys_msg_mode 4 0 0
			msg_info
		else
			add_exp @Mid -1 Ret
			DEC_MONEY @Mid @Mu2 20 1
		endif
	endif
endproc

proc 151

	strvar Mid Ret Lv Mu1 Moy
	GET_PLAYER_ID 1 Mid
	GET_ID_PLAYER_INFO 0 8 Lv	
	var Mu1 @Lv
	mul Mu1 @Lv
	dev Mu1 2
	DEC_MONEY_TEST 0 @Mu1 20 Moy
	if Moy = -1	
		add_sys_msg_start
		add_sys_msg �������е����Ҳ��㣡
		set_sys_msg_mode 4 0 0
		msg_info
	else
		add_exp @Mid 0 Ret
		if Ret = -1
			add_sys_msg_start
			add_sys_msg ����ʱ�Բ���Ҫ���������Ը�������������ҡ�
			set_sys_msg_mode 4 0 0
			msg_info
		else
			DEC_MONEY @Mid @Mu1 20 10	
		endif
	endif
endproc 


proc 98
	strvar Cost LVL
	GET_ID_PLAYER_INFO 0 8 LVL
	var Cost @LVL

	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < @Cost
		choose 1 101 299 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	
	elseif ret < @Cost
		choose 1 101 299 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 101
	endif
endproc 
proc 299
	return
endproc

proc 101
	strvar MaxHP HP MaxMP MP LVL Ret1 Cost
	strvar PetMaxHP PetHP
	GET_ID_PLAYER_INFO 0 2 HP
	GET_ID_PLAYER_INFO 0 9 MaxHP
	GET_ID_PLAYER_INFO 0 11 MP
	GET_ID_PLAYER_INFO 0 10 MaxMP
	
	GET_ID_PLAYER_INFO 0 12 PetHP
	GET_ID_PLAYER_INFO 0 13 PetMaxHP
	GET_ID_PLAYER_INFO 0 8 LVL
	var Cost @LVL
;	mul Cost 2

			
	if @HP < @MaxHP or @MP < @MaxMP or @PetHP < @PetMaxHP
	
			;DEC_MONEY ��ɫID ���ٵ�Ǯ Ǯ���� ���ر��
			
		DEC_MONEY_TEST 0 @Cost 21 Ret1	
		
		if Ret1 = 0
			strvar ret
			DEC_MONEY 0 @Cost 21 ret
			ADD_PLAYER_HP 0 999999 0
			ADD_PLAYER_MP 0 999999 0
			ADD_FIGHT_PET_HP 0 999999 0
			
			add_sys_msg_start
			add_sys_msg ����ܵ���ʥ��Ĺ�â������ֵ��ħ��ֵ������
			set_sys_msg_mode 4 0 0
			msg_info
		else
			add_sys_msg_start
			add_sys_msg ���İ������㣬�޷����ƣ�
			set_sys_msg_mode 4 0 0
			msg_info	
		endif
	else

		add_sys_msg_start
		add_sys_msg ���״̬���ã�����Ҫ�ҵ�Ԯ����
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc



proc 104
	strvar Ret8 PetMaxHP PetHP
	strvar MaxHP HP MaxMP MP LVL Ret1 Cost
	
	GET_ID_PLAYER_INFO 0 12 PetHP
	GET_ID_PLAYER_INFO 0 13 PetMaxHP
	if PetHP = -1 or @PetMaxHP = -1
		add_sys_msg_start
		add_sys_msg ��δ�г����ս���޷����ƣ�
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	;DEC_MONEY ��ɫID ���ٵ�Ǯ Ǯ���� ���ر��
	DEC_MONEY 0 @LVL 21 Ret8
	
	if Ret8 = 0
		if PetHP < @PetMaxHP	
				if Ret8 = 0
					ADD_FIGHT_PET_HP 0 999999 0
					add_sys_msg_start
					add_sys_msg ��������ֵ�ѻ�����
					set_sys_msg_mode 4 0 0
					msg_info
				else
					return
				endif
			else
					add_sys_msg_start
					add_sys_msg ��ĳ���״̬���ã�����Ҫ�ҵ�Ԯ����
					set_sys_msg_mode 4 0 0
					msg_info
			endif		
	else
		add_sys_msg_start
		add_sys_msg �������Ҳ��㣬�޷����ƣ�
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc

proc 105
	strvar MaxHP HP MaxMP MP LVL Ret1 Cost
	strvar PetMaxHP PetHP
	GET_ID_PLAYER_INFO 0 2 HP
	GET_ID_PLAYER_INFO 0 9 MaxHP
	GET_ID_PLAYER_INFO 0 11 MP
	GET_ID_PLAYER_INFO 0 10 MaxMP
	
	GET_ID_PLAYER_INFO 0 12 PetHP
	GET_ID_PLAYER_INFO 0 13 PetMaxHP
	GET_ID_PLAYER_INFO 0 8 LVL


			
	if @HP < @MaxHP or @MP < @MaxMP or @PetHP < @PetMaxHP



		ADD_PLAYER_HP 0 999999 0
		ADD_PLAYER_MP 0 999999 0
		ADD_FIGHT_PET_HP 0 999999 0
		
				
		add_sys_msg_start
		add_sys_msg ����ܵ���ʥ��Ĺ�â������ֵ��ħ��ֵ������
		set_sys_msg_mode 4 0 0
		msg_info
	else
		add_sys_msg_start
		add_sys_msg ���״̬���ã�����Ҫ�ҵ�Ԯ����
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc
