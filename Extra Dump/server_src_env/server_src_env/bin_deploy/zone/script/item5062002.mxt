;�ͼ��ر�ͼ3

proc 1
	strvar Odds1 Odds2 Item1 Item2 Item3 Rand1
	var Item1 0
	var Item2 0
	;USE_TREASURE_PIC  #��ƷID #��Ч��Χ @����ֵ @�Ƕ� @��ͼID @X���� @Y����
	USE_TREASURE_PIC 5062002 500 10 4 5 6 7
	dev 6 100
	dev 7 100
	GET_ITEM_ONEDAY_INFO 5062000 1 800 801
		;���ʹ�ô���
	if 800 >= 20
		;ʹ��20������
		add_sys_msg_start
		add_sys_msg ����Ʒ����ʹ�ô����Ѵﵽ���ޣ���
		set_sys_msg_mode 4 0 0
		msg_info
	else

	if 10 = 0
			;ADD_ITEM_ON_MAP  #��һ������Ϊ����ֵ���ڶ���������ʾ�Ƿ�Ҫ�ͻ���������ʰȡ��� ��������� itemid_1,itemnum_1,itemid_2,itemnum_2,itemid_2,itemnum_2
			rand Odds1 100
			rand Rand1 2
					if Odds1 < 10
						var Item1 5229300
					endif
					if Odds1 >= 10 and Odds1 < 25
						var Item1 5222020
					endif
					if Odds1 >= 25 and Odds1 < 40
						var Item1 5221020
					endif
					if Odds1 >= 40 and Odds1 < 55
						var Item1 5222120
					endif
					if Odds1 >= 55 and Odds1 < 70
						var Item1 5221120
					endif
					if Odds1 >= 70 and Odds1 < 75
						var Item1 5221220
					endif
					if Odds1 >= 75 and Odds1 < 80
						var Item1 5221320
					endif					
					if Odds1 >= 80 and Odds1 < 85
						var Item1 5229500
					endif			
					if Odds1 >= 85 and Odds1 < 95
						var Item1 5200003
					endif			
					if Odds1 >= 95 and Odds1 < 100
						var Item1 5229100
					endif
			if Rand1 = 1
			rand Odds2 100
					if Odds2 < 10
						var Item2 5229300
					endif
					if Odds2 >= 10 and Odds2 < 25
						var Item2 5222020
					endif
					if Odds2 >= 25 and Odds2 < 40
						var Item2 5221020
					endif
					if Odds2 >= 40 and Odds2 < 55
						var Item2 5222120
					endif
					if Odds2 >= 55 and Odds2 < 70
						var Item2 5221120
					endif
					if Odds2 >= 70 and Odds2 < 75
						var Item2 5221220
					endif
					if Odds2 >= 75 and Odds2 < 80
						var Item2 5221320
					endif					
					if Odds2 >= 80 and Odds2 < 85
						var Item2 5229500
					endif			
					if Odds2 >= 85 and Odds2 < 95
						var Item2 5200003
					endif			
					if Odds2 >= 95 and Odds2 < 100
						var Item2 5229100
					endif
			endif
			ADD_ITEM_ON_MAP 8 1 @Item1 1 @Item2 1 0 0
			
			
			
			If 8 = 0
				ITEM_ADD_ONEDAY 5062000 1
				;ʹ�ô���+1
				dec_item_num_inpak 5062002 1 0

;				rand 21 49
;				If 21 = 49
	;GET_MAP_MON_NUM #��ͼID #���ﶨ��ID #���ر�־ #�������� #�ҵ���һ�������ʵ���ɣ� 
;				GET_MAP_MON_NUM 2100 204080 0 23 0
;				GET_MAP_MON_NUM 2200 204080 0 24 0
;				GET_MAP_MON_NUM 2300 204080 0 25 0
;				GET_MAP_MON_NUM 2400 204080 0 26 0
;				GET_MAP_MON_NUM 2500 204080 0 27 0
;				If 23 = 0 And 24 = 0 And 25 = 0 And 26 = 0 And 27 = 0 
;					rand 22 9
	;NEW_MAP_MON #����id #�������� #��ͼID  #����x #����y #����뾶 #Ѳ��ID #�Ƿ�������� #���� #����ʱ�� #���ر�־ #��һֻ�����ʵ��ID
