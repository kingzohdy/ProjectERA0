
proc 1

	strvar Tim 
	var Tim 0
	set_event_var 0 32 @Tim 1
	
	strvar Maxlv
	var Maxlv 0
	
	strvar Rflag
	var Rflag 0
	
	GET_CUR_MAX_LEVEL Rflag Maxlv
	set_event_var 8 32 @Maxlv 1
	set_event_var 16 32 @Rflag 1
	
	if @Rflag = 0 and Maxlv >= 30 and Maxlv <= 49 
		add_sys_msg_start
		add_sys_msg ħӰ����: ����ɳĮ,���ջ�Ұ��������ѱ�ĳ���ħ��,��20:00֮ǰ,�ҵ�ħ��������ɱ,������ϡ�еı���!
		set_sys_msg_mode 6 0 0
		SVR_MSG_INFO
	elseif @Rflag = 0 and Maxlv >= 50 and Maxlv <= 59
		add_sys_msg_start
		add_sys_msg ħӰ����: ����ɳĮ,���ջ�Ұ,Խ�����ֳ�������ѱ�ĳ���ħ��,��20:00֮ǰ,�ҵ�ħ��������ɱ,������ϡ�еı���!
		set_sys_msg_mode 6 0 0
		SVR_MSG_INFO
	elseif @Rflag = 0 and Maxlv >= 60 and Maxlv <= 69
		add_sys_msg_start
		add_sys_msg ħӰ����: ����ɳĮ,���ջ�Ұ,Խ������,���ֵس�������ѱ�ĳ���ħ��,��20:00֮ǰ,�ҵ�ħ��������ɱ,������ϡ�еı���!
		set_sys_msg_mode 6 0 0
		SVR_MSG_INFO
	elseif @Rflag = 0 and Maxlv >= 70 and Maxlv <= 79
		add_sys_msg_start
		add_sys_msg ħӰ����: ����ɳĮ,���ջ�Ұ,Խ������,���ֵ�,����ѹȳ�������ѱ�ĳ���ħ��,��20:00֮ǰ,�ҵ�ħ��������ɱ,������ϡ�еı���!
		set_sys_msg_mode 6 0 0
		SVR_MSG_INFO
	else
		add_sys_msg_start
		add_sys_msg ħӰ����: ����ɳĮ,���ջ�Ұ,Խ������,���ֵ�,����ѹ�, ��ɳ�ݵس�������ѱ�ĳ���ħ��,��20:00֮ǰ,�ҵ�ħ��������ɱ,������ϡ�еı���!
		set_sys_msg_mode 6 0 0
		SVR_MSG_INFO
	endif
	
	if @Rflag = 0 and Maxlv >= 30
		call 100
	endif
	if @Rflag = 0 and Maxlv >= 40
		call 200
	endif
	if @Rflag = 0 and Maxlv >= 50
		call 210
	endif
	if @Rflag = 0 and Maxlv >= 60
		call 220
	endif
	if @Rflag = 0 and Maxlv >= 70	
		call 230
	endif
	if @Rflag = 0 and Maxlv >= 80
		call 240
	endif
	
endproc  

proc 2
	add_sys_msg_start
	add_sys_msg ħӰ���ػ�Ѿ�����,δ����ɱ��ħ���Ѿ����¶�����Ӱ֮�С�
	set_sys_msg_mode 6 0 0
	SVR_MSG_INFO
	
	GET_MAP_INST 2300 1
	DESTROY_MON_BY_DEFID 1 206900 
	DESTROY_MON_BY_DEFID 1 206901 
	
	GET_MAP_INST 2400 1
	DESTROY_MON_BY_DEFID 1 206910
	DESTROY_MON_BY_DEFID 1 206911 
	
	GET_MAP_INST 2500 1
	DESTROY_MON_BY_DEFID 1 206921
	DESTROY_MON_BY_DEFID 1 206920 
	
	GET_MAP_INST 2104 1
	DESTROY_MON_BY_DEFID 1 206931
	DESTROY_MON_BY_DEFID 1 206930 
	
	GET_MAP_INST 2201 1
	DESTROY_MON_BY_DEFID 1 206941
	DESTROY_MON_BY_DEFID 1 206940 
	
	GET_MAP_INST 3404 1
	DESTROY_MON_BY_DEFID 1 206951
	DESTROY_MON_BY_DEFID 1 206950 
endproc 

