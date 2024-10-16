
proc 2
	strvar ret lv
	get_mon_level ret lv
	if ret = 0
		if lv < 40
			 MON_DROP_BOOTYGROUP 2041 2087 2010 2020 0
		endif                         
		if lv >= 40 and lv < 50       
			 MON_DROP_BOOTYGROUP 2041 2088 2011 2021 0
		endif                         
		if lv >= 50 and lv < 60       
			 MON_DROP_BOOTYGROUP 2041 2089 2012 2022 0
		endif                         
		if lv >= 60 and lv < 70       
			 MON_DROP_BOOTYGROUP 2041 2090 2013 2023 0
		endif                         
		if lv >= 70 and lv < 80       
			 MON_DROP_BOOTYGROUP 2041 2091 2014 2024 0
		endif                         
		if lv >= 80 and lv < 90       
			 MON_DROP_BOOTYGROUP 2041 2092 2015 2025 0
		endif                         
		if lv >= 90 and lv < 100      
			 MON_DROP_BOOTYGROUP 2041 2093 2016 2026 0
		endif                         
		if lv >= 100 and lv < 110     
			 MON_DROP_BOOTYGROUP 2041 2094 2017 2027 0
		endif                         
		if lv >= 110 and lv <= 120    
			 MON_DROP_BOOTYGROUP 2041 2095 2018 2028 0
		endif 
		 MON_DROP_BOOTYGROUP 5060016 0 0 0 0
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
