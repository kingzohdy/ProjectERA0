;npc海蚌

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b只有在阳光海滩的浅海里才能放养小海蚌，请记住放养海蚌的位置。
	add_talk #b#b#c186506每天第一次育蚌取珠总数量为：18个#n
	add_talk #b#b#c186506每天第二次育蚌取珠总数量为：9个#n
	add_talk #b#b#c186506每天第3—N次育蚌取珠总数为：6个#n
	add_talk #b#b在养海蚌的时间，您可以去在沙滩椅上晒日光浴，或者去参与航海大亨比赛，或者去解决少年帕克的烦恼，总之，省时又省事，不需要您付出太多的时间与精力。
	add_talk #b#b海蚌需要一段时间将沙子#c186506孕育成珍珠#n，这个过程#c186506需要10分钟#n。
	add_talk #b#b收获的珍珠，您可以去#c186506跟米尔斯交换成银币#n。
	
	option 100 #查看海蚌生长情况

	talk
endproc

proc 100
	OPEN_YANG_HAI_BANG_UI 1
endproc