proc 3
	
	get_event_var 0 32 1 Tim
	add Tim 1
	set_event_var 0 32 @Tim 1
	if Tim = 60
		get_event_var 8 32 1 Maxlv
		get_event_var 16 32 1 Rflag
		
		if @Rflag = 0 and Maxlv >= 30 and Maxlv <= 49 
			add_sys_msg_start
			add_sys_msg ħӰ����: ����ɳĮ,���ջ�Ұ��������ѱ�ĳ���ħ��,��20:00֮ǰ,�ҵ�ħ��������ɱ,������ϡ�еı���!
			set_sys_msg_mode 6 0 0
			SVR_MSG_INFO
		elseif @Rflag = 0 and Maxlv >= 50 and Maxlv <= 59
			add_sys_msg_start
			add_sys_msg ħӰ����: ����ɳĮ,���ջ�Ұ,Խ�����ֳ�������ѱ�ĳ���ħ��,��20:00֮ǰ,�ҵ�ħ��������ɱ,������ϡ�еı���!
			set_sys_msg_mode 6 0 0
			SVR_MSG_INFO
		elseif @Rflag = 0 and Maxlv >= 60 and Maxlv <= 69
			add_sys_msg_start
			add_sys_msg ħӰ����: ����ɳĮ,���ջ�Ұ,Խ������,���ֵس�������ѱ�ĳ���ħ��,��20:00֮ǰ,�ҵ�ħ��������ɱ,������ϡ�еı���!
			set_sys_msg_mode 6 0 0
			SVR_MSG_INFO
		elseif @Rflag = 0 and Maxlv >= 70 and Maxlv <= 79
			add_sys_msg_start
			add_sys_msg ħӰ����: ����ɳĮ,���ջ�Ұ,Խ������,���ֵ�,����ѹȳ�������ѱ�ĳ���ħ��,��20:00֮ǰ,�ҵ�ħ��������ɱ,������ϡ�еı���!
			set_sys_msg_mode 6 0 0
			SVR_MSG_INFO
		else
			add_sys_msg_start
			add_sys_msg ħӰ����: ����ɳĮ,���ջ�Ұ,Խ������,���ֵ�,����ѹ�, ��ɳ�ݵس�������ѱ�ĳ���ħ��,��20:00֮ǰ,�ҵ�ħ��������ɱ,������ϡ�еı���!
			set_sys_msg_mode 6 0 0
			SVR_MSG_INFO
		endif
		
		var Tim 0
		set_event_var 0 32 @Tim 1
		
	endif
	
endproc  


proc 300
	NEW_MAP_MON @monid 1 @map @x @y 0 0 0 0 0 10 30
	TARGET_ADD_STATUS 3 @30 1095 1 25920000 0
	;��
;		add_sys_msg_start
;		add_sys_msg ��=$@10$ 
;		set_sys_msg_mode 1 0 0
;		SVR_MSG_INFO
endproc 

proc 333
	;��
	NEW_MAP_MON @monid 1 @map @x @y 0 0 0 0 0 20 30
	TARGET_ADD_STATUS 3 @30 1095 1 25920000 0
	
;		add_sys_msg_start
;		add_sys_msg ��=$@20$ 
;		set_sys_msg_mode 1 0 0
;		SVR_MSG_INFO
endproc 

proc 100
;����ɳĮ����
	strvar x y monid map sol
	strvar rad
	;����ɳĮ����
	strvar lin
	;ֻ���1��
	var sol 0
	var rad 0
	var lin 0
	
	rand rad 10
	while @sol < 10
	add sol 1
	;ѭ��10��
	
		if lin = 1 or lin = 2
			add rad 1
			if rad = 10
				var rad 0
			endif
			var lin 2
		endif
		
		if rad = 0
			set_call_var x 19568	
			set_call_var y 21905
		endif
		if rad = 1
			set_call_var x 5098	
			set_call_var y 22102
		endif
		if rad = 2
			set_call_var x 12158
			set_call_var y 21592
		endif
		if rad = 3
			set_call_var x 2184
			set_call_var y 18131
		endif
		if rad = 4
			set_call_var x 3217
			set_call_var y 13900
		endif
		if rad = 5
			set_call_var x 9855
			set_call_var y 6549
		endif
		if rad = 6
			set_call_var x 20684
			set_call_var y 2743
		endif
		if rad = 7
			set_call_var x 13435
			set_call_var y 3335
		endif
		if rad = 8
			set_call_var x 4656
			set_call_var y 3068
		endif
		if rad = 9
			set_call_var x 21137
			set_call_var y 13859
		endif
		;ֻ��һ��
		if lin != 2
			var lin 1
		endif
			
		;ˢ��1ֻ���	
		if lin = 1
			set_call_var monid 206900
			set_call_var map 2300
			call 300	
		endif
		if lin = 2
		;ˢ9ֻ�ٵ�
			set_call_var monid 206901
			set_call_var map 2300
			call 333	
		endif
	
