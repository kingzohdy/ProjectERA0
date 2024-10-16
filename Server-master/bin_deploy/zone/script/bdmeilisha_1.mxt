


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b#c676464<她躺着的姿态怪异而特别，她就这么躺着，以一种奇特的方式安静地躺着，一动不动，只有那微微起伏的胸口显示着她还有生命的迹象>#n 

;		task_add_option 1368 1 100

		strvar Ret
		task_stat 1373 1 Ret
		if Ret = 2
			option 150 #赶紧抱回去吧！

		endif
	
	talk
	
endproc 

;PROC 100
;	task_do_option 1368 1
;ENDPROC
	
	
proc 150

	APPLY_TMP_ITEM 1460077 0 1
endproc 
	
	