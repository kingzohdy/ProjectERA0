proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���������ĻԻͣ�һ�빦������������ĿƼ��������ۡ����˱�������ʹ������ӵ���˸��������ȥ�������硣	
	add_talk #b#b�������Ӧ�ñ���ѧ������֮�ģ�����Ƭ���أ������޾���δ֪�������̽���뷢�֡�
;	TASKCYCLE_ADD_OPTION 102 1 120 ��ȡ��������
	add_talk #b#b	
	add_talk #b#b#c186506�Ƽ�����[30]-����ﵽ30����ɡ�רҵ֮·������󣬿��ҿƼ�������ʦѧϰ��������#n
	add_talk #b#b#c186506�����ʦ����#n#L���@@(1000,14600,36200)#n
	add_talk #b#b#c186506��ħ��ʦ����#n#L׿��@@(1000,12800,28100)#n
	add_talk #b#b#c186506�����ʦ����#n#L̩��@@(1000,13400,37600)#n
;	add_talk #b#b#c186506�����ʦ�������#n
;	add_talk #b#b#c186506��ħ��ʦ����׿��#n
;	add_talk #b#b#c186506�����ʦ����̩��#n


;	strvar num
;	;���������
;	var num 0
;   #L��ɳ�ݵ�@@(3404,15492,33303)#n
;	strvar m_Ret m_Lv
;	GET_PLAYER_SECONDARY_SKILL 0 1 m_Ret m_Lv
;	if m_Ret = 0
;		add num 1
;	endif
;	GET_PLAYER_SECONDARY_SKILL 0 2 m_Ret m_Lv
;	if m_Ret = 0
;		add num 1
;	endif
;	GET_PLAYER_SECONDARY_SKILL 0 3 m_Ret m_Lv
;	if m_Ret = 0
;		add num 1
;	endif
;	GET_PLAYER_SECONDARY_SKILL 0 4 m_Ret m_Lv
;	if m_Ret = 0
;		add num 1
;	endif
;	GET_PLAYER_SECONDARY_SKILL 0 5 m_Ret m_Lv
;	if m_Ret = 0
;		add num 1
;	endif
;	GET_PLAYER_SECONDARY_SKILL 0 6 m_Ret m_Lv
;	if m_Ret = 0
;		add num 1
;	endif
;	;�������ܳ���2��
;	if num >= 2
;		talk
;		return 
;	endif
;
;	;��������
;
;	task_stat 4020 1 10
;	if 10 = 1
;		add num 1
;	endif
;	task_stat 4022 1 11
;	if 11 = 1
;		add num 1
;	endif
;	task_stat 4024 1 12
;	if 12 = 1
;		add num 1
;	endif
;	task_stat 4021 1 13
;	if 13 = 1
;		add num 1
;	endif
;	task_stat 4023 1 14
;	if 14 = 1
;		add num 1
;	endif
;	task_stat 4025 1 15
;	if 15 = 1
;		add num 1
;	endif
;	;�������ܳ���2��
;	if num >= 2
;		talk
;		return 
;	endif

	
;	task_add_option 4021 1 102
;	task_add_option 4022 1 103
;	task_add_option 4023 1 104
;	task_add_option 4024 1 105
;	task_add_option 4025 1 106
									
	; task_add_option 4100 1 110	
	; task_add_option 4101 1 111
	; task_add_option 4102 1 112
	; task_add_option 4103 1 113
	; task_add_option 4104 1 114
	; task_add_option 4105 1 115
	; task_add_option 4106 1 116
	; task_add_option 4107 1 117
	; task_add_option 4108 1 118
	; task_add_option 4109 1 119
	; task_add_option 4110 1 120
	; task_add_option 4111 1 121
	; task_add_option 4112 1 122
	; task_add_option 4113 1 123
	; task_add_option 4114 1 124
	; task_add_option 4115 1 125
	; task_add_option 4116 1 126
	; task_add_option 4117 1 127
	; task_add_option 4118 1 128
	; task_add_option 4119 1 129
	; task_add_option 4120 1 130
	; task_add_option 4121 1 131
	; task_add_option 4122 1 132
	; task_add_option 4123 1 133
	; task_add_option 4124 1 134
	; task_add_option 4125 1 135
	; task_add_option 4126 1 136
	; task_add_option 4127 1 137
	; task_add_option 4128 1 138
	; task_add_option 4129 1 139
	; task_add_option 4130 1 140
	; task_add_option 4131 1 141
	; task_add_option 4132 1 142
	; task_add_option 4133 1 143
	; task_add_option 4134 1 144

	; task_add_option 4141 1 145
	; task_add_option 4142 1 146
	; task_add_option 4143 1 147 
	; task_add_option 4144 1 148
	; task_add_option 4145 1 149
	; task_add_option 4146 1 150

	; task_add_option 4147 1 151
	; task_add_option 4148 1 152

	; task_add_option 4149 1 153
	; task_add_option 4150 1 154
	; task_add_option 4151 1 155
	; task_add_option 4152 1 156
	; task_add_option 4153 1 157
	; task_add_option 4154 1 158
	; task_add_option 4155 1 159
	; task_add_option 4156 1 160
	; task_add_option 4157 1 161
	; task_add_option 4158 1 162
	; task_add_option 4159 1 163


	
	; TASKCYCLE_STAT 132 1 30
	; TASKCYCLE_STAT 133 1 31
	; TASKCYCLE_STAT 134 1 32
	; TASKCYCLE_STAT 135 1 33
	; TASKCYCLE_STAT 136 1 34
	;if 30 = 2 or 31 = 2 or 32 = 2 or 33 = 2 or 34 = 2
	
	task_stat 4100 1 100
	task_stat 4101 1 101
	task_stat 4102 1 102
	task_stat 4103 1 103
	task_stat 4104 1 104
	task_stat 4105 1 105
	task_stat 4106 1 106
	task_stat 4107 1 107
	task_stat 4108 1 108
	task_stat 4109 1 109
	task_stat 4110 1 110
	task_stat 4111 1 111
	task_stat 4112 1 112
	task_stat 4113 1 113
	task_stat 4114 1 114
	task_stat 4115 1 115
	task_stat 4116 1 116
	task_stat 4117 1 117
	task_stat 4118 1 118
	task_stat 4119 1 119
	task_stat 4120 1 120
	task_stat 4121 1 121
	task_stat 4122 1 122
	task_stat 4123 1 123
	task_stat 4124 1 124
	task_stat 4125 1 125
	task_stat 4126 1 126
	task_stat 4127 1 127
	task_stat 4128 1 128
	task_stat 4129 1 129
	task_stat 4130 1 130
	task_stat 4131 1 131
	task_stat 4132 1 132
	task_stat 4133 1 133
	task_stat 4134 1 134
	task_stat 4159 1 135
	
	if 100 = 2 or 101 = 2 or 102 = 2 or 103 = 2 or 104 = 2 or 105 = 2 or 106 = 2 or 107 = 2
		option 600 #�ɼ��ʸ�-�ύ��Ʒ
	endif
	if 108 = 2 or 109 = 2 or 110 = 2 or 111 = 2 or 112 = 2 or 113 = 2 or 114 = 2
		option 600 #�ɼ��ʸ�-�ύ��Ʒ
	endif
	if 115 = 2 or 116 = 2 or 117 = 2 or 118 = 2 or 119 = 2 or 120 = 2 or 121 = 2 or 122 = 2
		option 600 #�ɼ��ʸ�-�ύ��Ʒ	
	endif
	if 123 = 2 or 124 = 2 or 125 = 2 or 126 = 2 or 127 = 2 or 128 = 2 or 129 = 2 or 130 = 2
		option 600 #�ɼ��ʸ�-�ύ��Ʒ	
	endif
	if 131 = 2 or 132 = 2 or 133 = 2 or 134 = 2 or 135 = 2
		option 600 #�ɼ��ʸ�-�ύ��Ʒ	
	endif
	
	TASKCYCLE_ADD_OPTION 132 1 110
	TASKCYCLE_ADD_OPTION 133 1 111
	TASKCYCLE_ADD_OPTION 134 1 112
	TASKCYCLE_ADD_OPTION 135 1 113
	TASKCYCLE_ADD_OPTION 136 1 114
	TASKCYCLE_ADD_OPTION 141 1 115
	
	
	task_stat 4020 1 10
	if 10 = 2
		option 201 #��ʼ������ʰ�
	endif
	
	task_add_option 4020 1 101
	
