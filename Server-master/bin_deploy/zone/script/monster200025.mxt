
;�ڳ��й�


proc 1
	strvar Mid Rad
	GET_MON_INFO 1 1 Mid 1 1 1 1 1
	TARGET_ADD_STATUS 3 @Mid 1262 1 3 1
	
	rand Rad 3
	if Rad = 0
		MON_SPEECH @Mid 1 1 1 ����Ŀ�꣬�����Ӷ�ģʽ
	endif
	if Rad = 1
		MON_SPEECH @Mid 1 1 1 Ŀ���ѽ���ƻ���Χ�������Ӷ�ģʽ
	endif
	if Rad = 2
		MON_SPEECH @Mid 1 1 1 ȷ��Ŀ�����������Ӷ�ģʽ
	endif
endproc
proc 2
	strvar ret lv
	get_mon_level ret lv
	if ret = 0
		if lv < 40
			 MON_DROP_BOOTYGROUP 2100 2110 0 0 0
		endif
		if lv >= 40 and lv < 50
			 MON_DROP_BOOTYGROUP 2101 2111 0 0 0
		endif
		if lv >= 50 and lv < 60
			 MON_DROP_BOOTYGROUP 2102 2112 0 0 0
		endif
		if lv >= 60 and lv < 70
			 MON_DROP_BOOTYGROUP 2103 2113 0 0 0
		endif
		if lv >= 70 and lv < 80
			 MON_DROP_BOOTYGROUP 2104 2114 0 0 0
		endif
		if lv >= 80 and lv < 90
			 MON_DROP_BOOTYGROUP 2105 2115 0 0 0
		endif
		if lv >= 90 and lv < 100
			 MON_DROP_BOOTYGROUP 2106 2116 0 0 0
		endif
		if lv >= 100 and lv < 110
			 MON_DROP_BOOTYGROUP 2107 2117 0 0 0
		endif
		if lv >= 110 and lv <= 120
			 MON_DROP_BOOTYGROUP 2108 2118 0 0 0
		endif
	endif

endproc