;		GET_MAP_MON_NUM 2300 206900 1 30 1
;		GET_MAP_MON_NUM 2300 206901 1 31 1
;		add_sys_msg_start
;		add_sys_msg ������$@30$ֻ  ������$@31$ֻ
;		set_sys_msg_mode 1 0 0
;		SVR_MSG_INFO
	endwhile
endproc 

proc 200
;���ջ�Ұ
	strvar x y monid map sol
	strvar rad
	strvar lin
	;ֻ���1��
	var sol 0
	var rad 0
	var lin 0
	
	rand rad 10
	while @sol < 10
	add sol 1
	;ѭ��10��
	
		if lin = 1 or lin = 2
			add rad 1
			if rad = 10
				var rad 0
			endif
			var lin 2
		endif
		
		if rad = 0
			set_call_var x 4667
			set_call_var y 21881
		endif
		if rad = 1
			set_call_var x 22219
			set_call_var y 21482
		endif
		if rad = 2
			set_call_var x 12232
			set_call_var y 22000
		endif
		if rad = 3
			set_call_var x 15334
			set_call_var y 6762
		endif
		if rad = 4
			set_call_var x 19707
			set_call_var y 3846
		endif
		if rad = 5
			set_call_var x 22047
			set_call_var y 12791
		endif
		if rad = 6
			set_call_var x 10733
			set_call_var y 8617
		endif
		if rad = 7
			set_call_var x 11758
			set_call_var y 15231
		endif
		if rad = 8
			set_call_var x 6293
			set_call_var y 2577
		endif
		if rad = 9
			set_call_var x 2655
			set_call_var y 5862
		endif
		;ֻ��һ��
		if lin != 2
			var lin 1
		endif
			
		;ˢ��1ֻ���	
		if lin = 1
			set_call_var monid 206910
			set_call_var map 2400
			call 300	
		endif
		if lin = 2
		;ˢ9ֻ�ٵ�
			set_call_var monid 206911
			set_call_var map 2400
			call 333	
		endif
	endwhile
endproc



proc 210
;Խ������
	strvar x y monid map sol
	strvar rad
	strvar lin
	;ֻ���1��
	var sol 0
	var rad 0
	var lin 0
	
	rand rad 10
	while @sol < 10
	add sol 1
	;ѭ��10��
	
		if lin = 1 or lin = 2
			add rad 1
			if rad = 10
				var rad 0
			endif
			var lin 2
		endif
		
		if rad = 0
			set_call_var x 9704
			set_call_var y 13482
		endif
		if rad = 1
			set_call_var x 10733
			set_call_var y 18038
		endif
		if rad = 2
			set_call_var x 15524
			set_call_var y 18110
		endif
		if rad = 3
			set_call_var x 23583
			set_call_var y 14325
		endif
		if rad = 4
			set_call_var x 31033
			set_call_var y 27672
		endif
		if rad = 5
			set_call_var x 23751
			set_call_var y 32041
		endif
		if rad = 6
			set_call_var x 6362
			set_call_var y 28951
		endif
		if rad = 7
			set_call_var x 7311
			set_call_var y 23359
		endif
		if rad = 8
			set_call_var x 20327
			set_call_var y 33212
		endif
		if rad = 9
			set_call_var x 18618
			set_call_var y 23250
		endif
		;ֻ��һ��
		if lin != 2
			var lin 1
		endif
			
		;ˢ��1ֻ���	
		if lin = 1
			set_call_var monid 206920
			set_call_var map 2500
			call 300	
		endif
		if lin = 2
		;ˢ9ֻ�ٵ�
			set_call_var monid 206921
			set_call_var map 2500
			call 333	
		endif
	endwhile
endproc



