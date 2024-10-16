
proc 2
	strvar ret lv
	get_mon_level ret lv
	if ret = 0
		if lv < 40
			 MON_DROP_BOOTYGROUP  2120 2130 2140 0 0
		endif                         
		if lv >= 40 and lv < 50       
			 MON_DROP_BOOTYGROUP  2121 2131 2141 0 0
		endif                         
		if lv >= 50 and lv < 60       
			 MON_DROP_BOOTYGROUP  2122 2132 2142 0 0
		endif                         
		if lv >= 60 and lv < 70       
			 MON_DROP_BOOTYGROUP  2123 2133 2143 0 0
		endif                         
		if lv >= 70 and lv < 80       
			 MON_DROP_BOOTYGROUP  2124 2134 2144 0 0
		endif                         
		if lv >= 80 and lv < 90       
			 MON_DROP_BOOTYGROUP  2125 2135 2145 0 0
		endif                         
		if lv >= 90 and lv < 100      
			 MON_DROP_BOOTYGROUP  2126 2136 2146 0 0
		endif                         
		if lv >= 100 and lv < 110     
			 MON_DROP_BOOTYGROUP  2127 2137 2147 0 0
		endif                         
		if lv >= 110 and lv <= 120    
			 MON_DROP_BOOTYGROUP  2128 2138 2148 0 0
		endif 
		
	endif


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	get_pworld_var 88 32 1 32
	var 32 6
	set_pworld_var 88 32 @32 1
	range_set 4 3
	range_set 4 6
	OPEN_PWORLD_AWARD_UI
	GET_PWORLD_ROLE_NUM 50 1
	if @50 > 1
	add 50 2
	RANGE_ADD_STATUS 0 1039 @50 1200 2
	endif
	
	
	
	
endproc
