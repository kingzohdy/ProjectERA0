;��
 
proc 1	
	dialog 1
	
	get_wedding_sth 4 60 61 ;�Ƿ������ȡ��������䡱
	get_wedding_sth 2 40 41 ;�Ƿ���ȡ�ˡ�������䡱
	get_wedding_sth 5 70 71 ;�Ƿ������ȡ�������̻���
	get_wedding_sth 3 50 51 ;�Ƿ���ȡ�ˡ������̻���
	get_wedding_sth 6 80 81 ;�Ƿ����˷��޹�ϵ
	get_wedding_sth 8 90 91 ;�Ƿ���Ծ��С�������䡱
	get_wedding_sth 9 30 31 ;�Ƿ���Ծ��С��������Ρ�
	get_wedding_sth 7 100 101 ;�Ƿ��������¹�ϵ
	get_wedding_sth 10 140 141 ;˫���Ƿ���ȡ�ˡ�������䡱
	get_wedding_sth 1 110 111
	
	add_talk $_npc$:
	add_talk #b#b���ǰ��ݣ�������⡣
	add_talk #b#b#cFF0080���ϵ�֮������Ը�ڴ�����ÿһ�����˲�������ĵ��ã���֤�����ǵ��Ҹ������㡣#n
	add_talk #b#bԸ�����������ճɾ��������š�
	
	if @61 = 1 and @60 = 1 and @41 = 1 and @40 = 0 and 141 = 1 and 140 = 0
		add_talk #b#b
		add_talk #b#b#c186506��ϲ����������ʽ��Ϊ���ޣ����������������ﵽ�ҵĸ�ǰ��ȡ��֤���ǰ���ġ�������䡱#n
	endif
	if @71 = 1 and @70 = 1 and @51 = 1 and @50 = 0
		add_talk #b#b
		add_talk #b#b#c186506����������ڽ����У����������������������ȡ�������̻���#n
	endif
	
	if @41 = 1 and @40 = 1
		if 141 = 1 and 140 = 0 
			;�Է���δ��ȡ������
			add_talk #b#b
			add_talk #b#b#c186506�������ﶼ��ȡ��������䡱�󣬼��ɼ������л�����ʽ#n
		endif
	endif
	
	if @41 = 1 and @40 = 1
		if 141 = 1 and 140 = 1
			if ( @91 = 1 and @90 = 0 ) or ( @30 = 0 and @31 = 1 )
				add_talk #b#b
				add_talk #b#b#c186506�����������ɻ����￪��������ʽ:#n
				if @91 = 1 and @90 = 0
					add_talk #b#b#c186506��ʽһ����������䡿��������һͬ�����������ʱ��#n#c005ebe�������ˡ���Լ��������񡱿ɾ��У�#n
				endif
				if @31 = 1 and @30 = 0
					add_talk #b#b#c186506��ʽ�������������Ρ������Ҹ���ףԸ�����ÿλ����#n#c005ebe�����޶����ˡ��������񡱿ɾ��У�#n
				endif
			endif
		endif
	endif
	
	if @61 = 1 and @60 = 1
		;���ɹ������Կ�ʼ��ȡ��������
		if @41 = 1 and @40 = 0
			;��û����ȡ
			option 240 #���������������ͳ�ŵ�ġ�������䡿
		endif
	endif
	
	if @71 = 1 and @70 = 1
		;��ʼ��������ˣ�������ȡ�����̻���
		if @51 = 1 and @50 = 0
			;��û����ȡ��
			option 250 #��ȡ�������̻�������ȼ���Ļ���
		endif
	endif

	;�Ƿ��޹�ϵ�����������������
	if @81 = 1 and @80 = 0
		option 200 #���ơ������ײ͡�
		option 211 #����Ϊ�������ֻ���
	endif
	
	;�Ƿ��޹�ϵ���������񸸾��л
	if @81 = 1 and @80 = 1 and @41 = 1 and @40 = 1
		if 141 = 1 and 140 = 1
			if @91 = 1 and @90 = 0
				option 220 #���С�������䡿
			endif
			if @31 = 1 and @30 = 0
				option 230 #���С��������Ρ�
			endif
		endif
	endif

	if (@81 = 1 and @80 = 1) or (@101 = 1 and @100 = 1)
		option 300 #����Ҫ������¹�ϵ
	endif
	
	talk