;	option 600 #�Ƽ��̵�
	
	strvar Ret num1
	GET_ITEM_NUM_INPAK 5300764 1 num1
	if num1 > 0
		TASK_STAT 3304 1 Ret
		if Ret = 2
			option 500 �����뻻����
		endif
	endif
	
	option 700 �һ��������ɼ����֤��
	option 705 �һ����߼��ɼ����֤��
	
	talk
endproc

PROC 110
	TASKCYCLE_DO_OPTION 132 1
ENDPROC
PROC 111
	TASKCYCLE_DO_OPTION 133 1
ENDPROC
PROC 112
	TASKCYCLE_DO_OPTION 134 1
ENDPROC
PROC 113
	TASKCYCLE_DO_OPTION 135 1
ENDPROC
PROC 114
	TASKCYCLE_DO_OPTION 136 1
ENDPROC
PROC 115
	TASKCYCLE_DO_OPTION 141 1
ENDPROC



PROC 101	
	task_do_option 4020 1
ENDPROC
; proc 600	
	; OPEN_NPC_SHOP
; endproc 
proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b���õ������Ȼ�����漣�����������ǵĹ�����������ȴ�����ô������ҽ����ɣ��һ���ѡһЩ���������ϱȽ����ŵ��½ڸ��㡣
		option 501 #����

	talk
endproc
	
proc 501
	strvar ItmNum Num Rad
	GET_ITEM_NUM_INPAK 5300764 1 ItmNum
	if ItmNum > 0
			
		GET_PACK_SPACE 0 Num
		if Num > 0
			DEC_ITEM_NUM_INPAK 5300764 1 1
			
			rand Rad 100
			if @Rad < 80
				ADD_ITEM_NUM_INPAK 5300765 1 0 0
			endif
			if @Rad >= 80 and @Rad < 95
				ADD_ITEM_NUM_INPAK 5300766 1 0 0
			endif
			if @Rad >= 95 and @Rad < 99
				ADD_ITEM_NUM_INPAK 5300783 1 0 0
			endif
			if @Rad = 99
				ADD_ITEM_NUM_INPAK 5300784 1 0 0
			endif
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

	
; proc 110
	; task_do_option 4100 1
; endproc            
; proc 111           
	; task_do_option 4101 1
; endproc            
; proc 112           
	; task_do_option 4102 1
; endproc            
; proc 113           
	; task_do_option 4103 1
; endproc            
; proc 114           
	; task_do_option 4104 1
; endproc            
; proc 115           
	; task_do_option 4105 1
; endproc            
; proc 116           
	; task_do_option 4106 1
; endproc            
; proc 117           
	; task_do_option 4107 1
; endproc            
; proc 118           
	; task_do_option 4108 1
; endproc            
; proc 119           
	; task_do_option 4109 1
; endproc            
; proc 120           
	; task_do_option 4110 1
; endproc            
; proc 121           
	; task_do_option 4111 1
; endproc            
; proc 122           
	; task_do_option 4112 1
; endproc            
; proc 123           
	; task_do_option 4113 1
; endproc            
; proc 124           
	; task_do_option 4114 1
; endproc            
; proc 125           
	; task_do_option 4115 1
; endproc            
; proc 126           
	; task_do_option 4116 1
; endproc            
; proc 127           
	; task_do_option 4117 1
; endproc            
; proc 128           
	; task_do_option 4118 1
; endproc            
; proc 129           
	; task_do_option 4119 1
; endproc            
; proc 130           
	; task_do_option 4120 1
; endproc            
; proc 131           
	; task_do_option 4121 1
; endproc            
; proc 132           
	; task_do_option 4122 1
; endproc            
; proc 133           
	; task_do_option 4123 1
; endproc            
; proc 134           
	; task_do_option 4124 1
; endproc            
; proc 135           
	; task_do_option 4125 1
; endproc            
; proc 136           
	; task_do_option 4126 1
; endproc            
; proc 137           
	; task_do_option 4127 1
; endproc            
; proc 138           
	; task_do_option 4128 1
; endproc            
; proc 139           
	; task_do_option 4129 1
; endproc            
; proc 140           
	; task_do_option 4130 1
; endproc            
; proc 141          
	; task_do_option 4131 1
; endproc            
; proc 142           
	; task_do_option 4132 1
; endproc            
; proc 143           
	; task_do_option 4133 1
; endproc            
; proc 144           
	; task_do_option 4134 1
