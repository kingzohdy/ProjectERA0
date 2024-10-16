
;小朱
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b555555555555555555555……同样是人，差距咋就那么大呢！
	add_talk #b#b<你怎么了？>
	add_talk #b#b#059#059#059#059#059#059我来这里做活动，被一个可恶的PK狂杀啦，55555，他杀了我好多次啊！
	add_talk #b#b<那你为什么还在这呢？>
	add_talk #b#b#047#047我要盯着PK狂，如果遇见好心人要替我报仇，我好告诉他PK狂去哪了呀！

	strvar ret
	task_stat 30012 1 ret
	if ret = 2
		option 150 #告诉我PK狂的方位吧，我替你报仇！
	endif
	talk
endproc
proc 150
	
	strvar num
	GET_MAP_MON_NUM 4400 241008 1 num 1
;	GET_MON_NUM 1 num
	if num = 0
	;	NEW_MON 241008 1 43100 37800 0 0 0 0 0
		strvar lv
		GET_PLAYER_BASE_ATT 1 1 lv 1
		
		NEW_LIFE_DYN_MON 1 241008 1 40000 38100 0 0 0 0 0 @lv 0 180
		add_sys_msg_start
		add_sys_msg pk狂在东边不远处出现！
		set_sys_msg_mode 3 0 0
		msg_info
	else
		add_sys_msg_start
		add_sys_msg pk狂已经在东边不远处等候多时了。。。。。
		set_sys_msg_mode 3 0 0
		msg_info
	
	endif
endproc 

