;贝德罗
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b需要购买种子和开垦土地吗，就来找我吧！	
	add_talk #b#b只需要花费银币就可以使你的菜园变得更大，种植更多的植被！
	add_talk #b#b你要开垦这块土地，让你的菜园更广阔吗？
	
		option 100 开垦菜园
	    OPTION 110 #(买卖)购买种子
		OPTION 120 #查询家园信息
	talk
endproc 

proc 100
	home_kaiken 0
endproc

proc 110
	OPEN_NPC_SHOP
endproc

proc 120
	strvar Nleve Ntudi Nqian leve ret ret1 ret2 ret3
	home_info ret2 1 Nleve
	
	if ret2 = -1
		RETURN
	endif
	
	home_info ret3 2 Ntudi
	home_info ret 3 Nqian
	home_info ret1 5 leve
	
	DIALOG 1
	add_talk #b#b菜地等级：$@Nleve$
	add_talk #b#b已开垦土地：$@Ntudi$
	add_talk #b#b
	
	if ret = 0
		add_talk #b#b下次开垦花费：$@Nqian$
	else
		add_talk #b#b#c005ebe土地已被全数开垦！#n
	endif
	
	if ret1 = 0
		add_talk #b#b下次开垦需要菜地等级：$@leve$
	else
	endif
	
		option 1 返回
	talk
endproc