; endproc    
             
; proc 145       
	; task_do_option 4141 1
; endproc            
; proc 146           
	; task_do_option 4142 1
; endproc            
; proc 147           
	; task_do_option 4143 1
; endproc            
; proc 148           
	; task_do_option 4144 1
; endproc            
; proc 149           
	; task_do_option 4145 1
; endproc            
; proc 150           
	; task_do_option 4146 1
; endproc            
; proc 151           
	; task_do_option 4147 1
; endproc            
; proc 152           
	; task_do_option 4148 1
; endproc            
; proc 153           
	; task_do_option 4149 1
; endproc            
; proc 154           
	; task_do_option 4150 1
; endproc            
; proc 155           
	; task_do_option 4151 1
; endproc            
; proc 156           
	; task_do_option 4152 1
; endproc            
; proc 157           
	; task_do_option 4153 1
; endproc            
; proc 158           
	; task_do_option 4154 1
; endproc            
; proc 159           
	; task_do_option 4155 1
; endproc            
; proc 160           
	; task_do_option 4156 1
; endproc            
; proc 161           
	; task_do_option 4157 1
; endproc            
; proc 162           
	; task_do_option 4158 1
; endproc            
; proc 163           
	; task_do_option 4159 1
; endproc            

	
;PROC 102	
;	task_do_option 4021 1
;ENDPROC
;	
;PROC 103	
;	task_do_option 4022 1
;ENDPROC
;	
;PROC 104	
;	task_do_option 4023 1
;ENDPROC
;	
;PROC 105	
;	task_do_option 4024 1
;ENDPROC
;	
;PROC 106	
;	task_do_option 4025 1
;ENDPROC

proc 201
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ѧϰ�ĿƼ�����רҵ�Ƿ�ֻ�����졢��ħ������
	option 301 #��
	option 401 #��
	talk
endproc

proc 202
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bÿ�����Ƿ���ͬʱѧϰ����Ƽ�����רҵ��


	option 302 #��
	option 402 #��
	talk
endproc

proc 203
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bҪ����רҵ�ȼ��Ƿ���Ҫ�ﵽ���������ޣ�


	option 303 #��
	option 403 #��
	talk
endproc

proc 204
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ħ��������������רҵ�Ƿ���������ģ�


	option 304 #��
	option 404 #��
	talk
endproc

proc 205
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ħʦ�Ƿ���������תְҵ�ļ����飿


	option 305 #��
	option 405 #��
	talk
endproc

proc 206
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ħʦ�Ƿ���������������ָ�ͻ����ĸ�ħ���᣿


	option 406 #��
	option 306 #��
	talk
endproc

proc 207
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ʦ�Ƿ�����������������������ҩ����


	option 307 #��
	option 407 #��
	talk
endproc







proc 208
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ʦ�Ƿ�����������ʳ��Ч����ǿ�Ľ�����ҩƬ��


	option 308 #��
	option 408 #��
	talk
endproc

proc 209
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�����Ƿ�ֻ������ʦ��������


	option 309 #��
	option 409 #��
	talk
endproc

proc 210
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ʦ�Ƿ�����������ɫƷ�ʵ�װ����


	option 310 #��
	option 410 #��
	talk
endproc

proc 301
	call 202
endproc

proc 302
	call 203
endproc

proc 303
;	call 204
	call 205
endproc

proc 304
	call 205
endproc

proc 305
	call 206
endproc

proc 306
	call 207
endproc

proc 307
	call 208
endproc

proc 308
	call 209
endproc

proc 309
	call 210
endproc

proc 310
	task_fini 4020 10
	call 1
endproc

proc 401
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ѧϰ�ĿƼ�����רҵ�Ƿ�ֻ�����졢��ħ������


	add_talk #b#b#c186506�ܱ�Ǹ��Ļش���󣬱������»ش�����⡣#n
	option 301 #��
	option 401 #��
	talk
endproc

proc 402
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bÿ�����Ƿ���ͬʱѧϰ����Ƽ�����רҵ��


	add_talk #b#b#c186506�ܱ�Ǹ��Ļش���󣬱������»ش�����⡣#n
	option 302 #��
	option 402 #��
	talk
endproc

proc 403
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bҪ����רҵ�ȼ��Ƿ���Ҫ�ﵽ���������ޣ�


	add_talk #b#b#c186506�ܱ�Ǹ��Ļش���󣬱������»ش�����⡣#n
	option 303 #��
	option 403 #��
	talk
endproc

proc 404
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ħ��������������רҵ�Ƿ���������ģ�


	add_talk #b#b#c186506�ܱ�Ǹ��Ļش���󣬱������»ش�����⡣#n
	option 304 #��
	option 404 #��
	talk
endproc

proc 405
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ħʦ�Ƿ���������תְҵ�ļ����飿


	add_talk #b#b#c186506�ܱ�Ǹ��Ļش���󣬱������»ش�����⡣#n
	option 305 #��
	option 405 #��
	talk
endproc

proc 406
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ħʦ�Ƿ���������������ָ�ͻ����ĸ�ħ���᣿


	add_talk #b#b#c186506�ܱ�Ǹ��Ļش���󣬱������»ش�����⡣#n
	option 406 #��
	option 306 #��
	talk
endproc

proc 407
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ʦ�Ƿ�����������������������ҩ����


	add_talk #b#b#c186506�ܱ�Ǹ��Ļش���󣬱������»ش�����⡣#n
	option 307 #��
	option 407 #��
	talk
endproc

proc 408
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ʦ�Ƿ�����������ʳ��Ч����ǿ�Ľ�����ҩƬ��


	add_talk #b#b#c186506�ܱ�Ǹ��Ļش���󣬱������»ش�����⡣#n
	option 308 #��
	option 408 #��
	talk
endproc

proc 409
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�����Ƿ�ֻ������ʦ��������

	add_talk #b#b#c186506�ܱ�Ǹ��Ļش���󣬱������»ش�����⡣#n
	option 309 #��
	option 409 #��
	talk
endproc

proc 410
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ʦ�Ƿ�����������ɫƷ�ʵ�װ����

	add_talk #b#b#c186506�ܱ�Ǹ��Ļش���󣬱������»ش�����⡣#n
	option 310 #��
	option 410 #��
	talk
endproc

