;�����ػ���Ŧ�    ,����buff
proc 1
	call 100
;	DIALOG 1
;	add_talk $_npc$:
;	add_talk #b#b��������ڸ����ĵ���Խ��Խ�࣬Խ��Խǿ��������Ǿ��÷��س����Ļ����������⣬���ܸ������ṩһ���İ�����
;	add_talk #b#b��е״̬��ʾ�ڻ�е�������Ϸ�
;	is_pworld_over 1
;	if 1 = 0
;		option 100 #ǿ��ʹ�õĻ�е
;
;	endif
;
;	talk
endproc
proc 100
	DIALOG 1
	add_talk $_npc$:
;	add_talk #b#bϣ����ʻ�Ļ�е����ǿ���������ṩ��������������;����
;	add_talk #b#b5��10����״̬���ڱ�����ʹ�õ����ظ����
	add_talk #b#b��������ڸ����ĵ���Խ��Խ�࣬Խ��Խǿ��������Ǿ��÷��س����Ļ����������⣬���ܸ������ṩһ���İ�����
	add_talk #b#b#c186506��ʾ:��е״̬��ʾ�ڻ�е�������Ϸ�#n
	is_pworld_over 1
	if 1 = 0
		strvar level time costtype cost itemid itemnum ret statusID
		var level 0
		get_role_ride_on_machine_mem_id 20 0
		;GET_ID_MON_STATUS_INFO  #����ʵ���ɣ� #״̬ID #���ر�־ #�ȼ� #���Ӳ��� #ʩ����ID #������Ϣ����ʱ���ã�
		;GET_MACHINE_PWORLD_STATUS_INFO #���ر�־ #״̬ID #״̬�ȼ� #״̬ʱ�� #��Ǯ���� #�۸� #��Ʒid #��Ʒ����
		var level 0
		get_id_mon_status_info @20 3167 3 level 0 0 0		
		add level 1
		GET_MACHINE_PWORLD_STATUS_INFO ret 3167 @level time costtype cost itemid itemnum removeitem removenum
		if ret = 0
			option 110 ������$@level$������������ӹ�������
		endif
		var level 0
		get_id_mon_status_info @20 3168 3 level 0 0 0		
		add level 1
		GET_MACHINE_PWORLD_STATUS_INFO ret 3168 @level time costtype cost itemid itemnum removeitem removenum
		if ret = 0
			;option 120 �ּ�$@level$�������������Ѫ�����ޣ�
		endif
		var level 0
		get_id_mon_status_info @20 3169 3 level 0 0 0		
		add level 1
		GET_MACHINE_PWORLD_STATUS_INFO ret 3169 @level time costtype cost itemid itemnum removeitem removenum
		if ret = 0
			option 130 ���׶�$@level$������������ӷ�����
		endif
	endif
	talk
endproc
proc 110
	strvar level time costtype cost itemid itemnum ret statusID
	get_role_ride_on_machine_mem_id 20 0
	var level 0
	get_id_mon_status_info @20 3167 3 level 0 0 0		
	add level 1
	GET_MACHINE_PWORLD_STATUS_INFO ret 3167 @level time costtype cost itemid itemnum removeitem removenum
	set_money_type 1 @costtype
	if ret = 0
		choose 2 111 112 ���������$@level$����Ҫ����$@cost$$_MoneyType$��ȷ�Ϲ���
	endif
endproc
proc 111
	strvar level time costtype cost itemid itemnum ret statusID removeitem removenum
	get_role_ride_on_machine_mem_id 20 0
	var level 0
	get_id_mon_status_info @20 3167 3 level 0 0 0		
	add level 1
	GET_MACHINE_PWORLD_STATUS_INFO ret 3167 @level time costtype cost itemid itemnum removeitem removenum
	if ret = 0
		set_call_var time @time
		set_call_var costtype @costtype
		set_call_var cost @cost
		set_call_var itemid @itemid
		set_call_var itemnum @itemnum
		set_call_var statusID 3167
		set_call_var level @level
		set_call_var removeitem @removeitem
		set_call_var removenum @removenum
		call 140
	endif