;					If 22 = 0
;					NEW_MAP_MON 204080 1 2100 32000	6100 500 1 0 0 0 0 0
;					add_sys_msg_start
;					add_sys_msg ��صĲ��ȶ�������������ʱ�շ���Ť����ͭ�Ĵ������ʱ��͵���ߣ�
;					set_sys_msg_mode 1 0 0
;					svr_msg_info
;					endif
;					If 22 = 1
;					NEW_MAP_MON 204080 1 2100 8000	23600 500 1 0 0 0 0 0
;					add_sys_msg_start
;					add_sys_msg ��صĲ��ȶ�������������ʱ�շ���Ť����ͭ�Ĵ������ʱ��͵���ߣ�
;					set_sys_msg_mode 1 0 0
;					svr_msg_info
;					endif
;					If 22 = 2
;					NEW_MAP_MON 204080 1 2200 9800	22800 500 1 0 0 0 0 0
;					add_sys_msg_start
;					add_sys_msg ��صĲ��ȶ�������������ʱ�շ���Ť����ѩԭ������ʱ��͵���ߣ�
;					set_sys_msg_mode 1 0 0
;					svr_msg_info
;					endif
;					If 22 = 3
;					NEW_MAP_MON 204080 1 2200 13800	20600 500 1 0 0 0 0 0
;					add_sys_msg_start
;					add_sys_msg ��صĲ��ȶ�������������ʱ�շ���Ť����ѩԭ������ʱ��͵���ߣ�
;					set_sys_msg_mode 1 0 0
;					svr_msg_info
;					endif
;					If 22 = 4
;					NEW_MAP_MON 204080 1 2300 20700	2500 500 1 0 0 0 0 0
;					add_sys_msg_start
;					add_sys_msg ��صĲ��ȶ�������������ʱ�շ���Ť��������ɳĮ������ʱ��͵���ߣ�
;					set_sys_msg_mode 1 0 0
;					svr_msg_info
;					endif
;					If 22 = 5
;					NEW_MAP_MON 204080 1 2300 19400	21700 500 1 0 0 0 0 0
;					add_sys_msg_start
;					add_sys_msg ��صĲ��ȶ�������������ʱ�շ���Ť��������ɳĮ������ʱ��͵���ߣ�
;					set_sys_msg_mode 1 0 0
;					svr_msg_info
;					endif
;					If 22 = 6
;					NEW_MAP_MON 204080 1 2400 4600	21600 500 1 0 0 0 0 0
;					add_sys_msg_start
;					add_sys_msg ��صĲ��ȶ�������������ʱ�շ���Ť�����������׵س�����ʱ��͵���ߣ�
;					set_sys_msg_mode 1 0 0
;					svr_msg_info
;					endif
;					If 22 = 7
;					NEW_MAP_MON 204080 1 2400 20600	4000 500 1 0 0 0 0 0
;					add_sys_msg_start
;					add_sys_msg ��صĲ��ȶ�������������ʱ�շ���Ť�����������׵س�����ʱ��͵���ߣ�
;					set_sys_msg_mode 1 0 0
;					svr_msg_info
;					endif
;					If 22 = 8
;					NEW_MAP_MON 204080 1 2500 12700	32100 500 1 0 0 0 0 0
;					add_sys_msg_start
;					add_sys_msg ��صĲ��ȶ�������������ʱ�շ���Ť�����������ų�����ʱ��͵���ߣ�
;					set_sys_msg_mode 1 0 0
;					svr_msg_info
;					endif
;					If 22 = 9
;					NEW_MAP_MON 204080 1 2500 12100	21500 500 1 0 0 0 0 0
;					add_sys_msg_start
;					add_sys_msg ��صĲ��ȶ�������������ʱ�շ���Ť�����������ų�����ʱ��͵���ߣ�
;					set_sys_msg_mode 1 0 0
;					svr_msg_info
;					endif
;				endif
;				endif

				add_sys_msg_start
				add_sys_msg ������иŬ���������ڻ���˱����е���Ʒ��
				set_sys_msg_mode 3 0 0
				msg_info

			Else
				add_sys_msg_start
				add_sys_msg ������Ʒ��λ�ò��㣡
				set_sys_msg_mode 4 0 0
				msg_info
			endif
	endif

	If 10 = 1
	;GET_ITEM_USE_DUR #���ر�־ #��Ʒʣ��ʹ�ô��� #��Ʒ���ʹ�ô���
	GET_ITEM_USE_DUR 0 700 701
			If 700 = 1
			add_sys_msg_start
			add_sys_msg �ر�ͼ�����岻�����ٴ�ʹ�ý����ݻ٣�
			set_sys_msg_mode 4 0 0
			msg_info
			endif
			If 700 = 0
			add_sys_msg_start
			add_sys_msg �ر�ͼ����٣�
			set_sys_msg_mode 4 0 0
			msg_info
			endif
			If 700 != 0 Or 700 != 1
			add_sys_msg_start
			add_sys_msg ���ز��ڸõ�ͼ���뵽Ŀ���ͼʹ�ã�
			set_sys_msg_mode 4 0 0
			msg_info
			endif
	endif

	If 10 >= 2 
	;GET_ITEM_USE_DUR #���ر�־ #��Ʒʣ��ʹ�ô��� #��Ʒ���ʹ�ô���
	GET_ITEM_USE_DUR 0 700 701
			If 700 = 0
			add_sys_msg_start
			add_sys_msg �ر�ͼ����٣�
			set_sys_msg_mode 4 0 0
			msg_info
			else
			add_sys_msg_start
			add_sys_msg �����ڵ�ͼ��$@6$��$@7$������λ�ã�
			set_sys_msg_mode 3 0 0
			msg_info
			endif


	endif
	endif

endproc	