endproc

proc 200
	dialog 1
	add_talk $_npc$:
	add_talk #b#b#cFF0080���鱾�޹����Ψ�������޼ۡ�
	add_talk #b#b��Ҫ����ɽ�ˣ������غ������#n
	add_talk #b#b
	add_talk #b#b���������ϵ�֮�����#c186506ÿ������ֻ��ͬʱ����1���ײ�#n��һ��ѡ�񲻵��ʱ�Թ�Ȳ��ðû��˻���
	
	option 201 #������ײ͡�����
	option 202 #����Լ�ײ͡�����
	option 203 #�������ײ͡�����
	
	talk
endproc

proc 201
	dialog 1
	add_talk #b#b#b#b#b#b#b��#c186506����ײ�#n���嵥
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#c186506��ʽһ����������#n
	add_talk #b#b#b �������Ļ�Լŵ�Ժ󼴿ɽ�Ϊ����
	add_talk #b#b#b#b#b#b#b#c186506��ʽ���������ָ#n
	add_talk  ��Ϊ���޺����˿����񸸡���ȡ��#c6d00d2�������#n��
	
	option 204 #���ơ�����ײ͡������ã�90��ң�
	option 400 #��������䡿����
	option 200 #�������˽������������ײ�
	talk
endproc

proc 400
	dialog 1
	add_talk $_npc$:
	call 411
	option 201 #�������˽��¡�����ײ͡�
	talk
endproc

proc 202
	dialog 1
	add_talk #b#b#b#b#b#b#b��#c005ebe��Լ�ײ�#n���嵥
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#c186506��ʽһ����������#n
	add_talk #b#b#b �������Ļ�Լŵ�Ժ󼴿ɽ�Ϊ����
	add_talk #b#b#b#b#b#b#b#c186506��ʽ���������ָ#n
	add_talk  ��Ϊ���޺����˿����񸸡���ȡ��#c6d00d2�������#n��
	add_talk #b#b#b#b#b#b#b#c186506��ʽ�����������#n
	add_talk  ���л�����������񸸡���ȡ��#c005ebe�����̻�#n��

	option 205 #���ơ���Լ�ײ͡������ã�1314��ң�
	option 402 #��������䡿����
	option 403 #�������̻�������
	option 200 #�������˽������������ײ�
	talk
endproc

proc 402
	dialog 1
	add_talk $_npc$:
	call 411
	option 202 #�������˽��¡���Լ�ײ͡�
	talk
endproc

proc 403
	dialog 1
	add_talk $_npc$:
	call 412
	option 202 #�������˽��¡���Լ�ײ͡�
	talk
endproc

proc 203
	dialog 1
	add_talk #b#b#b#b#b#b#b��#c6d00d2�����ײ�#n���嵥
	add_talk #b#b
	add_talk #b#b#b#b#b#b#b#c186506��ʽһ����������#n
	add_talk #b#b#b �������Ļ�Լŵ�Ժ󼴿ɽ�Ϊ����
	add_talk #b#b#b#b#b#b#b#c186506��ʽ���������ָ#n
	add_talk  ��Ϊ���޺����˿����񸸡���ȡ��#c6d00d2�������#n��
	add_talk #b#b#b#b#b#b#b#c186506��ʽ�����������#n
	add_talk  ���л�����������񸸡���ȡ��#c005ebe�����̻�#n��
	add_talk #b#b#b#b#b#b#b#c186506��Ŀ�ģ���������#n
	add_talk ���л������κ󽫳ˡ�#c005ebe�����ܳ�#n������ʱ���
	
	option 206 #���ơ������ײ͡������ã�5200��ң�
	option 404 #��������䡿����
	option 405 #�������̻�������
	option 406 #�������ܳ�������
	option 200 #�������˽������������ײ�
	talk
endproc

proc 404
	dialog 1
	add_talk $_npc$:
	call 411
	option 203 #�������˽��¡������ײ͡�
	talk
endproc

proc 405
	dialog 1
	add_talk $_npc$:
	call 412
	option 203 #�������˽��¡������ײ͡�
	talk
endproc

proc 406
	dialog 1
	add_talk $_npc$:
	call 413
	option 203 #�������˽��¡������ײ͡�
	talk
endproc

proc 204
	choose 1  207 210 ��������ײ���Ҫ#cff4e00֧��90���#n\n�Ƿ�ȷ�϶��Ƹ��ײ�
endproc

proc 205
	choose 1  208 210 ���Ƽ�Լ�ײ���Ҫ#cff4e00֧��1314���#n\n�Ƿ�ȷ�϶��Ƹ��ײ�
endproc

proc 206
	choose 1  209 210 ���ƺ����ײ���Ҫ#cff4e00֧��5200���#n\n�Ƿ�ȷ�϶��Ƹ��ײ�
endproc

proc 210
endproc

proc 207
	order_wedding_meal_check 10
	if @10 = 1
		;���Զ���
		DEC_MONEY_TEST 0 90 60 12
		if @12 = 0
			GET_PACK_SPACE 0 30
			if @30 > 0
				DEC_MONEY 0 90 60 13
				if @13 = 0
					set_wedding_sth 1 1 11
					if @11 = 1
						ADD_BIND_ITEM_NUM_INPAK 5251132 1 1 31
						
						;��ʾ#������ҵ�����#�ɹ�������#�ײ�����#�������񸸡�������и��ײ͵Ļ���
						broadcast_wedding_tip 4 0
						
						; add_sys_msg_start
						; add_sys_msg ��ɹ�����������ײͣ������񸸡�������и��ײ͵Ļ���
						; set_sys_msg_mode 3 0 0
						; MSG_INFO
						return
					endif
				endif
			else
				add_sys_msg_start
				add_sys_msg ��ı�������������������һ����λ
				set_sys_msg_mode 3 0 0
				MSG_INFO
				return
			endif
		else
			add_sys_msg_start
			add_sys_msg ����еĽ�Ҳ���֧������ײ͵ķ��ã���ʱ�޷�����
			set_sys_msg_mode 3 0 0
			MSG_INFO
			return
		endif
	endif
endproc

proc 208
	order_wedding_meal_check 10
	if @10 = 1
		;���Զ���
		DEC_MONEY_TEST 0 1314 60 12
		if @12 = 0
			GET_PACK_SPACE 0 30
			if @30 > 0
				DEC_MONEY 0 1314 60 13
				if @13 = 0
					set_wedding_sth 1 2 11
					if @11 = 1
						ADD_BIND_ITEM_NUM_INPAK 5251133 1 1 31
						
						;��ʾ#������ҵ�����#�ɹ�������#�ײ�����#�������񸸡�������и��ײ͵Ļ���
						broadcast_wedding_tip 4 0
						
						; add_sys_msg_start
						; add_sys_msg ��ɹ������˼�Լ�ײͣ������񸸡�������и��ײ͵Ļ���
						; set_sys_msg_mode 3 0 0
						; MSG_INFO
						return
					endif
				endif
			else
				add_sys_msg_start
				add_sys_msg ��ı�������������������һ����λ
				set_sys_msg_mode 3 0 0
				MSG_INFO
				return
			endif
		else
			add_sys_msg_start
			add_sys_msg ����еĽ�Ҳ���֧����Լ�ײ͵ķ��ã���ʱ�޷�����
			set_sys_msg_mode 3 0 0
			MSG_INFO
			return
		endif
	endif
endproc

proc 209
	order_wedding_meal_check 10
	if @10 = 1
		;���Զ���
		DEC_MONEY_TEST 0 5200 60 12
		if @12 = 0
			GET_PACK_SPACE 0 30
			if @30 > 0
				DEC_MONEY 0 5200 60 13
				if @13 = 0
					set_wedding_sth 1 3 11
					if @11 = 1
						ADD_BIND_ITEM_NUM_INPAK 5251134 1 1 31
						
						;��ʾ#������ҵ�����#�ɹ�������#�ײ�����#�������񸸡�������и��ײ͵Ļ���
						broadcast_wedding_tip 4 0
						
						; add_sys_msg_start
						; add_sys_msg ��ɹ������˺����ײͣ������񸸡�������и��ײ͵Ļ���
						; set_sys_msg_mode 3 0 0
						; MSG_INFO
						return
					endif
				endif
			else
				add_sys_msg_start
				add_sys_msg ��ı�������������������һ����λ
				set_sys_msg_mode 3 0 0
				MSG_INFO
				return
			endif
		else
			add_sys_msg_start
			add_sys_msg ����еĽ�Ҳ���֧�������ײ͵ķ��ã���ʱ�޷�����
			set_sys_msg_mode 3 0 0
			MSG_INFO
			return
		endif
	endif
endproc

proc 211
	dialog 1
	add_talk #b#b#b#b#b#b#b#b#c005ebe������׼����
	add_talk #b#b
	add_talk ���ʼ����ʱ����Ҫ���Ʒ���ʾ�ġ��ײ��嵥��
	add_talk ����л�ʷ�ߣ����Զ��������еġ�������䡱
	add_talk ����л���ʱ����������˫������Ҫ�ڻ����ֳ�#n
	
	option 299 #��ʼ������ʽ
	
	talk
endproc

proc 299
	begin_wedding_check 20
	if @20 = 1 ;���Կ�ʼ����
		broadcast_wedding_tip 1 21
		if @21 = 1
			oath_ui 22  ;˫�������������
		endif
	endif
endproc

;�������
proc 220
	begin_wedding_celebration_check 10
	if @10 = 1 ;���Կ�ʼ���������
		set_wedding_sth 4 0 80
		;��¼����������Ϣ�������ֺ���俪ʼ��ʱ���
		if @80 = 1
			;��ʾ#��������#��#��������#�����⺣̲������¡�صĻ�����䣡���Ҽ�����ȡ�����̻�ѡ��
			broadcast_wedding_tip 2 11
			call 1
			
			NEW_MAP_MON 352120 1 4403 23278 15083 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22298 15887 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 23695 15238 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22829 14503 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 23513 15128 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 23050 14526 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 23765 15682 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22531 14560 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 23634 15917 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22357 14756 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 23397 16051 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22866 16066 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22559 16005 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22260 15593 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22273 15032 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22244 15304 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 23136 16082 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 23783 15429 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 23273 14857 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 23223 14650 0 0 0 0 1800 0 0
			
			NEW_MAP_MON 352127 1 4403 23275 14961 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 23273 14748 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 23147 14577 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 22940 14504 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 22668 14518 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 22426 14653 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 22300 14890 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 22253 15168 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 22248 15443 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 22277 15734 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 22420 15958 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 22715 16042 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 23004 16077 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 23274 16068 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 23522 15987 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 23714 15796 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 23779 15539 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 23763 15323 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 23611 15169 0 0 0 0 1800 0 0
			NEW_MAP_MON 352127 1 4403 23393 15108 0 0 0 0 1800 0 0
			
			NEW_MAP_MON 352123 1 4403 22119 14854 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 21824 14988 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 21525 15076 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 21228 15016 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 20992 14841 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 20709 14802 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 20425 14900 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 20181 15080 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19963 15292 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19772 15510 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19609 15765 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19495 16013 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19379 16275 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19298 16520 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19217 16834 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19155 17138 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19114 17478 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19101 17820 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19103 18162 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19141 18469 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19199 18743 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19267 19031 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19366 19285 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19495 19539 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19678 19758 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 19897 19942 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 20155 20053 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 20457 20120 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 20742 20129 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 21011 20065 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 21283 19909 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 21515 19748 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 21728 19502 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 21870 19208 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 21933 18919 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 22102 18696 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 22307 18505 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 22569 18329 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 22839 18173 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 23058 17916 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 23206 17641 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 23196 17325 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 23110 17019 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 23177 16683 0 0 0 0 1800 0 0
			NEW_MAP_MON 352123 1 4403 23340 16415 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 21773 18817 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 21684 18728 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 21599 18601 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 21550 18461 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 21533 18335 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 21564 18170 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 21626 18037 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 21726 17916 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 21853 17829 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 21997 17781 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22147 17767 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22300 17796 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22431 17867 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22542 17954 0 0 0 0 1800 0 0
			NEW_MAP_MON 352120 1 4403 22633 18087 0 0 0 0 1800 0 0
			
			NEW_MAP_MON 352126 1 4403 26367 10497 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 26021 10497 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 25705 10605 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 25427 10798 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 25227 11068 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 25121 11389 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 25074 11742 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 24989 12089 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 24813 12412 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 24607 12662 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 24363 12908 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 24111 13117 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 23898 13362 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 23676 13606 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 23468 13833 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 23267 14077 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 23822 14590 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 24008 14355 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 24191 14122 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 24375 13865 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 24572 13629 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 24814 13368 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 25036 13148 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 25275 12945 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 25538 12735 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 25869 12643 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 26244 12624 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 26599 12460 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 26872 12183 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 27053 11867 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 27107 11508 0 0 0 0 1800 0 0
			NEW_MAP_MON 352126 1 4403 27091 11138 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 27115 11325 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 27103 11702 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 26988 12052 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 26754 12355 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 26440 12579 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 26060 12655 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 25687 12649 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 25403 12851 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 25159 13046 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 24939 13261 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 24691 13516 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 24479 13754 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 24289 14004 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 24105 14245 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 23919 14479 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 23368 13955 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 23569 13725 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 23783 13481 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 24005 13226 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 24236 13014 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 24492 12784 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 24719 12541 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 24936 12269 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 25079 11930 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 25084 11565 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 25157 11226 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 25315 10917 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 25547 10681 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 25854 10529 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 26204 10487 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 23154 14195 0 0 0 0 1800 0 0
			NEW_MAP_MON 352125 1 4403 23749 14737 0 0 0 0 1800 0 0
		endif
	endif
endproc

proc 230
	begin_honeymoon_travel_check 10
	if @10 = 1 ;���Կ�ʼ������
		set_wedding_sth 5 0 81   ;��¼���ε���Ϣ
		if @81 = 1
			broadcast_wedding_tip 3 11
			if @11 = 1
				NEW_MACHINE_PATROL_SPOUSE 1460090 1130 23127 14948 16
			endif
		endif
	endif
endproc

proc 240
	strvar ret zhi num
	get_wedding_sth 2 ret 40
	if @40 = 1 and @ret = 0
		GET_PACK_SPACE 0 30
		if @30 > 0
			GET_ROLE_VAR 72 8 0 zhi
			if zhi = 0
				ADD_BIND_ITEM_NUM_INPAK 2911000 1 1 31
				
				add_sys_msg_start
				add_sys_msg ��������Ͻӹ��������Ű����ŵ�ġ�������䡱
				set_sys_msg_mode 3 0 0
				MSG_INFO
				
				add zhi 1
				SET_ROLE_VAR 72 8 @zhi 0
				
				if @31 = 0
					set_wedding_sth 2 0 0
				endif
			endif
			if zhi > 0
				GET_ITEM_NUM_INPAK 2911000 0 num
				DEC_ITEM_NUM_INPAK 2911000 @num 0
				
				ADD_BIND_ITEM_NUM_INPAK 2911000 1 1 31
				
				add_sys_msg_start
				add_sys_msg �㽻���������������ġ�������䡱���������Ͻӹ��µġ�������䡱
				set_sys_msg_mode 3 0 0
				MSG_INFO
				
				if @31 = 0
					set_wedding_sth 2 0 0
				endif
			endif
		else
			add_sys_msg_start
			add_sys_msg ��ı�������������������һ����λװ�š�������䡱
			set_sys_msg_mode 3 0 0
			MSG_INFO
			return
		endif
	else
		add_sys_msg_start
		add_sys_msg ���Ѿ���ȡ����������䡱��
		set_sys_msg_mode 3 0 0
		MSG_INFO
		return
	endif
endproc

;��ȡ�������̻�������ȼ���Ļ���
proc 250
	strvar ret
	get_wedding_sth 3 ret 40
	if @40 = 1 and @ret = 0
		GET_PACK_SPACE 0 30
		if @30 > 0
			ADD_BIND_ITEM_NUM_INPAK 5251136 1 1 31
			
			add_sys_msg_start
			add_sys_msg ��������Ͻӹ����������������貵ġ������̻���
			set_sys_msg_mode 3 0 0
			MSG_INFO
			
			if @31 = 0
				set_wedding_sth 3 0 0
			endif
		else
			add_sys_msg_start
			add_sys_msg ��ı�������������������һ����λװ�š������̻���
			set_sys_msg_mode 3 0 0
			MSG_INFO
			return
		endif
	else
		add_sys_msg_start
		add_sys_msg ���Ѿ���ȡ���������̻�����
		set_sys_msg_mode 3 0 0
		MSG_INFO
		return
	endif
endproc

proc 300
	dialog 1
	add_talk #b#b#b#b#b#b#b#b#c005ebe������׼����
	add_talk #b#b
	add_talk ������ϵʱ������������������ɷ���
	add_talk ��Է�����ʱ��˫����ֱ���Զ������ϵ
	add_talk ������ϵǰ����������ṩ�����������#n
	add_talk #b#b
	add_talk #cF80B0Bע�⣺��������˻����ײͣ����ֺ��ײͽ������#n
	
	option 301 #���Ѿ������������ѡ��
	
	talk
endproc

proc 301
	remove_relation_ui 0
endproc

proc 411
	; add_talk #b#b��������䡿�ǰ����ŵ����������ָ��������ָ��ζ�Ű����ů������Ѫ�����������˵����ָ��������Ѫ����ͨ������ָ������������֮������
	add_talk #b#b��������䡿�ǰ����ŵ���������ѽ�ָ��������ָ����ζ�Ž������������ֿ���
	add_talk #b#b���˫�ֵ���ָ�������������࿿��ʣ����ָ��ָ����Ӧ��������ʱ�ٰ�ָ��һ��һ�ֿ���������ͻ������ˡ�
	add_talk #b#b��Ϊ����ָ���Ǵ�����ޡ�
	add_talk #b#b
	add_talk #c186506��������䡿
	add_talk װ������������
	add_talk ��ָ��;��װ����ɻ�á������㡱���޼��ܣ�ʩ�ż��ܿ����ϴ��͵����µ����ԡ�#n
endproc

proc 412
	add_talk #b#b�����˼�Լ������ײ͵����ˣ�������ʽ��������������롾������䡿������ȡ1�ݡ������̻�����
	add_talk #b#b��������˿ɵ�ȼ�̻������������һͬ����������һ�̡�
	add_talk #b#b
	add_talk #c186506�������̻�����
	add_talk ��ȡ������1. �������˼�Լ������ײ�
	add_talk #b#b#b#b#b2. �ɹ������ˡ�������䡿
	add_talk ʹ��������ֻ���ڻ����ֳ�ʹ��
	add_talk �̻��������ɼ��̻�ȼ�ղ����ġ��̻��ǳ����ɻ����ϡ����#n
endproc

proc 413
	add_talk #b#b�����˺����ײ͵����ˣ�������ʽ��������������롾�������Ρ��������������ܳ�������ʱ��ǡ�
	add_talk #b#b���������˿����ܳ����������͡��������򡿣�Ըÿ���˶��������������е���һ�롣
	add_talk #b#b
	add_talk #c186506���������򡿣�
	add_talk ʹ�÷�����ֻ��ʹ�á������ܳ����ļ������ͻ���
	add_talk �����������ɼ������ڵ��ϵġ��������򡿿ɻ����ϡ����#n
endproc
