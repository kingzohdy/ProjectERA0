;npc萝拉    生命在于运动

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我生平最喜欢做运动了，运动给我带来了无穷的乐趣，你愿意和我一起做运动吗？
	add_talk #b#b国庆期间，每天我都会举办两场“生命在于运动”活动，希望你能日复一日的来跟我一起做运动哟！
	add_talk #b#b
	add_talk #b#b#c186506活动时间：#n
	add_talk #b#b#b#b#c18650614：00 - 14：10#n
	add_talk #b#b#b#b#c18650619：00 - 19：10#n

	option 201 活动技巧
	option 200 #了解，我会准时来的！
	
	strvar rrRet
	get_role_var 57 8 rrRet 99
	if @rrRet = 0 and @99 = 1
		option 202 #领取奖励
	endif
		
	talk
endproc 

proc 200
	PLAYER_CLOSE_DIALOG1 0 0
endproc

proc 201
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b活动开始后，我会先讲解活动技巧
	add_talk #b#b正式开始后，我会指定某个动作表情
	add_talk #b#b你在#c186506聚光灯柱下做出相对应的动作表情#n即可
	add_talk #b#b每次做对一个动作表情，你就会获得奖励
	add_talk #b#b如果动作完美还有可能会获得女神的青睐
	add_talk #b#b倘若做错，当然也会受到点小小的惩罚
	
	option 1 明白了
	talk
endproc

proc 202
	strvar ret1
	GET_PACK_SPACE 0 ret1
	if @ret1 = 0
		add_sys_msg_start
		add_sys_msg 背包空间不足，无法领取
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	ADD_ITEM_NUM_INPAK 5043267 1 1 10
	if @10 = 0
		set_role_var 57 8 0 1
	endif
endproc
