;�������ս����    ,�Ʒ���Ŧ�ػ���,3

proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Ŧ�Ǿ���ս��ʤ���Ĺؼ���
	add_talk #b
	add_talk ս��ʤ������#c0181f9��ս������ʱ����
	add_talk #b#b#c186506������ֻʣһ��������Ŧ����Ŧ���ڵĹ�����Ӫʤ�������ȱ��ݻ���Ŧ�Ĺ���Ϊ��3����
	add_talk #b#b#c186506������ʣ����Ŧ����1��������ŦѪ�������������Σ�Ѫ�����Ĺ���ʤ����
	add_talk #b
	add_talk ��Ŧ����������ʩ��
	add_talk #b#b#c6d00d2�����������ܵ����˺�����50%������15��
	add_talk #b#b#c6d00d2�߼��������ܵ����˺�����95%������15��
	option 100 #��������
	option 200 #�߼�����

	option 400 #������ȴʱ��
	option 300 #��㿴��,�ټ�
	talk

;;	add_sys_msg_start
;;	add_sys_msg xyz
;;	set_sys_msg_mode 1 0 0
;;	map_msg_info

endproc



proc 100
	; ��������
;	call 301
	strvar cost
	var cost 0
	if cost <= 0
		call 101
	else
		choose 1 101 302 ����������������0 ��ң���Ŧ�ܵ����˺�����50%������15�룬ȷ�Ͽ�����
	endif

endproc

proc 101
	; ��������


	;״̬id
	set_call_var 100 3211

	;״̬�ȼ�
	set_call_var 101 1

	;����ʱ��
	set_call_var 102 15

	;����
	set_call_var 103 0

	;��ȴʱ��
	set_call_var 104 600

	;��������
	set_call_var 105 60
	
	call 301
endproc
proc 200
	; �߼�����
;	call 301
	strvar cost
	var cost 50
	if cost <= 0
		call 201
	else
		choose 1 201 302 �����߼���������50 ��ң���Ŧ�ܵ����˺�����95%������15�룬ȷ�Ͽ�����
	endif

endproc

proc 201
	; �߼�����


	;״̬id
	set_call_var 100 3211

	;״̬�ȼ�
	set_call_var 101 2

	;����ʱ��
	set_call_var 102 15

	;����
	set_call_var 103 50

	;��ȴʱ��
	set_call_var 104 600

	;��������
	set_call_var 105 60
	
	call 301
endproc

proc 301
	;��ȡ�������ĸ�������
	; ״̬id
	strvar id
	var id @100
	; ״̬�ȼ�
	strvar level
	var level @101
	; ����ʱ��
	strvar time
	var time @102
	; ����
	strvar cost
	var cost @103
	; ��ȴʱ��
	strvar cool
	var cool @104
	; ��������
	strvar moneytype
	var moneytype @105
;301�ĺ�һ������file��
	;�ж���ȴʱ��
	strvar sec sectip
	get_time_sec sec
	;camp3buffcool14,��Ӫ3buff��ȴʱ��
	var 14 0
	get_pworld_var 16 32 1 14
	if 14 > @sec
		sub 14 @sec
		add_sys_msg_start
		add_sys_msg �ò�������$@14$�����ȴʱ��,���Ժ�����   
		set_sys_msg_mode 4 0 0
		msg_info
		return 
	endif
	
	;�жϽ������
	dec_money_test 0 @cost @moneytype 2
	if 2 = 0
		;�۽��
		dec_money 0 @cost @moneytype 2
		;��״̬
		;TARGET_ADD_STATUS   #Ŀ������ #memid #statusID #statusLV #statusTM #����ֵ
		strvar memid
		;get_mon_info 1 0 memid 0 0 0 0 0
		;GET_MAP_MON_ID  #Monster �����ID  #���ص�ʵ��ID #���ر�־
		GET_MAP_MON_ID 261198 memid 1
		if 1 = 0
			TARGET_ADD_STATUS 3 @memid @id @level @time 1
		endif
		
		;������ȴʱ��
		add cool @sec
		;camp3buffcool14,��Ӫ3buff��ȴʱ��
		var 14 @cool
		set_pworld_var 16 32 @14 1
		add_sys_msg_start
		add_sys_msg $_ClanName$��$_Player$��������Ŧ���� 
		set_sys_msg_mode 4 0 0
		map_msg_info
		if cost > 0
			add_pworld_resource 0 1 5 1
		endif		
	else
		add_sys_msg_start
		add_sys_msg ��Ҳ���,�޷����д˲���   	
		set_sys_msg_mode 4 0 0
		msg_info
	endif
	
	
endproc
proc 302
endproc
proc 400
	choose 1 401 302 ������Ŧ������ȴʱ��300��,����200 ���,һ��ս����ֻ��ʹ��һ��,ȷ�Ϲ���?
endproc
proc 401
	;�ж���ȴʱ��
	strvar sec sectip
	get_time_sec sec
	;camp3buffcoolreduce33,��Ӫ3buff������ȴʱ��ʹ�ô���
	var 33 0
	get_pworld_var 92 32 1 33
	if 33 > 0
		add_sys_msg_start
		add_sys_msg һ��ս����,�˲���ֻ��ִ��һ��   
		set_sys_msg_mode 4 0 0
		msg_info
		return 
	endif
	;camp3buffcool14,��Ӫ3buff��ȴʱ��
	var 14 0
	get_pworld_var 16 32 1 14
	if 14 <= @sec
		add_sys_msg_start
		add_sys_msg ��Ŧ��������ȴ,����Ҫ�ٽ��д˲���   
		set_sys_msg_mode 4 0 0
		msg_info
		return 
	endif
	sub 14 @sec
	sub 14 300
	if 14 < 0
		var 14 0
	endif
	add 14 @sec
	
	;�жϽ������
	dec_money_test 0 200 60 2
	if 2 = 0
		;�۽��
		dec_money 0 200 60 2
		
		;���ü���
		add 33 1
		;camp3buffcoolreduce33,��Ӫ3buff������ȴʱ��ʹ�ô���
		
		set_pworld_var 92 32 @33 1
		;������ȴʱ��
		;camp3buffcool14,��Ӫ3buff��ȴʱ��
		
		set_pworld_var 16 32 @14 1
		add_sys_msg_start
		add_sys_msg $_ClanName$��$_Player$��������Ŧ������ȴʱ��������300�� 
		set_sys_msg_mode 4 0 0
		map_msg_info
		add_pworld_resource 0 1 5 1
	else
		add_sys_msg_start
		add_sys_msg ��Ҳ���,�޷����д˲���   	
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc
