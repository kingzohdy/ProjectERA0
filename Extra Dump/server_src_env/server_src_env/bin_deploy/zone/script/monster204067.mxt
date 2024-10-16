;救出的居民、


proc 1

	strvar Mid
	GET_MON_INFO 0 0 Mid 0 0 0 0 0
	rand 10 3
	
	if 10 = 0
	
		mon_speech @Mid 0 2 1 终于能逃离这让人窒息的地方了！
	endif
	if 10 = 1
		mon_speech @Mid 0 2 1 崔蒂就在矿井最里面的采集场中！
	endif
	if 10 = 2
	
		mon_speech @Mid 0 2 1 一定要救救大家！
	endif

endproc 
