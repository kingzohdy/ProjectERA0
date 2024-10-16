
;小白
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b伤不起啊，伤不起！
	add_talk #b#b#012#012#012#012#012#012
	add_talk #b#b<这位大哥，你怎么了？>
	add_talk #b#b我被骗子害的心力交瘁啊，现在连说话的力气都木有了啊，而且现在我一想起被骗的情况，就忍心不住泪流满面啊！
	add_talk #b#b听说蛋糕能让人变得高兴起来，不如你帮我买块奶油蛋糕来补充补充体力，调剂一下心情吧！
		
		strvar ret
		task_stat 30011 1 ret
		if ret = 2
			option 150 帮你教训骗子岂不是更好？
		endif
	talk
endproc

proc 150
	dialog 1
	add_talk $_npc$:
	add_talk #b#b嗝，难得这世界上还有好人，我就把我惨痛的经历讲给你听吧，希望你能吸取我的教训。
	add_talk #b#b话说那天我在时间城闲晃，突然看到有人在世界叫宠物“完美霸王龙”，属性很好很便宜，于是我马上密他了，然后我们就在时间城交易了。


		option 155 当面交易也能被骗？

	talk

endproc 
proc 155
	dialog 1
	add_talk $_npc$:
	add_talk #b#b第1次交易的时候，她放上的的确是“完美霸王龙”，我看了的确是完美资质，然后我就放了之前谈好的5W银币。
	add_talk #b#b接下来你猜怎么着？
	add_talk #b#b#012#012
	add_talk #b#b她突然取消交易了，并问我怎么没放钱上去，想白拿呀？当时我还在奇怪，我明明放了钱上去的！！
	add_talk #b#b然后我们又开始重新交易，她放上了个名字为“完美霸王龙”的宠物上来，而我也给了他5W银币，这次交易成功了。
	
		option 160 你不是收到霸王龙了，还哭什么？
	
	talk
endproc 
proc 160
	dialog 1
	add_talk $_npc$:
	add_talk #b#b这根本就不是我第一次看到的那个霸王龙，只是他们俩的名字是一样的，这实际上是一只名叫“完美霸王龙”的松鼠枪手！
	add_talk #b#b伤不起啊，伤不起！
	add_talk #b#b#012#012#012#012#012#012
	add_talk #b#b我直到现在才想明白，她之所第取消第一次交易就是为了要狸猫换太子！是我太大意了，第二次交易时没有看清楚！我万万没有想到，那么清纯可爱的小女孩竟然会骗我。
		
		option 161 #看我帮你讨个公道！
	talk
endproc 
proc  161

	add_sys_msg_start
	add_sys_msg 旁边有个正在叫卖完美霸王龙宠物的小女孩，去看看。
	set_sys_msg_mode 3 0 0
	MSG_INFO

	get_task_var 30011 0 8 1 20
	var 20 1
	set_task_var 30011 0 8 @20 1
	
endproc  














