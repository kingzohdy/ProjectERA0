
;小奶哥
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b风萧萧兮易水寒，装备一去兮，不复还；账号空兮，银币光，仰天呼气兮，泪两行。
	add_talk #b#b<你怎么了？>
	add_talk #b#b#001#001#001#001#001#001我被盗号了，什么都木有了，装备、材料、银币、金币，都木有了！
	add_talk #b#b<呃，你是不是上了什么不健康网站呀？>
	add_talk #b#b#028#028本帅哥一向洁身自好，从不……等等，我好像想起来什么了，我昨天登陆了一个领奖网站，喏，就是不远处那个奖品发放员给的网站，然后我就被T下线了，密码也不正确了，等我再上来就是现在这样子了！
	
	strvar ret
	task_stat 30013 1 ret
	if ret = 2
		option 150 #那个奖品发放员一看就是骗子啊！
	endif
	
	
	talk
endproc

proc 150

	add_sys_msg_start
	add_sys_msg 找到可疑的奖品发放员，深入了解小果被盗的经过
	set_sys_msg_mode 3 0 0
	MSG_INFO
	get_task_var 30013 0 8 1 20
	var 20 1
	set_task_var 30013 0 8 @20 1

endproc


