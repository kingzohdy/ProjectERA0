proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b���ǻ������̻���Ҫ��Ӫó������ҵ��ӵ���ŷḻ�����Դ�����ջ�Ұһ������Ȼ��Ϊ����������ص�·�ߡ������Ҳ��׬ȡ���ı��꣬���������Э�����䣬�������ڼ�ȱ�����С�
	add_talk #b
	add_talk #b#b#c186506����[55��]-����ﵽ55�����ǰ����ȡ����������ɺ󽫻�÷��ľ��齱��

	add_talk #b#b#c0181f9�û������ϵġ�F12��������������Χ����ң�ʹ������ҵ��Լ��Ļ���#n
	add_talk #b#b#c186506<ӵ�����ǻ�����Ȩ����������20%�ƶ��ٶ�>
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300782 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 600 �����뻻����
		endif
	endif
	
	
	task_stat 4497 1 20
	if 20 = 2
		option 200 #��������̻��������һ������
	endif
	
	;OPTION 101 #(����)�����̵�
	

	strvar car
	get_dart_car_type 1 car
	if car != 6
		strvar ret num
		reset_dart_car_check ret
		GET_ITEM_NUM_INPAK 5300944 1 num
		
		if ret = 0 and num > 0 
			option 401 #��������
		endif	
	else
		
		if car = 6
			task_stat 1096 1 20
			if 20 = 2
				option 450 #���޻���
			endif
		endif
	endif
	strvar ret
	GET_ITEM_NUM_INPAK 5300945 1 num
	task_stat 1096 1 ret
	if ret = 2 and num > 0
		option 300 #�ύ��������
	endif	
	
	;����
	;(�¼����ˡ�����֪���٣������񡪡�hnz��
	task_stat 1097 1 10
	if 10 = 2
	option 701 #����׼����������ʱ��������
	endif
	
	strvar LV ret2
	GET_PLAYER_BASE_ATT 1 1 LV 1
	CLAN_IS_OPEN_DART 1 ret2
	; get_now_time 0 0 0 0 time 0 0

	task_add_option 1096 1 120
	task_add_option 1095 1 121

	;(�¼����ˡ�����֪���٣������񡪡�hnz��
	task_add_option 1097 1 799
	
	;(�¼ӹ�������ѡ���hnz��
	if @LV >= 55
		option 150 #���������ˡ��
	endif
	talk
endproc


; PROC 101
	; OPEN_NPC_SHOP
; ENDPROC
;;;;;����

;(�¼����ˡ�����֪���٣������񡪡�hnz��
proc 799
	task_do_option 1097 1
endproc

proc 120
	task_do_option 1096 1
endproc
proc 121
	task_do_option 1095 1
endproc

proc 200
	dialog 3
	add_talk $_npc$:
	add_talk #b#b��������ĸ����з���#c186506��˼��#n��#c186506������#n
	add_talk #b#b�����û�����ӵĻ�����ȥ�����Ƕ������ɣ�
	
	NPC_COMMIT_OPT 201
	
	talk
endproc 
proc 201
	strvar m_ID m_Pos1 m_Pos2 m_Col Ret RetTask
	GET_COMMIT_ITEM_INFO 1 m_ID
	if m_ID = -1
		add_sys_msg_start
		add_sys_msg �㻹û���ύ��Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info		
		return 
	endif
	
	GET_COMMIT_ITEM_INFO 2 m_Pos1
	GET_COMMIT_ITEM_INFO 3 m_Pos2
	GET_COMMIT_ITEM_INFO 4 m_Col
	task_stat 4497 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4497 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4497 1
					call 502
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg ��Ʒ��������Ҫ��δ�ܳɹ��Ͻ�
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	
	endif
endproc 
proc 502
	add_sys_msg_start
	add_sys_msg �ɹ��Ͻ���Ʒ���������
	set_sys_msg_mode 3 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc 




proc 300

	strvar retin retcar
	in_dart_car retin retcar 0
	if retin = 0 and retcar = 1
	
		strvar num
		GET_ITEM_NUM_INPAK 5300945 1 num
		if num > 0
			strvar ret
			dec_item_num_inpak 5300945 1 ret
			if ret = 0
				TASK_FINI 1096 1
			endif
			
		else
			add_sys_msg_start
			add_sys_msg ������û�������Ʒ��
			set_sys_msg_mode 4 0 0
			msg_info
		
		endif

	else
		add_sys_msg_start
		add_sys_msg �ύ��������ʱ��������Ҫ�����ڻ�����
		set_sys_msg_mode 4 0 0
		msg_info
		
	endif
endproc

proc 400
	new_dart_car 1 600
endproc

proc 401
		
	dialog 1
	;add_talk $_npc$:
	add_talk ����˵��
	add_talk #b#b�� ���������������
	add_talk #b#b�� ��������Խ�ߣ��������յĽ���Խ��
	add_talk #b#b�� #c005ebe����#n\#c6d00d2����#n\#cff4e00�ƽ�#n�������⽱�����������ġ�
	add_talk #b#b�� ���������ġ������ڲ��롰����Ԫ�ء��
	add_talk #b#b�� �۷�·���̻Ὣֹͣ��������
	add_talk #b#b�� ������Ҫ֧��1�ݡ�������ͬ��
	add_talk #b
	;add_talk ��������
	add_talk ��ͨ�����������վ��齱��500%
    add_talk #c0a6400С�ʻ���#n�������վ��齱��600%
    add_talk #c005ebe���ػ���#n�������վ��齱��750%����������*1
    add_talk #c6d00d2���̻���#n�������վ��齱��1500%����������*2
    add_talk #cff4e00�ƽ����#n�������վ��齱��3000%����������*4
	add_talk #b
	
	strvar car
	get_dart_car_type 1 car
	if car = 0
	add_talk ��ǰ��������ǰ���κλ���
	endif
	if car = 1 	
	add_talk ��ǰ��������ͨ����
	endif
	if car = 2
	add_talk ��ǰ������#c0a6400С�ʻ���#n
	endif
	if car = 3
	add_talk ��ǰ������#c005ebe���ػ���#n
	endif
	if car = 4
	add_talk ��ǰ������#c6d00d2���̻���#n
	endif
	if car = 5
	add_talk ��ǰ������#cff4e00�ƽ����#n
	endif
	if car = 6
	add_talk ��ǰ�������ƾɻ���
	endif	
	
	option 402 ����������͵Ļ���
	
	talk

endproc
proc 402
	task_stat 1096 1 ret
	if ret = 0
		add_sys_msg_start
		add_sys_msg δ��ȡ���������޷���������
		set_sys_msg_mode 3 0 0
		msg_info
		return
	ENDIF
	
	GET_ITEM_NUM_INPAK 5060023 100 101
	if 101 < 1
		BUY_HETONG 5060023
		return
	ENDIF
	
	strvar car
	get_dart_car_type 1 car
	if car = 5
		choose 2 403 403 ��ǰ������ߵ��εġ��ƽ������
	else
		call 404
	endif
	
endproc 
proc 403
	
endproc 
proc 404
	strvar mid
	reset_dart_car mid
	GET_STAR_INFO 4 20
	
	target_add_status 3 @mid 3203 1 3600 1
	if 20 = 1
		strvar car
		get_dart_car_type 1 car
		if car = 1 or car = 2 or car = 3
		
			target_add_status 3 @mid 3202 2 3600 1
		endif
		if car = 4 or car = 5
			target_add_status 3 @mid 3202 3 3600 1
		endif
	endif
	call 401
endproc

proc 450
	dialog 1
	;add_talk $_npc$:
	add_talk �⳵˵��
	;add_talk #b#b�� ��������ʱ������󣬿��������޻�����
	add_talk #b#b�� �����𻵳��ƾɻ����󣬿��������޻�����
	add_talk #b#b�� �������޵Ļ���Ϊԭ�е��Ρ�
	;add_talk #b#b�� ���޵Ļ������ٽ�������䡰�������ġ���
	;add_talk #b#b�� �������޻���ʱ���̻��Զ������ƾɻ�����	
	add_talk #b#b�� ������Ҫ֧����Ӧ�����ġ�������ͬ����
	add_talk #b#b
	add_talk ��ͨ��������������ͬ*1
	add_talk #c0a6400С�ʻ���#n����������ͬ*1
	add_talk #c005ebe���ػ���#n����������ͬ*2
	add_talk #c6d00d2���̻���#n����������ͬ*4
	add_talk #cff4e00�ƽ����#n����������ͬ*6
	add_talk #b#b
	
	GET_PRE_DART_CAR 10 20
	GET_ITEM_NUM_INPAK 5060023 1 40
	if 10 = 0 
		if 20 = 1
			add_talk ԭ�е��Σ���ͨ����
			if 40 = 0
				add_talk ��ǰЯ����������ͬ* #cF80B0B $@40$#n
			else
				add_talk ��ǰЯ����������ͬ* #c0a6400 $@40$#n
			endif
		endif
		if 20 = 2
			add_talk ԭ�е��Σ�#c0a6400С�ʻ���#n
			if 40 = 0
				add_talk ��ǰЯ����������ͬ* #cF80B0B $@40$#n
			else
				add_talk ��ǰЯ����������ͬ* #c0a6400 $@40$#n
			endif
		endif
		if 20 = 3
			add_talk ԭ�е��Σ�#c005ebe���ػ���#n
			if 40 < 2
				add_talk ��ǰЯ����������ͬ* #cF80B0B $@40$#n
			else
				add_talk ��ǰЯ����������ͬ* #c0a6400 $@40$#n
			endif
		endif
		if 20 = 4
			add_talk ԭ�е��Σ�#c6d00d2���̻���#n
			if 40 < 4
				add_talk ��ǰЯ����������ͬ* #cF80B0B $@40$#n
			else
				add_talk ��ǰЯ����������ͬ* #c0a6400 $@40$#n
			endif
		endif
		if 20 = 5
			add_talk ԭ�е��Σ�#cff4e00�ƽ����#n
			if 40 < 6
				add_talk ��ǰЯ����������ͬ* #cF80B0B $@40$#n
			else
				add_talk ��ǰЯ����������ͬ* #c0a6400 $@40$#n
			endif
		endif
	endif

	add_talk #b#b
	add_talk #c0a6400��ݷ���
	add_talk �������޻���ʱ���ƾɻ������迪����ƾ����
	
	option 451 #�����������޻���
	
	talk
	
endproc
proc 451
	GET_PRE_DART_CAR 10 20
	if 10 = 0 
		if 20 = 1
			RESUME_DART_CAR 10 1
			target_add_status 3 @10 3203 1 3600 1
		endif
		if 20 = 2
			RESUME_DART_CAR 10 1
			target_add_status 3 @10 3203 1 3600 1
		endif
		if 20 = 3
			RESUME_DART_CAR 10 2
			target_add_status 3 @10 3203 1 3600 1
		endif
		if 20 = 4
			RESUME_DART_CAR 10 4
			target_add_status 3 @10 3203 1 3600 1
		endif
		if 20 = 5
			RESUME_DART_CAR 10 6
			target_add_status 3 @10 3203 1 3600 1
		endif
	endif

endproc




proc 2
	get_trig_task 1 2
	if 2 = 1096
		
		RIDE_OFF 1
		strvar mid
		new_dart_car mid 3600
		
		target_add_status 3 @mid 3203 1 3600 1
		
		GET_STAR_INFO 4 20
		if 20 = 1
			strvar car
			get_dart_car_type 1 car
			if car = 1 or car = 2 or car = 3
				target_add_status 3 @mid 3202 2 3600 1
			
			endif
			if car = 4 or car = 5
				target_add_status 3 @mid 3202 3 3600 1
			endif
		endif
		dialog 1
		add_talk $_npc$:
		add_talk #b#b���ǻ������̻���Ҫ��Ӫó������ҵ����Ա�鲼��������䡣
		add_talk #b#b��ӵ�зḻ�����Դ�����ջ�Ұһ������Ϊ�����ص������·�ߣ�ֻҪ�ͻ�֧���㹻���ã����Ǿͱ�֤���ʰ�ȫ�ִ
		add_talk #b#b#�������׬ȡ���ı��꣬���������Э���������䣬�����������ڼ�ȱ���֡�
		add_talk #b
		add_talk #b#b#c186506����[55��]-����ﵽ55�����ǰ����ȡ����������ɺ󽫻�÷��ľ��齱��
		
		; OPTION 101 #(����)�����̵�
		
		strvar ret num
		reset_dart_car_check ret
		GET_ITEM_NUM_INPAK 5300944 1 num
		if ret = 0 and num > 0 
			option 401 #��������
		endif
		
		strvar ret
		GET_ITEM_NUM_INPAK 5300945 1 num
		task_stat 1096 1 ret
		if ret = 2 and num > 0
			option 300 #�ύ��������
		endif	
	
		talk
	endif
	
	;(�¼����ˡ�����֪���٣������񡪡�hnz��
	if 2 = 1097
		dialog 1
		add_talk $_npc$:
		add_talk #b#b���ǻ������̻���Ҫ��Ӫó������ҵ��ӵ���ŷḻ�����Դ�����ջ�Ұһ������Ȼ��Ϊ����������ص�·�ߡ������Ҳ��׬ȡ���ı��꣬���������Э�����䣬�������ڼ�ȱ�����С�
		add_talk #b
		add_talk #b#b#c186506����[55��]-����ﵽ55�����ǰ����ȡ����������ɺ󽫻�÷��ľ��齱��

		add_talk #b#b#c0181f9�û������ϵġ�F12��������������Χ����ң�ʹ������ҵ��Լ��Ļ���#n
		add_talk #b#b#c186506<ӵ�����ǻ�����Ȩ����������20%�ƶ��ٶ�>
		strvar Ret num1
		GET_ITEM_NUM_INPAK 5300782 1 num1
		if num1 > 0
			TASK_STAT 3304 1 Ret
			if Ret = 2
				option 600 �����뻻����
			endif
		endif
	
	
		; OPTION 101 #(����)�����̵�
	
		strvar ret num
		reset_dart_car_check ret
		GET_ITEM_NUM_INPAK 5300944 1 num
	
		if ret = 0 and num > 0 
			option 401 #��������
		endif
		strvar ret
		GET_ITEM_NUM_INPAK 5300945 1 num
		task_stat 1096 1 ret
		if ret = 2 and num > 0
			option 300 #�ύ��������
		endif	
		;����
		task_stat 1097 1 10
		if 10 = 2
			option 701 #����׼����������ʱ��������
		endif
		task_add_option 1096 1 120
		task_add_option 1095 1 121
		task_add_option 1097 1 801
		talk
		;if 2 = 30002
		;	task_fini 30002 1
		;endif
	endif

endproc 




proc 600
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��û�뵽�Ȿ��Ѱ����ʮ��ġ���ʯ������Ȼ��������У����һ��ɣ�����Ҫʲô��������
	add_talk #b#b<������˼��һ��>
	add_talk #b#b�ðɣ��ɽ���
	option 601 #����
		
	talk
endproc

proc 601
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300782 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300782 1 1
			ADD_ITEM_NUM_INPAK 5300785 1 0 0
			
		else
			add_sys_msg_start
			add_sys_msg �����ռ䲻�㣡
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg ������û�п��Խ�������Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info
	endif
endproc 

proc 701
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���˵Ļ����Ƿ�֡���ͨ��С�ʡ����ء����̡��ƽ�5�൵�Σ�
	option 801 #��
	option 901 #��
	talk
endproc

proc 702
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��������Խ�ߣ����˵����ս����Ƿ�Խ���
	option 802 #��
	option 902 #��
	talk
endproc

proc 703
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������Ƿ��޷��ҡ������������������ı�����ĵ��Σ�
	option 903 #��
	option 803 #��
	talk
endproc

proc 704
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���˷���;�У��Ƿ����ҡ���������������������
	option 904 #��
	option 804 #��
	talk
endproc

proc 705
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�����Ƿ�涨�˻���������ʱ�䣿
	option 805 #��
	option 905 #��
	talk
endproc

proc 706
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����������ʱ����δ������ˣ������Ƿ��ʧ�ܣ�
	option 806 #��
	option 906 #��
	talk
endproc

proc 707
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�����Ƿ����Ե������Ի�е���ڵ��Ĺ�����
	option 907 #��
	option 807 #��
	talk
endproc

proc 708
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������ݻٺ��Ƿ��޷��ҡ���������������������
	option 808 #��
	option 908 #��
	talk
endproc

proc 709
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������ݻٺ��Ƿ��޷��ҡ��������������޻�����
	option 909 #��
	option 809 #��
	talk
endproc

proc 710
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������޵Ļ����Ƿ�Ϊԭ�л����ĵ��Σ�
	option 810 #��
	option 910 #��
	talk
endproc

proc 801
	call 702
endproc

proc 802
	call 703
endproc

proc 803
	call 704
endproc

proc 804
	call 705
endproc

proc 805
	call 706
endproc

proc 806
	call 707
endproc

proc 807
	call 708
endproc

proc 808
	call 709
endproc

proc 809
	call 710
endproc

proc 810
	task_fini 1097 10
	call 1
endproc

proc 901
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���˵Ļ����Ƿ�֡���ͨ��С�ʡ����ء����̡��ƽ�5�൵�Σ�

	add_talk #b#b#c186506��ȷ������˵Ļ����ӵ͵��߷�Ϊ����ͨ��С�ʡ����ء����̡��ƽ�5�൵�Ρ�#n
	option 801 #��
	option 901 #��
	talk
endproc

proc 902
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��������Խ�ߣ����˵����ս����Ƿ�Խ���

	add_talk #b#b#c186506��ȷ��𣺻����ĵ���Խ�ߣ����˿ɻ��Խ�������ս�����#n
	option 802 #��
	option 902 #��
	talk
endproc

proc 903
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������Ƿ��޷��ҡ������������������ı�����ĵ��Σ�

	add_talk #b#b#c186506��ȷ������˿��ҡ������������������ı�����ĵ��Ρ�#n
	option 903 #��
	option 803 #��
	talk
endproc

proc 904
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���˷���;�У��Ƿ����ҡ���������������������

	add_talk #b#b#c186506��ȷ������˷���;�У��޷��ҡ���������������������#n
	option 904 #��
	option 804 #��
	talk
endproc

proc 905
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�����Ƿ�涨�˻���������ʱ�䣿

	add_talk #b#b#c186506��ȷ����̻��ϸ�涨�����˻���������ʱ�䡣#n
	option 805 #��
	option 905 #��
	talk
endproc

proc 906
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����������ʱ����δ������ˣ������Ƿ��ʧ�ܣ�

	add_talk #b#b#c186506��ȷ�������ʱ����δ������ˣ��̻Ὣ��Ϊ����ʧ�ܣ������ջ�����#n
	option 806 #��
	option 906 #��
	talk
endproc

proc 907
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�����Ƿ����Ե������Ի�е���ڵ��Ĺ�����

	add_talk #b#b#c186506��ȷ��𣺻�����̵���ǽ���Ч�������Ի�е���ڵ��Ĺ�����#n
	option 907 #��
	option 807 #��
	talk
endproc

proc 908
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������ݻٺ��Ƿ��޷��ҡ���������������������

	add_talk #b#b#c186506��ȷ��𣺻������ݻٺ󣬡����������ṩ���������ķ���#n
	option 808 #��
	option 908 #��
	talk
endproc

proc 909
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������ݻٺ��Ƿ��޷��ҡ��������������޻�����

	add_talk #b#b#c186506��ȷ��𣺻������ݻٺ󣬡����������ṩ���޻����ķ���#n
	option 909 #��
	option 809 #��
	talk
endproc

proc 910
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������޵Ļ����Ƿ�Ϊԭ�л����ĵ��Σ�

	add_talk #b#b#c186506��ȷ����������޻�����ֱ������ԭ�е��εĻ�����#n
	option 810 #��
	option 910 #��
	talk
endproc

;(�¼ӹ������˶Ի���塪��hnz��
proc 150
	strvar ret pos time
	get_now_time 1 1 1 1 time 1 1
	
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bΪ���������ó�׵�Ч�ʣ��������̻���ʱ������ǩ���ˡ�Э�˺�Լ���������ɹ�����֯�����˶��ɻ�ö��ⱨ�ꡣ
	add_talk #b#b
	add_talk #c005ebe���������ˡ���ڼ��ȡ�ġ����ͻ�����������ɺ󽫶��⽱����������ֵ#n
	add_talk #b#b
	add_talk #c186506�ʱ�䣺ÿ��1��(06:00-24:00)
	add_talk �᳤������06:00-22:00
	add_talk ��Ա������22:00-24:00��ǰ�᣺�᳤����δ������
	add_talk ����ʱ�䣺�����20����#n
	
	if time >= 6 and time < 24
		option 151 #�����ټ���Ա���������������ˡ��
	endif
	
	talk
endproc

;(�¼ӹ������˽ӿڡ���hnz��
proc 151
	strvar time ret pos art tab
	get_now_time 1 1 1 1 time 1 1
	PLAYER_CLAN_POS_INFO ret 0 0 0 pos
	CLAN_IS_OPEN_DART tab art
	
	if tab = 0 and art = 1
		add_sys_msg_start
		add_sys_msg ���������ˡ��ѿ������ѽ����������޷��ٿ����
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time >= 6 and time < 22
		if ret = 0 and pos > 0
			if ret = 0 and ( pos = 7 or pos = 8 )
				choose 0 152 153 �Ƿ�ȷ�Ͽ������������ˡ����
			else
				add_sys_msg_start
				add_sys_msg 22:00��᳤��δ��������Ա�����п������������ˡ��
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg �ǹ����Ա�޷��������������ˡ��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	elseif time >= 22 and time < 24
		if ret = 0 and pos > 0
			choose 0 152 153 �Ƿ�ȷ�Ͽ������������ˡ����
		else
			add_sys_msg_start
			add_sys_msg �ǹ����Ա�޷��������������ˡ��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc
proc 152
	strvar time ret pos art tab
	get_now_time 1 1 1 1 time 1 1
	PLAYER_CLAN_POS_INFO ret 0 0 0 pos
	CLAN_IS_OPEN_DART tab art
	
	if tab = 0 and art = 1
		add_sys_msg_start
		add_sys_msg ���������ˡ��ѿ������ѽ����������޷��ٿ����
		set_sys_msg_mode 4 0 0
		msg_info
	elseif time >= 6 and time < 22
		if ret = 0 and pos > 0
			if ret = 0 and ( pos = 7 or pos = 8 )
				CLAN_OPEN_DART 0
			else
				add_sys_msg_start
				add_sys_msg 22:00��᳤��δ��������Ա�����п������������ˡ��
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg �ǹ����Ա�޷��������������ˡ��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	elseif time >= 22 and time < 24
		if ret = 0 and pos > 0
			CLAN_OPEN_DART 0
		else
			add_sys_msg_start
			add_sys_msg �ǹ����Ա�޷��������������ˡ��
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	else
	endif
endproc












