

proc 1
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b悬赏通告
	add_talk #b
	add_talk #b#b为进一步净化怒风荒漠治安环境，确保社会秩序持续稳定发展，保障人民群众生命财产安全，推动经济发展，现决定对阿努比斯神殿中的数名（名单附后）重大危害公开悬赏通缉，特此通告。
	add_talk #b#b本通告自发布之日起施行。
	add_talk #b
	add_talk #b#b#b#b#b#b#b#b#b#b#b#b#b暖泉镇 梅里耶特拉
	add_talk #b
	add_talk #b#b附：重大危害名单及特性
	
	task_add_option 1591 1 101
	task_add_option 1592 1 102
	talk
endproc 


PROC 101
	task_do_option 1591 1
ENDPROC
PROC 102
	task_do_option 1592 1
ENDPROC