proc 220
;�����ֵ�
	strvar x y monid map sol
	strvar rad
	strvar lin
	;ֻ���1��
	var sol 0
	var rad 0
	var lin 0
	
	rand rad 10
	while @sol < 10
	add sol 1
	;ѭ��10��
	
		if lin = 1 or lin = 2
			add rad 1
			if rad = 10
				var rad 0
			endif
			var lin 2
		endif
		
		if rad = 0
			set_call_var x 22160
			set_call_var y 3159
		endif
		if rad = 1
			set_call_var x 2942
			set_call_var y 2385
		endif
		if rad = 2
			set_call_var x 3910
			set_call_var y 21414
		endif
		if rad = 3
			set_call_var x 9285
			set_call_var y 22974
		endif
		if rad = 4
			set_call_var x 22042
			set_call_var y 19438
		endif
		if rad = 5
			set_call_var x 12371
			set_call_var y 14271
		endif
		if rad = 6
			set_call_var x 10152
			set_call_var y 2162
		endif
		if rad = 7
			set_call_var x 15180
			set_call_var y 7482
		endif
		if rad = 8
			set_call_var x 2545
			set_call_var y 14756
		endif
		if rad = 9
			set_call_var x 18609
			set_call_var y 16498
		endif
		;ֻ��һ��
		if lin != 2
			var lin 1
		endif
			
		;ˢ��1ֻ���	
		if lin = 1
			set_call_var monid 206930
			set_call_var map 2104
			call 300	
		endif
		if lin = 2
		;ˢ9ֻ�ٵ�
			set_call_var monid 206931
			set_call_var map 2104
			call 333	
		endif
	endwhile
endproc

proc 230
;����ѹ�
	strvar x y monid map sol
	strvar rad
	strvar lin
	;ֻ���1��
	var sol 0
	var rad 0
	var lin 0
	
	rand rad 10
	while @sol < 10
	add sol 1
	;ѭ��10��
	
		if lin = 1 or lin = 2
			add rad 1
			if rad = 10
				var rad 0
			endif
			var lin 2
		endif
		
		if rad = 0
			set_call_var x 14363
			set_call_var y 4625
		endif
		if rad = 1
			set_call_var x 29908
			set_call_var y 3248
		endif
		if rad = 2
			set_call_var x 27433
			set_call_var y 14529
		endif
		if rad = 3
			set_call_var x 35202
			set_call_var y 22919
		endif
		if rad = 4
			set_call_var x 34695
			set_call_var y 33624
		endif
		if rad = 5
			set_call_var x 18628
			set_call_var y 32660
		endif
		if rad = 6
			set_call_var x 10319
			set_call_var y 33437
		endif
		if rad = 7
			set_call_var x 3018
			set_call_var y 27296
		endif
		if rad = 8
			set_call_var x 3598
			set_call_var y 12445
		endif
		if rad = 9
			set_call_var x 16098
			set_call_var y 20617
		endif
		;ֻ��һ��
		if lin != 2
			var lin 1
		endif
			
		;ˢ��1ֻ���	
		if lin = 1
			set_call_var monid 206940
			set_call_var map 2201
			call 300	
		endif
		if lin = 2
		;ˢ9ֻ�ٵ�
			set_call_var monid 206941
			set_call_var map 2201
			call 333	
		endif
	endwhile
endproc



proc 240
;��ɳ�ݵ�
	strvar x y monid map sol
	strvar rad
	strvar lin
	;ֻ���1��
	var sol 0
	var rad 0
	var lin 0
	
	rand rad 10
	while @sol < 10
	add sol 1
	;ѭ��10��
	
		if lin = 1 or lin = 2
			add rad 1
			if rad = 10
				var rad 0
			endif
			var lin 2
		endif
		
		if rad = 0
			set_call_var x 12773
			set_call_var y 8320
		endif
		if rad = 1
			set_call_var x 26296
			set_call_var y 5099
		endif
		if rad = 2
			set_call_var x 32811
			set_call_var y 11824
		endif
		if rad = 3
			set_call_var x 34270
			set_call_var y 18487
		endif
		if rad = 4
			set_call_var x 26661
			set_call_var y 33352
		endif
		if rad = 5
			set_call_var x 14919
			set_call_var y 32176
		endif
		if rad = 6
			set_call_var x 26969
			set_call_var y 26198
		endif
		if rad = 7
			set_call_var x 33246
			set_call_var y 27856
		endif
		if rad = 8
			set_call_var x 21044
			set_call_var y 22954
		endif
		if rad = 9
			set_call_var x 20584
			set_call_var y 11339
		endif
		;ֻ��һ��
		if lin != 2
			var lin 1
		endif
			
		;ˢ��1ֻ���	
		if lin = 1
			set_call_var monid 206950
			set_call_var map 3404
			call 300	
		endif
		if lin = 2
		;ˢ9ֻ�ٵ�
			set_call_var monid 206951
			set_call_var map 3404
			call 333	
		endif
	endwhile
endproc