proc 600
	dialog 3
	add_talk $_npc$:
	add_talk #b#b�ռ������Ķ��������������ӡ�
	add_talk #b#b
	task_stat 4100 1 100
	task_stat 4101 1 101
	task_stat 4102 1 102
	task_stat 4103 1 103
	task_stat 4104 1 104
	task_stat 4105 1 105
	task_stat 4106 1 106
	task_stat 4107 1 107
	task_stat 4108 1 108
	task_stat 4109 1 109
	task_stat 4110 1 110
	task_stat 4111 1 111
	task_stat 4112 1 112
	task_stat 4113 1 113
	task_stat 4114 1 114
	task_stat 4115 1 115
	task_stat 4116 1 116
	task_stat 4117 1 117
	task_stat 4118 1 118
	task_stat 4119 1 119
	task_stat 4120 1 120
	task_stat 4121 1 121
	task_stat 4122 1 122
	task_stat 4123 1 123
	task_stat 4124 1 124
	task_stat 4125 1 125
	task_stat 4126 1 126
	task_stat 4127 1 127
	task_stat 4128 1 128
	task_stat 4129 1 129
	task_stat 4130 1 130
	task_stat 4131 1 131
	task_stat 4132 1 132
	task_stat 4133 1 133
	task_stat 4134 1 134
	
	task_stat 4159 1 135
	
	if 100 = 2
		add_talk #b#b#c186506�ռ�11���İ�ɫƷ���������� #n
	endif
	if 101 = 2
		add_talk #b#b#c186506�ռ�12���İ�ɫƷ���ؼ�װ�� #n
	endif
	if 102 = 2
		add_talk #b#b#c186506�ռ�13���İ�ɫƷ��ͷ��װ�� #n
	endif
	if 103 = 2
		add_talk #b#b#c186506�ռ�14���İ�ɫƷ������װ�� #n
	endif
	if 104 = 2
		add_talk #b#b#c186506�ռ�15���İ�ɫƷ�ʼ��װ�� #n
	endif
	if 105 = 2
		add_talk #b#b#c186506�ռ�16���İ�ɫƷ��Ь��װ�� #n
	endif
	if 106 = 2
		add_talk #b#b#c186506�ռ�17���İ�ɫƷ�ʸ������� #n
	endif
	if 107 = 2
		add_talk #b#b#c186506�ռ�21���İ�ɫƷ���������� #n
	endif
	if 108 = 2
		add_talk #b#b#c186506�ռ�22���İ�ɫƷ���ؼ�װ�� #n
	endif
	if 109 = 2
		add_talk #b#b#c186506�ռ�23���İ�ɫƷ��ͷ��װ�� #n
	endif
	if 110 = 2
		add_talk #b#b#c186506�ռ�24���İ�ɫƷ������װ�� #n
	endif
	if 111 = 2
		add_talk #b#b#c186506�ռ�25���İ�ɫƷ�ʼ��װ�� #n
	endif
	if 112 = 2
		add_talk #b#b#c186506�ռ�26���İ�ɫƷ��Ь��װ�� #n
	endif
	if 113 = 2
		add_talk #b#b#c186506�ռ�27���İ�ɫƷ�ʸ������� #n
	endif
	if 114 = 2
		add_talk #b#b#c186506�ռ�31���İ�ɫƷ���������� #n
	endif
	if 115 = 2
		add_talk #b#b#c186506�ռ�32���İ�ɫƷ���ؼ�װ�� #n
	endif
	if 116 = 2
		add_talk #b#b#c186506�ռ�33���İ�ɫƷ��ͷ��װ�� #n
	endif
	if 117 = 2
		add_talk #b#b#c186506�ռ�34���İ�ɫƷ������װ�� #n
	endif
	if 118 = 2
		add_talk #b#b#c186506�ռ�35���İ�ɫƷ�ʼ��װ��#n
	endif
	if 119 = 2
		add_talk #b#b#c186506�ռ�36���İ�ɫƷ��Ь��װ�� #n
	endif
	if 120 = 2
		add_talk #b#b#c186506�ռ�37���İ�ɫƷ�ʸ������� #n
	endif
	if 121 = 2
		add_talk #b#b#c186506�ռ�41���İ�ɫƷ���������� #n
	endif
	if 122 = 2
		add_talk #b#b#c186506�ռ�42���İ�ɫƷ���ؼ�װ�� #n
	endif
	if 123 = 2
		add_talk #b#b#c186506�ռ�43���İ�ɫƷ��ͷ��װ�� #n
	endif
	if 124 = 2
		add_talk #b#b#c186506�ռ�44���İ�ɫƷ������װ�� #n
	endif
	if 125 = 2
		add_talk #b#b#c186506�ռ�45���İ�ɫƷ�ʼ��װ�� #n
	endif
	if 126 = 2
		add_talk #b#b#c186506�ռ�46���İ�ɫƷ��Ь��װ�� #n
	endif
	if 127 = 2
		add_talk #b#b#c186506�ռ�47���İ�ɫƷ�ʸ������� #n
	endif
	if 128 = 2
		add_talk #b#b#c186506�ռ�51���İ�ɫƷ���������� #n
	endif
	if 129 = 2
		add_talk #b#b#c186506�ռ�52���İ�ɫƷ���ؼ�װ�� #n
	endif
	if 130 = 2
		add_talk #b#b#c186506�ռ�53���İ�ɫƷ��ͷ��װ�� #n
	endif
	if 131 = 2
		add_talk #b#b#c186506�ռ�54���İ�ɫƷ������װ�� #n
	endif
	if 132 = 2
		add_talk #b#b#c186506�ռ�55���İ�ɫƷ�ʼ��װ�� #n
	endif
	if 133 = 2
		add_talk #b#b#c186506�ռ�56���İ�ɫƷ��Ь��װ�� #n
	endif
	if 134 = 2
		add_talk #b#b#c186506�ռ�57���İ�ɫƷ�ʸ������� #n
	endif
	if 135 = 2
		
		add_talk #b#b#c186506�ռ�1������ҩˮ #n
	endif
	
		NPC_COMMIT_OPT 601
	
	talk
endproc