endproc
proc 112
endproc
proc 140
	strvar level time costtype cost itemid itemnum ret statusID ret1  removeitem removenum
	get_role_ride_on_machine_mem_id 20 0
	if 20 <= 0
		add_sys_msg_start
		add_sys_msg ����ʧ��,��Ҫ��˻�е���ܹ���  
		set_sys_msg_mode 1 0 0
		msg_info
		return
	endif
	DEC_MONEY_TEST 0 @cost @costtype 1
	set_money_type 2 @costtype
	if 1 != 0
		add_sys_msg_start
		add_sys_msg $_MoneyType$����,�޷����д˲���  
		set_sys_msg_mode 1 0 0
		msg_info
		return
	endif
	
	if itemnum > 0 and itemid != 0
		add_item_num_inpak @itemid @itemnum 2 ret1
		if ret1 != 0
			add_sys_msg_start
			add_sys_msg ��������,�޷����д˲���  
			set_sys_msg_mode 1 0 0
			msg_info
			return
		endif
		add_item_num_inpak  @itemid @itemnum 0 ret1
	endif
	DEC_ITEM_NUM_INPAK @removeitem @removenum 1
	dec_money 0  @cost @costtype 1
	TARGET_ADD_STATUS 3 @20 @statusID @level @time 1
	
	call 100
endproc
proc 120
	strvar level time costtype cost itemid itemnum ret statusID
	get_role_ride_on_machine_mem_id 20 0
	var level 0
	get_id_mon_status_info @20 3168 3 level 0 0 0		
	add level 1
	GET_MACHINE_PWORLD_STATUS_INFO ret 3168 @level time costtype cost itemid itemnum removeitem removenum
	set_money_type 1 @costtype
	if ret = 0
		choose 2 121 122 ����ּ�$@level$����Ҫ����$@cost$$_MoneyType$��ȷ�Ϲ���
	endif
endproc
proc 121
	strvar level time costtype cost itemid itemnum ret statusID
	get_role_ride_on_machine_mem_id 20 0
	var level 0
	get_id_mon_status_info @20 3168 3 level 0 0 0		
	add level 1
	GET_MACHINE_PWORLD_STATUS_INFO ret 3168 @level time costtype cost itemid itemnum removeitem removenum
	if ret = 0
		set_call_var time @time
		set_call_var costtype @costtype
		set_call_var cost @cost
		set_call_var itemid @itemid
		set_call_var itemnum @itemnum
		set_call_var statusID 3168
		set_call_var level @level
		call 140
	endif
endproc
proc 122
endproc
proc 130
	strvar level time costtype cost itemid itemnum ret statusID
	get_role_ride_on_machine_mem_id 20 0
	var level 0
	get_id_mon_status_info @20 3169 3 level 0 0 0		
	add level 1
	GET_MACHINE_PWORLD_STATUS_INFO ret 3169 @level time costtype cost itemid itemnum removeitem removenum
	set_money_type 1 @costtype
	if ret = 0
		choose 2 131 132 ������׶�$@level$����Ҫ����$@cost$$_MoneyType$��ȷ�Ϲ���
	endif
endproc
proc 131
	strvar level time costtype cost itemid itemnum ret statusID
	get_role_ride_on_machine_mem_id 20 0
	var level 0
	get_id_mon_status_info @20 3169 3 level 0 0 0		
	add level 1
	GET_MACHINE_PWORLD_STATUS_INFO ret 3169 @level time costtype cost itemid itemnum removeitem removenum
	if ret = 0
		set_call_var time @time
		set_call_var costtype @costtype
		set_call_var cost @cost
		set_call_var itemid @itemid
		set_call_var itemnum @itemnum
		set_call_var statusID 3169
		set_call_var level @level
		call 140
	endif
endproc
proc 132
endproc
