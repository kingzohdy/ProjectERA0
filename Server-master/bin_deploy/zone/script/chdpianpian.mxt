
;小奶哥
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b完美霸王龙，极品宠物，超级便宜，#Gender3#，来看看嘛。
	add_talk #b#b<你的宠物怎么卖？>
	add_talk #b#b只要5W银币，你没听错，就是这么便宜，是完美资质哟！#003#003#003
	add_talk #b#b<只怕5W银币买到的是一只满大街都是的普通宠物吧？>
	add_talk #b#b#031#031#031呃，被拆穿了么？
	add_talk #b#b<你们这些可恶的骗子，竟然还装成可爱小女生来卖萌，欺骗纯情少男的心，今天就让我来教训教训你，看你以后还敢不也出来骗人！>
		
	
	get_task_var 30011 0 8 1 20
	task_stat 30011 1 30

	if 20 = 1 and 30 = 2
		option 150 #你们这些骗子，露出真面目吧！
	endif
	talk
endproc

proc 150
	strvar num
	GET_MAP_MON_NUM 4400 241007 1 num 1
;	GET_MON_NUM 1 num
	if num = 0
		NEW_MON 241007 1 35200 39300 0 0 0 0 0
		add_sys_msg_start
		add_sys_msg 骗术之所以能成功，是因为人的贪欲，只要你稍有想法，骗子就能长驱直入
		set_sys_msg_mode 3 0 0
		msg_info
	else
		add_sys_msg_start
		add_sys_msg 小心哟，你已经被游戏骗子盯上了，注意查看可疑的小女孩翩翩周围
		set_sys_msg_mode 3 0 0
		msg_info
	
	endif
endproc 
