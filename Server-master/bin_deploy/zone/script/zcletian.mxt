
;乐天春节活动
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b新年好！在这美好的春节时期，我们是应该愉快地度过的，年轻的勇士，要和我一起打打年兽，来获得新年的祝福吗？
		
		task_add_option 3020 1 100
		task_add_option 3021 1 101
		task_add_option 3022 1 102
		task_add_option 3023 1 103
		task_add_option 3024 1 104
		task_add_option 3025 1 105
		task_add_option 3026 1 106
		
	
		option 150 我能帮上什么忙？
		option 151 #没有兴趣
	
	talk
endproc 

PROC 100
	task_do_option 3020 1
ENDPROC
PROC 101
	task_do_option 3021 1
ENDPROC
PROC 102
	task_do_option 3022 1
ENDPROC
PROC 103
	task_do_option 3023 1
ENDPROC
PROC 104
	task_do_option 3024 1
ENDPROC
PROC 105
	task_do_option 3025 1
ENDPROC
PROC 106
	task_do_option 3026 1
ENDPROC

proc 150
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b在每年的#c186506 1月27日-2月10日#n，年兽都会蜂拥而出并且对我们的世界进行大肆破坏，这个时候需要各位勇士的帮助，一起消灭年兽，来得到新一年的祝福！完成我给予的任务将会得到福袋奖励！
	add_talk #b#b1月27日-2月10日期间可参加以下活动帮助我：
	add_talk #b#b#c186506活动1：完成每日乐天发布的年兽击杀任务
	add_talk #b#b活动2：完成每日乐天发布的永夜谷、黄金矿井、森林结界的其中一个击杀任务（普通难度及以上）
	add_talk #b#b活动3：10:00-20:00，击杀在银风雪原、流金沙漠、落日荒野、越雾雨林、越雾甬道出现的年兽，年兽会每半个小时刷新一次
	
		option 1 返回
		option 151 #我知道了
	
	talk

endproc
