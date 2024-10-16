;逆往之间（打强盗）  哈利  302043   副本ID：51500

proc 1
	DIALOG 1
	add_talk 眼疾手快诀窍： 
	add_talk #b#b快速分辨出好人（小朋友、路人），坏人（强盗、喽啰、大盗），击杀坏人，积攒分数。
	add_talk #b#b
	add_talk #c108650分数规则：
	add_talk #b#b#c108650小朋友    -3分
	add_talk #b#b#c108650路人      -5分
	add_talk #b#b#c108650强盗      +5分
	add_talk #b#b#c108650喽啰      +3分
	add_talk #b#b#c108650大盗      +6分#n
	add_talk #b#b
	add_talk 胜利条件：击杀强盗，积攒的分数≥100
	
	strvar sta1 sta4 ret
	TASK_STAT 4615 ret 11
	if @11 = 2 and @ret = 0
		GET_PWORLD_VAR 0 32 20 sta1
		if @sta1 = 0
			option 200 #开始挑战
		endif	
	endif

	option 201 #谢谢你的指导
	
	talk
endproc 

proc 200
	;0位表示开始刷怪与否的标志   设置值为1表示开始挑战选项不出现了
	set_pworld_var 0 32 1 1
	add_sys_msg_start
	add_sys_msg 当你眼前有人影闪过时，快速分辨好人与坏人，击杀坏人得分！
	set_sys_msg_mode 4 0 0
	msg_info
	
	;分数清0
	set_pworld_var 8 32 0 1
endproc

proc 201
	PLAYER_CLOSE_DIALOG1 0 0
endproc