proc 601
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

	task_stat 4100 1 100
	if 100 = 2
		if m_ID = 1019902 or m_ID = 1109902 or m_ID = 1209902 or m_ID = 1509902
			switch m_ID
				case 1019902
					DEC_ITEM_NUM_BYIDX 1019902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4100 1
					call 502
				endcase
				case 1109902
					DEC_ITEM_NUM_BYIDX 1109902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4100 1
					call 502
				endcase
				case 1209902
					DEC_ITEM_NUM_BYIDX 1209902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4100 1
					call 502
				endcase
				case 1509902
					DEC_ITEM_NUM_BYIDX 1509902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4100 1
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
	
	task_stat 4101 1 101
	if 101 = 2
		if m_ID = 2019902 or m_ID = 2029902 or m_ID = 2039902 or m_ID = 2049902
			switch m_ID
				case 2019902
					DEC_ITEM_NUM_BYIDX 2019902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4101 1
					call 502
				endcase
				case 2029902
					DEC_ITEM_NUM_BYIDX 2029902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4101 1
					call 502
				endcase
				case 2039902
					DEC_ITEM_NUM_BYIDX 2039902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4101 1
					call 502
				endcase
				case 2049902
					DEC_ITEM_NUM_BYIDX 2049902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4101 1
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
	
	task_stat 4102 1 102
	if 102 = 2
		if m_ID = 2119902 or m_ID = 2129902 or m_ID = 2139902 or m_ID = 2149902
			switch m_ID
				case 2119902
					DEC_ITEM_NUM_BYIDX 2119902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4102 1
					call 502
				endcase
				case 2129902
					DEC_ITEM_NUM_BYIDX 2129902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4102 1
					call 502
				endcase
				case 2139902
					DEC_ITEM_NUM_BYIDX 2139902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4102 1
					call 502
				endcase
				case 2149902
					DEC_ITEM_NUM_BYIDX 2149902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4102 1
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
	task_stat 4103 1 103
	if 103 = 2
		if m_ID = 2449902 or m_ID = 2429902 or m_ID = 2419902 or m_ID = 2439902
			switch m_ID
				case 2449902
					DEC_ITEM_NUM_BYIDX 2449902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4103 1
					call 502
				endcase
				case 2429902
					DEC_ITEM_NUM_BYIDX 2429902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4103 1
					call 502
				endcase
				case 2419902
					DEC_ITEM_NUM_BYIDX 2419902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4103 1
					call 502
				endcase
				case 2439902
					DEC_ITEM_NUM_BYIDX 2439902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4103 1
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
	task_stat 4104 1 104
	if 104 = 2
		if m_ID = 2219902 or m_ID = 2229902 or m_ID = 1279902 or m_ID = 2249902
			switch m_ID
				case 2219902
					DEC_ITEM_NUM_BYIDX 2219902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4104 1
					call 502
				endcase
				case 2229902
					DEC_ITEM_NUM_BYIDX 2229902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4104 1
					call 502
				endcase
				case 1279902
					DEC_ITEM_NUM_BYIDX 1279902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4104 1
					call 502
				endcase
				case 2249902
					DEC_ITEM_NUM_BYIDX 2249902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4104 1
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
	task_stat 4105 1 105
	if 105 = 2
		if m_ID = 2619902 or m_ID = 2629902 or m_ID = 2639902 or m_ID = 2649902
			switch m_ID
				case 2619902
					DEC_ITEM_NUM_BYIDX 2619902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4105 1
					call 502
				endcase
				case 2629902
					DEC_ITEM_NUM_BYIDX 2629902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4105 1
					call 502
				endcase
				case 2639902
					DEC_ITEM_NUM_BYIDX 2639902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4105 1
					call 502
				endcase
				case 2649902
					DEC_ITEM_NUM_BYIDX 2649902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4105 1
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
	task_stat 4106 1 106
	if 106 = 2
		if m_ID = 1069902 or m_ID = 1189902 or m_ID = 1289902 or m_ID = 1589902
			switch m_ID
				case 1069902
					DEC_ITEM_NUM_BYIDX 1069902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4106 1
					call 502
				endcase
				case 1189902
					DEC_ITEM_NUM_BYIDX 1189902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4106 1
					call 502
				endcase
				case 1289902
					DEC_ITEM_NUM_BYIDX 1289902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4106 1
					call 502
				endcase
				case 1589902
					DEC_ITEM_NUM_BYIDX 1589902 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4106 1
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
	task_stat 4107 1 107
	if 107 = 2
		if m_ID = 1019903 or m_ID = 1109903 or m_ID = 1209903 or m_ID = 1509903
			switch m_ID
				case 1019903
					DEC_ITEM_NUM_BYIDX 1019903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4107 1
					call 502
				endcase
				case 1109903
					DEC_ITEM_NUM_BYIDX 1109903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4107 1
					call 502
				endcase
				case 1209903
					DEC_ITEM_NUM_BYIDX 1209903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4107 1
					call 502
				endcase
				case 1509903
					DEC_ITEM_NUM_BYIDX 1509903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4107 1
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
	task_stat 4108 1 108
	if 108 = 2
		if m_ID = 2019903 or m_ID = 2029903 or m_ID = 2039903 or m_ID = 2049903
			switch m_ID
				case 2019903
					DEC_ITEM_NUM_BYIDX 2019903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4108 1
					call 502
				endcase
				case 2029903
					DEC_ITEM_NUM_BYIDX 2029903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4108 1
					call 502
				endcase
				case 2039903
					DEC_ITEM_NUM_BYIDX 2039903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4108 1
					call 502
				endcase
				case 2049903
					DEC_ITEM_NUM_BYIDX 2049903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4108 1
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
	task_stat 4109 1 109
	if 109 = 2
		if m_ID = 2119903 or m_ID = 2129903 or m_ID = 2139903 or m_ID = 2149903
			switch m_ID
				case 2119903
					DEC_ITEM_NUM_BYIDX 2119903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4109 1
					call 502
				endcase
				case 2129903
					DEC_ITEM_NUM_BYIDX 2129903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4109 1
					call 502
				endcase
				case 2139903
					DEC_ITEM_NUM_BYIDX 2139903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4109 1
					call 502
				endcase
				case 2149903
					DEC_ITEM_NUM_BYIDX 2149903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4109 1
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
	task_stat 4110 1 110
	if 110 = 2
		if m_ID = 2449903 or m_ID = 2429903 or m_ID = 2439903 or m_ID = 2419903
			switch m_ID
				case 2449903
					DEC_ITEM_NUM_BYIDX 2449903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4110 1
					call 502
				endcase
				case 2429903
					DEC_ITEM_NUM_BYIDX 2429903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4110 1
					call 502
				endcase
				case 2439903
					DEC_ITEM_NUM_BYIDX 2439903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4110 1
					call 502
				endcase
				case 2419903
					DEC_ITEM_NUM_BYIDX 2419903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4110 1
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
	task_stat 4111 1 111
	if 111 = 2
		if m_ID = 2219903 or m_ID = 2229903 or m_ID = 1279903 or m_ID = 2249903
			switch m_ID
				case 2219903
					DEC_ITEM_NUM_BYIDX 2219903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4111 1
					call 502
				endcase
				case 2229903
					DEC_ITEM_NUM_BYIDX 2229903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4111 1
					call 502
				endcase
				case 1279903
					DEC_ITEM_NUM_BYIDX 1279903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4111 1
					call 502
				endcase
				case 2249903
					DEC_ITEM_NUM_BYIDX 2249903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4111 1
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
	task_stat 4112 1 112
	if 112 = 2
		if m_ID = 2619903 or m_ID = 2629903 or m_ID = 2639903 or m_ID = 2649903
			switch m_ID
				case 2619903
					DEC_ITEM_NUM_BYIDX 2619903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4112 1
					call 502
				endcase
				case 2629903
					DEC_ITEM_NUM_BYIDX 2629903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4112 1
					call 502
				endcase
				case 2639903
					DEC_ITEM_NUM_BYIDX 2639903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4112 1
					call 502
				endcase
				case 2649903
					DEC_ITEM_NUM_BYIDX 2649903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4112 1
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
	task_stat 4113 1 113
	if 113 = 2
		if m_ID = 1069903 or m_ID = 1189903 or m_ID = 1289903 or m_ID = 1589903
			switch m_ID
				case 1069903
					DEC_ITEM_NUM_BYIDX 1069903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4113 1
					call 502
				endcase
				case 1189903
					DEC_ITEM_NUM_BYIDX 1189903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4113 1
					call 502
				endcase
				case 1289903
					DEC_ITEM_NUM_BYIDX 1289903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4113 1
					call 502
				endcase
				case 1589903
					DEC_ITEM_NUM_BYIDX 1589903 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4113 1
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
	
	task_stat 4114 1 114
	if 114 = 2
		if m_ID = 1019904 or m_ID = 1109904 or m_ID = 1209904 or m_ID = 1509904
			switch m_ID
				case 1019904
					DEC_ITEM_NUM_BYIDX 1019904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4114 1
					call 502
				endcase
				case 1109904
					DEC_ITEM_NUM_BYIDX 1109904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4114 1
					call 502
				endcase
				case 1209904
					DEC_ITEM_NUM_BYIDX 1209904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4114 1
					call 502
				endcase
				case 1509904
					DEC_ITEM_NUM_BYIDX 1509904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4114 1
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
	task_stat 4115 1 115
	if 115 = 2
		if m_ID = 2019904 or m_ID = 2029904 or m_ID = 2039904 or m_ID = 2049904
			switch m_ID
				case 2019904
					DEC_ITEM_NUM_BYIDX 2019904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4115 1
					call 502
				endcase
				case 2029904
					DEC_ITEM_NUM_BYIDX 2029904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4115 1
					call 502
				endcase
				case 2039904
					DEC_ITEM_NUM_BYIDX 2039904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4115 1
					call 502
				endcase
				case 2049904
					DEC_ITEM_NUM_BYIDX 2049904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4115 1
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
	task_stat 4116 1 116
	if 116 = 2
		if m_ID = 2119904 or m_ID = 2129904 or m_ID = 2139904 or m_ID = 2149904
			switch m_ID
				case 2119904
					DEC_ITEM_NUM_BYIDX 2119904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4116 1
					call 502
				endcase
				case 2129904
					DEC_ITEM_NUM_BYIDX 2129904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4116 1
					call 502
				endcase
				case 2139904
					DEC_ITEM_NUM_BYIDX 2139904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4116 1
					call 502
				endcase
				case 2149904
					DEC_ITEM_NUM_BYIDX 2149904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4116 1
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
	task_stat 4117 1 117
	if 117 = 2
		if m_ID = 2419904 or m_ID = 2429904 or m_ID = 2439904 or m_ID = 2449904
			switch m_ID
				case 2419904
					DEC_ITEM_NUM_BYIDX 2419904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4117 1
					call 502
				endcase
				case 2429904
					DEC_ITEM_NUM_BYIDX 2429904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4117 1
					call 502
				endcase
				case 2439904
					DEC_ITEM_NUM_BYIDX 2439904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4117 1
					call 502
				endcase
				case 2449904
					DEC_ITEM_NUM_BYIDX 2449904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4117 1
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
	task_stat 4118 1 118
	if 118 = 2
		if m_ID = 2219904 or m_ID = 2229904 or m_ID = 1279904 or m_ID = 2249904
			switch m_ID
				case 2219904
					DEC_ITEM_NUM_BYIDX 2219904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4118 1
					call 502
				endcase
				case 2229904
					DEC_ITEM_NUM_BYIDX 2229904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4118 1
					call 502
				endcase
				case 1279904
					DEC_ITEM_NUM_BYIDX 1279904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4118 1
					call 502
				endcase
				case 2249904
					DEC_ITEM_NUM_BYIDX 2249904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4118 1
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
	task_stat 4119 1 119
	if 119 = 2
		if m_ID = 2619904 or m_ID = 2629904 or m_ID = 2639904 or m_ID = 2649904
			switch m_ID
				case 2619904
					DEC_ITEM_NUM_BYIDX 2619904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4119 1
					call 502
				endcase
				case 2629904
					DEC_ITEM_NUM_BYIDX 2629904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4119 1
					call 502
				endcase
				case 2639904
					DEC_ITEM_NUM_BYIDX 2639904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4119 1
					call 502
				endcase
				case 2649904
					DEC_ITEM_NUM_BYIDX 2649904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4119 1
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
	task_stat 4120 1 120
	if 120 = 2
		if m_ID = 1069904 or m_ID = 1189904 or m_ID = 1289904 or m_ID = 1589904
			switch m_ID
				case 1069904
					DEC_ITEM_NUM_BYIDX 1069904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4120 1
					call 502
				endcase
				case 1189904
					DEC_ITEM_NUM_BYIDX 1189904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4120 1
					call 502
				endcase
				case 1289904
					DEC_ITEM_NUM_BYIDX 1289904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4120 1
					call 502
				endcase
				case 1589904
					DEC_ITEM_NUM_BYIDX 1589904 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4120 1
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
	task_stat 4121 1 121
	if 121 = 2
		if m_ID = 1019905 or m_ID = 1109905 or m_ID = 1209905 or m_ID = 1509905
			switch m_ID
				case 1019905
					DEC_ITEM_NUM_BYIDX 1019905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4121 1
					call 502
				endcase
				case 1109905
					DEC_ITEM_NUM_BYIDX 1109905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4121 1
					call 502
				endcase
				case 1209905
					DEC_ITEM_NUM_BYIDX 1209905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4121 1
					call 502
				endcase
				case 1509905
					DEC_ITEM_NUM_BYIDX 1509905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4121 1
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
	task_stat 4122 1 122
	if 122 = 2
		if m_ID = 2019905 or m_ID = 2029905 or m_ID = 2039905 or m_ID = 2049905
			switch m_ID
				case 2019905
					DEC_ITEM_NUM_BYIDX 2019905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4122 1
					call 502
				endcase
				case 2029905
					DEC_ITEM_NUM_BYIDX 2029905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4122 1
					call 502
				endcase
				case 2039905
					DEC_ITEM_NUM_BYIDX 2039905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4122 1
					call 502
				endcase
				case 2049905
					DEC_ITEM_NUM_BYIDX 2049905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4122 1
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
	task_stat 4123 1 123
	if 123 = 2
		if m_ID = 2119905 or m_ID = 2129905 or m_ID = 2139905 or m_ID = 2149905
			switch m_ID
				case 2119905
					DEC_ITEM_NUM_BYIDX 2119905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4123 1
					call 502
				endcase
				case 2129905
					DEC_ITEM_NUM_BYIDX 2129905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4123 1
					call 502
				endcase
				case 2139905
					DEC_ITEM_NUM_BYIDX 2139905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4123 1
					call 502
				endcase
				case 2149905
					DEC_ITEM_NUM_BYIDX 2149905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4123 1
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
	task_stat 4124 1 124
	if 124 = 2
		if m_ID = 2419905 or m_ID = 2439905 or m_ID = 2449905 or m_ID = 2429905
			switch m_ID
				case 2419905
					DEC_ITEM_NUM_BYIDX 2419905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4124 1
					call 502
				endcase
				case 2439905
					DEC_ITEM_NUM_BYIDX 2439905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4124 1
					call 502
				endcase
				case 2449905
					DEC_ITEM_NUM_BYIDX 2449905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4124 1
					call 502
				endcase
				case 2429905
					DEC_ITEM_NUM_BYIDX 2429905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4124 1
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
	task_stat 4125 1 125
	if 125 = 2
		if m_ID = 2219905 or m_ID = 2229905 or m_ID = 1279905 or m_ID = 2249905
			switch m_ID
				case 2219905
					DEC_ITEM_NUM_BYIDX 2219905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4125 1
					call 502
				endcase
				case 2229905
					DEC_ITEM_NUM_BYIDX 2229905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4125 1
					call 502
				endcase
				case 1279905
					DEC_ITEM_NUM_BYIDX 1279905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4125 1
					call 502
				endcase
				case 2249905
					DEC_ITEM_NUM_BYIDX 2249905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4125 1
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
	task_stat 4126 1 126
	if 126 = 2
		if m_ID = 2619905 or m_ID = 2629905 or m_ID = 2639905 or m_ID = 2649905
			switch m_ID
				case 2619905
					DEC_ITEM_NUM_BYIDX 2619905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4126 1
					call 502
				endcase
				case 2629905
					DEC_ITEM_NUM_BYIDX 2629905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4126 1
					call 502
				endcase
				case 2639905
					DEC_ITEM_NUM_BYIDX 2639905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4126 1
					call 502
				endcase
				case 2649905
					DEC_ITEM_NUM_BYIDX 2649905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4126 1
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
	task_stat 4127 1 127
	if 127 = 2
		if m_ID = 1069905 or m_ID = 1189905 or m_ID = 1289905 or m_ID = 1589905
			switch m_ID
				case 1069905
					DEC_ITEM_NUM_BYIDX 1069905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4127 1
					call 502
				endcase
				case 1189905
					DEC_ITEM_NUM_BYIDX 1189905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4127 1
					call 502
				endcase
				case 1289905
					DEC_ITEM_NUM_BYIDX 1289905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4127 1
					call 502
				endcase
				case 1589905
					DEC_ITEM_NUM_BYIDX 1589905 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4127 1
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
	task_stat 4128 1 128
	if 128 = 2
		if m_ID = 1019906 or m_ID = 1109906 or m_ID = 1209906 or m_ID = 1509906
			switch m_ID
				case 1019906
					DEC_ITEM_NUM_BYIDX 1019906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4128 1
					call 502
				endcase
				case 1109906
					DEC_ITEM_NUM_BYIDX 1109906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4128 1
					call 502
				endcase
				case 1209906
					DEC_ITEM_NUM_BYIDX 1209906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4128 1
					call 502
				endcase
				case 1509906
					DEC_ITEM_NUM_BYIDX 1509906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4128 1
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
	task_stat 4129 1 129
	if 129 = 2
		if m_ID = 2019906 or m_ID = 2029906 or m_ID = 2039906 or m_ID = 2049906
			switch m_ID
				case 2019906
					DEC_ITEM_NUM_BYIDX 2019906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4129 1
					call 502
				endcase
				case 2029906
					DEC_ITEM_NUM_BYIDX 2029906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4129 1
					call 502
				endcase
				case 2039906
					DEC_ITEM_NUM_BYIDX 2039906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4129 1
					call 502
				endcase
				case 2049906
					DEC_ITEM_NUM_BYIDX 2049906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4129 1
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
	task_stat 4130 1 130
	if 130 = 2
		if m_ID = 2119906 or m_ID = 2129906 or m_ID = 2139906 or m_ID = 2149906
			switch m_ID
				case 2119906
					DEC_ITEM_NUM_BYIDX 2119906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4130 1
					call 502
				endcase
				case 2129906
					DEC_ITEM_NUM_BYIDX 2129906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4130 1
					call 502
				endcase
				case 2139906
					DEC_ITEM_NUM_BYIDX 2139906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4130 1
					call 502
				endcase
				case 2149906
					DEC_ITEM_NUM_BYIDX 2149906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4130 1
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
	task_stat 4131 1 131
	if 131 = 2
		if m_ID = 2439906 or m_ID = 2429906 or m_ID = 2419906 or m_ID = 2449906
			switch m_ID
				case 2439906
					DEC_ITEM_NUM_BYIDX 2439906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4131 1
					call 502
				endcase
				case 2429906
					DEC_ITEM_NUM_BYIDX 2429906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4131 1
					call 502
				endcase
				case 2419906
					DEC_ITEM_NUM_BYIDX 2419906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4131 1
					call 502
				endcase
				case 2449906
					DEC_ITEM_NUM_BYIDX 2449906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4131 1
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
	task_stat 4132 1 132
	if 132 = 2
		if m_ID = 2219906 or m_ID = 2229906 or m_ID = 1279906 or m_ID = 2249906
			switch m_ID
				case 2219906
					DEC_ITEM_NUM_BYIDX 2219906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4132 1
					call 502
				endcase
				case 2229906
					DEC_ITEM_NUM_BYIDX 2229906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4132 1
					call 502
				endcase
				case 1279906
					DEC_ITEM_NUM_BYIDX 1279906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4132 1
					call 502
				endcase
				case 2249906
					DEC_ITEM_NUM_BYIDX 2249906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4132 1
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
	task_stat 4133 1 133
	if 133 = 2
		if m_ID = 2619906 or m_ID = 2629906 or m_ID = 2639906 or m_ID = 2649906
			switch m_ID
				case 2619906
					DEC_ITEM_NUM_BYIDX 2619906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4133 1
					call 502
				endcase
				case 2629906
					DEC_ITEM_NUM_BYIDX 2629906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4133 1
					call 502
				endcase
				case 2639906
					DEC_ITEM_NUM_BYIDX 2639906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4133 1
					call 502
				endcase
				case 2649906
					DEC_ITEM_NUM_BYIDX 2649906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4133 1
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
	task_stat 4134 1 134
	if 134 = 2
		if m_ID = 1069906 or m_ID = 1189906 or m_ID = 1289906 or m_ID = 1589906
			switch m_ID
				case 1069906
					DEC_ITEM_NUM_BYIDX 1069906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4134 1
					call 502
				endcase
				case 1189906
					DEC_ITEM_NUM_BYIDX 1189906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4134 1
					call 502
				endcase
				case 1289906
					DEC_ITEM_NUM_BYIDX 1289906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4134 1
					call 502
				endcase
				case 1589906
					DEC_ITEM_NUM_BYIDX 1589906 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4134 1
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
	task_stat 4159 1 135
	if 135 = 2
		if m_ID = 5006100 or m_ID = 5006101 or m_ID = 5006102 or m_ID = 5006103 or m_ID = 5006106
			switch m_ID
				case 5006100
					DEC_ITEM_NUM_BYIDX 5006100 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4159 1
					call 502
				endcase
				case 5006101
					DEC_ITEM_NUM_BYIDX 5006101 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4159 1
					call 502
				endcase
				case 5006102
					DEC_ITEM_NUM_BYIDX 5006102 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4159 1
					call 502
				endcase
				case 5006103
					DEC_ITEM_NUM_BYIDX 5006103 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4159 1
					call 502
				endcase
				case 5006106
					DEC_ITEM_NUM_BYIDX 5006106 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4159 1
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
	set_sys_msg_mode 1 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc 
