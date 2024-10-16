;NPC 脚本 - 传送到海滩地图

;NPC 脚本默认执行事件
proc 1		
	dialog 1
	add_talk $_npc$:
	;2 获取用户等级
	GET_PLAYER_BASE_ATT 0 1 2 3
	if @2 >= 50
		add_talk #b#b 嗨，#Gender2#，跟我走吧，现在就出发！开始一场幸福之旅吧。
		add_talk #b#b#c186506每天晒1小时日光浴，经验多多哟#n~#010~#002
		option 100	晒日光浴注意事项
		option 300 #我要进入阳光海滩
		option 500 #我是来打酱油的
	else
		add_talk #b#b 嗨，小#Gender2#，你也想去阳光海滩？哦，NONONO~你现在还太小了，等你到50级的时候再来吧！#006
	endif
	
	
	task_stat 4461 1 20
	if 20 = 2
		option 600 #您忠诚的铁杆粉丝希望拥有您的美丽留念
	endif
	
	talk
endproc

;接取任务成功触发的事件
proc 2

endproc

;NPC 交任务触发事件
proc 3

endproc

proc 100
	dialog 1
	add_talk $_npc$:
	add_talk 晒日光浴注意事项:
	add_talk #b#b 1.只要在阳光海滩场景内，就会获得经验；
	add_talk #b#b 2.每人每天有60分钟的多倍日光浴经验，只有在“沙滩”附近才可享受；
	add_talk #b#b 3.你获得的经验和你的等级有关，等级越高获得的经验越多；
	add_talk #b#b 4.除等级以外，特定的时装、装备和道具都会影响经验加成；
	add_talk #b#b 5.小心螃蟹；
	option 1 返回
	talk
endproc

proc 300
	transmit 4403 29843 9300 1
	add_sys_msg_start
	add_sys_msg 欢迎来到阳光海滩，美丽的景致让您进入心旷神怡的状态！
	set_sys_msg_mode 6 0 0
	msg_info
endproc 

proc 500
	return
endproc
proc 600

	add_item_num_inpak 5301006 1 1 0

endproc 
