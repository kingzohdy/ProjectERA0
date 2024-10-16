;公会守护枢纽活动    ,购买buff
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b你好，我将负责记录公会物质的收入和支出，我想应该没人比我需要谨慎行事了。
	option 100 #打开仓库
	option 200 #打开公会资源中心
	talk
endproc
proc 100
	 OPEN_UI 0
endproc
proc 200
	OPEN_UI 47
endproc