;proc 2
;	GET_TRIG_TASK 1 2
;
;	if 2 = 4020
;                task_fini 4020 1
;        endif
;	if 2 = 4021
;                task_fini 4021 1
;        endif
;	if 2 = 4022
;                task_fini 4022 1
;        endif
;	if 2 = 4023
;                task_fini 4023 1
;        endif
;	if 2 = 4024
;                task_fini 4024 1
;        endif
;	if 2 = 4025
;                task_fini 4025 1
;        endif
;endproc

proc 700
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b�һ�����
	add_talk ֧��1��#c6d00d2�߼��ɼ����֤#n�ɶһ�1��#c005ebe�����ɼ����֤#n
	add_talk #b#b
	add_talk ��һ������ 
	strvar num
	GET_ITEM_NUM_INPAK 5030006 1 num
	;����num=1��5030006��Ʒ
	if num = 0
		add_talk ���ߣ����߼��ɼ����֤�� #cF80B0B 0 #n/1
	else
		add_talk ���ߣ����߼��ɼ����֤�� #c186506 $@num$ #n/1
	endif

		option 701 #ͬ��һ�

	talk
endproc

proc 701 
	choose 2 702 703 �һ�1�ų����ɼ����֤����֧��1�Ÿ߼��ɼ����֤         �Ƿ�ȷ�϶һ���
endproc

proc 702
	PKG_IS_UNLOCK 20 30
	if 20 = 0 and 30 = 0
		return
	endif
	strvar num
	GET_ITEM_NUM_INPAK 5030006 1 num
	;����num=5030006��Ʒ
	if @num > 0
			dec_item_num_inpak 5030006 1 0
			ADD_BIND_ITEM_NUM_INPAK 5030005 1 1 0
			else
		ADD_SYS_MSG_START
		ADD_SYS_MSG ���ı�����û�и߼��ɼ����֤���޷�����
		SET_SYS_MSG_MODE 1 0 0
		MSG_INFO
	endif
endproc

proc 703
endproc

proc 705
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b�һ�����
	add_talk ֧��2��#c6d00d2�����ɼ����֤#n�ɶһ�1��#c005ebe�߼��ɼ����֤#n
	add_talk #b#b
	add_talk ��һ������ 
	strvar num
	GET_ITEM_NUM_INPAK 5030007 2 num
	;����num=5030007��Ʒ
	if num < 2
		add_talk ���ߣ��������ɼ����֤�� #cF80B0B $@num$ #n/2
	else
		add_talk ���ߣ��������ɼ����֤�� #c186506 $@num$ #n/2
	endif

		option 706 #ͬ��һ�

	talk
endproc

proc 706
	choose 2 707 708 �һ�1�Ÿ߼��ɼ����֤����֧��2�Ŷ����ɼ����֤         �Ƿ�ȷ�϶һ���
endproc

proc 707
	PKG_IS_UNLOCK 20 30
	if 20 = 0 and 30 = 0
		return
	endif
	strvar num
	GET_ITEM_NUM_INPAK 5030007 1 num
	;����num=5030007��Ʒ
	if @num > 1
		dec_item_num_inpak 5030007 2 0
		ADD_BIND_ITEM_NUM_INPAK 5030006 1 1 0
	else
		ADD_SYS_MSG_START
		ADD_SYS_MSG ���ı�����ȱ�ٶ����ɼ����֤���޷�����
		SET_SYS_MSG_MODE 1 0 0
		MSG_INFO
	endif
endproc

proc 708
endproc
