;30级副本
;
;判断死亡后刷24只骷髅和30只蝙蝠,判断死亡后刷吸血鬼
;判断死亡后刷30只幽魂,判断死亡后刷黑暗魔法师

PROC 1					;创建副本事件

	add_sys_msg_start
	add_sys_msg 你的到来，打破了这座破落城堡死一般地寂静，也唤醒了这群迷失教徒的狂热！
	set_sys_msg_mode 4 0 0
	map_msg_info
	

	var 10 0			;设置变量起始值
	set_pworld_var 0 32 @10 1	;设置副本变量
	var 11 0
	set_pworld_var 4 32 @11 1	;16怪物实例id，15boss死亡标记，11、12计数，13阶段控制,14怪物id,22蝙蝠实例id.23返回标志,24康斯坦丁实例id
	VAR 12 0
	set_pworld_var 8 32 @12 1
	VAR 13 0
	set_pworld_var 16 32 @13 1
	var 14 0
	set_pworld_var 12 32 @14 1
	VAR 15 0
	set_pworld_var 52 32 @15 1
	VAR 103 0
	set_pworld_var 32 32 @103 1
	VAR 104 0
	set_pworld_var 64 32 @104 1
	VAR 105 0
	set_pworld_var 36 32 @105 1
	VAR 106 0
	set_pworld_var 40 32 @106 1
	VAR 107 1
	set_pworld_var 44 32 @107 1
	VAR 108 1
	set_pworld_var 48 32 @108 1
	var 16 0
	set_pworld_var 56 32 @16 1
	var 17 1
	set_pworld_var 60 32 @17 1
	var 18 0
	set_pworld_var 68 32 @18 1
	var 20 0
	set_pworld_var 76 32 @20 1
	var 21 0
	set_pworld_var 80 32 @21 1
	var 22 0
	set_pworld_var 84 32 @22 1
	var 23 0
	set_pworld_var 88 32 @23 1
	var 24 0
	set_pworld_var 92 32 @24 1
	var 30 0
	set_pworld_var 100 32 @30 1     ;阻挡怪2个
	var 31 0
	set_pworld_var 104 32 @31 1



		NEW_TMP_ITEM 1400003 8948 16445 0 0 0 0 1		;手雷
		NEW_TMP_ITEM 1440000 8027 16123 0 0 0 0 1		;包子	
		NEW_NPC 19 306015 9153 16822 47 jack.mac
		set_pworld_var 72 32 @19 1
		NEW_NPC 24 306018 7656 10614 39 ozkangsitanding.mac
		mon_speech @24 2 2 1 黑暗终究会过去，阳光将再次照耀大地之上，崭新的一天即将到来！

		
		;NEW_MON 284010 1 11486 18553 0 39 0 1 2
		;NEW_MON 284008 4 11486 18553 10 39 0 1 2
		;NEW_MON 284010 1 13063 16552 0 40 0 1 2
		;NEW_MON 284008 4 13063 16552 10 40 0 1 2
		;NEW_MON 284010 1 15171 17907 0 41 0 1 2
		;NEW_MON 284008 4 15171 17907 10 41 0 1 2
		;NEW_MON 284010 1 14145 19407 0 42 0 1 2
		;NEW_MON 284008 4 14145 19407 10 42 0 1 2
		NEW_MON 230014 4 6990 15185 0 0 0 1 2
		NEW_MON 230014 4 5875 15781 0 0 0 1 2
		NEW_MON 230014 4 6760 17562 0 0 0 1 2
		NEW_MON 230014 4 8776 18074 0 0 0 1 2
		NEW_MON 230014 4 8642 15596 0 0 0 1 2


		NEW_MON_BY_DIR 201063 1 9900 18100 0 0 0 40 1 30
		set_pworld_var 100 32 @30 1
		NEW_MON_BY_DIR 201063 1 18800 10600 0 0 0 0 1 31
		set_pworld_var 104 32 @31 1



ENDPROC

PROC 3
	get_pworld_var 0 32 1 10
	get_pworld_var 4 32 1 11
	get_pworld_var 8 32 1 12
	get_pworld_var 16 32 1 13
	get_pworld_var 52 32 1 15
	get_pworld_var 36 32 1 105
	get_pworld_var 40 32 1 106
	;set_pworld_var 76 32 @20 1
	;set_pworld_var 80 32 @21 1
	;get_pworld_var 76 32 1 20
	;get_pworld_var 80 32 1 21
	;GET_PWORLD_VAR 60 32 1 17
	IS_PWORLD_OVER 2
	if @2 = 0
	add 11 1
	set_pworld_var 4 32 @11 1
	endif
	;add_sys_msg_start
	;add_sys_msg 17=$@17$,12=$@12$,20=$@20$,21=$@21$,106=$@106$
	;set_sys_msg_mode 0 0 0
	;map_msg_info




	if 11 = 1 and 13 = 0

		;NEW_MON 284010 1 11486 18553 0 39 0 1 2
		;NEW_MON 284008 1 11386 18553 0 41 0 1 2
		NEW_MON 284008 1 11586 18553 0 42 0 1 2
		NEW_MON 284008 1 11486 18653 0 40 0 1 2
		NEW_MON 284008 1 11486 18453 0 44 0 1 2
		var 13 1
		set_pworld_var 16 32 @13 1
	endif
	if 11 = 11 and 13 = 1
		;NEW_MON 284010 1 11486 18553 0 39 0 1 2
		;NEW_MON 284008 1 11386 18553 0 41 0 1 2
		NEW_MON 284008 1 11586 18553 0 42 0 1 2
		NEW_MON 284008 1 11486 18653 0 40 0 1 2
		NEW_MON 284008 1 11486 18453 0 44 0 1 2
		var 13 2
		set_pworld_var 16 32 @13 1
	endif

	if 11 = 21 and 13 = 2
		;NEW_MON 284010 1 11486 18553 0 39 0 1 2
		;NEW_MON 284008 1 11386 18553 0 41 0 1 2
		NEW_MON 284008 1 11586 18553 0 42 0 1 2
		NEW_MON 284008 1 11486 18653 0 40 0 1 2
		NEW_MON 284008 1 11486 18453 0 44 0 1 2
		var 13 3
		set_pworld_var 16 32 @13 1
	endif


	if 11 = 31 and 13 = 3
		;NEW_MON 284010 1 11486 18553 0 39 0 1 2
		;NEW_MON 284008 1 11386 18553 0 41 0 1 2
		NEW_MON 284008 1 11586 18553 0 42 0 1 2
		NEW_MON 284008 1 11486 18653 0 40 0 1 2
		NEW_MON 284008 1 11486 18453 0 44 0 1 2
		var 13 4
		set_pworld_var 16 32 @13 1
	endif

	if 11 = 41 and 13 = 4
		;NEW_MON 284010 1 11486 18553 0 39 0 1 2
		;NEW_MON 284008 1 11386 18553 0 41 0 1 2
		NEW_MON 284008 1 11586 18553 0 42 0 1 2
		NEW_MON 284008 1 11486 18653 0 40 0 1 2
		NEW_MON 284008 1 11486 18453 0 44 0 1 2
		var 13 5
		set_pworld_var 16 32 @13 1
	endif

		GET_PWORLD_VAR 60 32 1 17
		GET_PWORLD_VAR 68 32 1 18
		if 17 = 2
			add 18 1
			SET_PWORLD_VAR 68 32 @18 1
			GET_PWORLD_VAR 68 32 1 18
			if 18 = 1
			add_sys_msg_start
			add_sys_msg 杰克脸上现出痛苦的神情，恐怕已经来不及了
			set_sys_msg_mode 4 0 0
			map_msg_info
			get_pworld_var 72 32 1 19
			EFFECT_ID 0 @19 playMontion(%d,"282013_18")
			endif
			if 18 = 2
			DEL_NPC 306015 2
			NEW_MON_BY_DIR 282013 1 9153 16822 0 0 0 47 1 16	;新月狼王
			set_pworld_var 56 32 @16 1
			get_pworld_var 56 32 1 16
			TARGET_ADD_STATUS 3 @16 1095 1 7200 2
			var 17 3
			set_pworld_var 60 32 @17 1
			endif
		endif

		GET_PWORLD_VAR 40 32 1 106
		GET_PWORLD_VAR 44 32 1 107
		get_pworld_var 16 32 1 13
		if 107 = 2
			get_mon_num_by_defid 284004 20
			get_mon_num_by_defid 284005 21
			add 106 1
			SET_PWORLD_VAR 40 32 @106 1
			GET_PWORLD_VAR 40 32 1 106
			;加速刷怪
			if 106 > 8 and 106 < 18 and 20 = 0 and 21 = 0
				var 106 18
				SET_PWORLD_VAR 40 32 @106 1
			endif
			if 106 > 21 and 106 < 31 and 20 = 0 and 21 = 0
				var 106 31
				SET_PWORLD_VAR 40 32 @106 1
			endif

			if 106 = 5
			add_sys_msg_start
			add_sys_msg 吸血鬼伯爵闻到新鲜的人类气息，派出了蝙蝠部队
			set_sys_msg_mode 4 0 0
			map_msg_info
			NEW_MON 284004 2 19035 11475 200 43 0 1 22
			set_pworld_var 84 32 @22 1
			endif

			if 106 = 8
				NEW_MON 284004 3 19035 11475 200 43 0 1 2
			endif

			if 106 = 18
				get_pworld_var 84 32 1 22
				GET_MON_HP_PERCENT @22 23
				set_pworld_var 88 32 @23 1
				get_pworld_var 88 32 1 23
				if 23 > 0
					sub 106 1
					SET_PWORLD_VAR 40 32 @106 1
				else
				add_sys_msg_start
				add_sys_msg 吸血鬼伯爵派出了第二支蝙蝠部队
				set_sys_msg_mode 4 0 0
				map_msg_info
				;NEW_MON 284004 2 17685 11799 400 45 0 1 2
				NEW_MON 284005 1 17685 11799 400 45 0 1 22
				set_pworld_var 84 32 @22 1
				endif
			endif
			
			if 106 = 21
				NEW_MON 284004 2 17685 11799 400 45 0 1 2
				NEW_MON 284005 1 17685 11799 400 45 0 1 2
			endif

			if 106 = 31
				get_pworld_var 84 32 1 22
				GET_MON_HP_PERCENT @22 23
				set_pworld_var 88 32 @23 1
				get_pworld_var 88 32 1 23
				if 23 > 0
					sub 106 1
					SET_PWORLD_VAR 40 32 @106 1
				else
				add_sys_msg_start
				add_sys_msg 吸血鬼伯爵派出了最后一支蝙蝠部队
				set_sys_msg_mode 4 0 0
				map_msg_info
				NEW_MON 284004 2 21271 12389 400 46 0 1 2
				NEW_MON 284005 1 21271 12389 400 46 0 1 22
				set_pworld_var 84 32 @22 1
				endif
			endif

			if 106 = 34
				NEW_MON 284004 2 21271 12389 400 46 0 1 2
				NEW_MON 284005 1 21271 12389 400 46 0 1 2
			endif

			if 106 = 40
				get_pworld_var 84 32 1 22
				GET_MON_HP_PERCENT @22 23
				set_pworld_var 88 32 @23 1
				get_pworld_var 88 32 1 23
				if 23 > 0
					sub 106 1
					SET_PWORLD_VAR 40 32 @106 1
				else
				add_sys_msg_start
				add_sys_msg 吸血鬼伯爵亲自出动了
				set_sys_msg_mode 4 0 0
				map_msg_info
				NEW_MON_BY_DIR 282011 1 19035 11475 0 0 0 95 1 16
				set_pworld_var 56 32 @16 1
				get_pworld_var 56 32 1 16
				TARGET_ADD_STATUS 3 @16 1092 1 7200 2
				TARGET_ADD_STATUS 3 @16 1095 1 7200 2
				var 107 3
				var 106 0
				set_pworld_var 44 32 @107 1
				set_pworld_var 40 32 @106 1
				endif
			endif
		endif
		
		get_pworld_var 48 32 1 108
		if 108 = 2
			NEW_MON 284009 4 18238 4177 600 0 0 1 2
			NEW_MON 284009 4 18394 3278 600 0 0 1 2
			NEW_MON 284009 4 20218 3227 600 0 0 1 2
			NEW_MON 284009 4 20235 4340 600 0 0 1 2
			var 108 3
			set_pworld_var 48 32 @108 1
		endif

	GET_PWORLD_VAR 0 32 1 10		
	GET_PWORLD_VAR 36 32 1 105
	if 10 = 2
		add 105 1
		set_pworld_var 36 32 @105 1
		GET_PWORLD_VAR 36 32 1 105
			if 105 = 5
			GET_MAP_NPC_ID 306018 19 1
			mon_speech @19 2 2 1 做好准备，勇士们，我体内的黑暗力量要爆发了
			endif
			if 105 = 10
			add_sys_msg_start
			add_sys_msg 康斯坦丁将在10秒后转化成黑暗形态
			set_sys_msg_mode 4 0 0
			map_msg_info
			endif
			if 105 = 15
			del_npc 306018 105
			NEW_MON 282012 1 19248 3548 0 0 0 1 16
			set_pworld_var 56 32 @16 1
			get_pworld_var 56 32 1 16
			TARGET_ADD_STATUS 3 @16 1095 1 7200 2
			endif
	endif

	GET_PWORLD_VAR 0 32 1 10
	GET_PWORLD_VAR 36 32 1 105
	IF 10 = 3
                ADD 105 1
		set_pworld_var 36 32 @105 1
		GET_PWORLD_VAR 36 32 1 105
			if 105 = 1
			EFFECT_ID 0 0 setOverlayRegion("区域_2")
                        endif
			if 105 = 3
			new_npc 1 333019 19211 2508 0 zhu30out.mac
			pworld_over
			OPEN_PWORLD_AWARD_UI
			add_sys_msg_start
			add_sys_msg 副本己完成，可通过传送门离开
			set_sys_msg_mode 4 0 0
			map_msg_info
                        endif
	ENDIF
ENDPROC

PROC 4

	
	

	GET_MON_INFO 0 14 0 0 0 0 1 1
	set_pworld_var 12 32 @14 1
	get_pworld_var 52 32 1 15
	
	
	

	If @14 = 282011
		get_pworld_var 104 32 1 31
		DEL_ID_MON @31 0
		get_pworld_var 0 32 1 10
		if 10 < 1
		get_mon_info 1 282011 0 0 0 0 103 104
		add 103 100
		sub 104 200
		del_npc 306018 2
		NEW_NPC 24 306018 @103 @104 90 ozkangsitanding.mac
		mon_speech @24 2 2 1 我能感觉到腐朽随着亚库里斯而消失，但黑暗仍在！战斗仍将继续！
		EFFECT_ID 0 @24 playMontion(%d,"1204001")
		endif
	ADD 15 1
        set_pworld_var 52 32 @15 1
	endif

	If @14 = 282012
	get_pworld_var 0 32 1 10
	get_pworld_var 36 32 1 105
	add 15 1
	var 10 3
	var 105 0
	set_pworld_var 36 32 @105 1
	set_pworld_var 0 32 @10 1
	set_pworld_var 52 32 @15 1
	endif

        If @14 = 282013
		get_pworld_var 100 32 1 30
		DEL_ID_MON @30 0
		get_pworld_var 0 32 1 10
		if 10 < 1
		get_mon_info 1 282012 0 0 0 0 103 104
		add 103 100
		add 104 300
		del_npc 306018 2
		NEW_NPC 24 306018 @103 @104 39 ozkangsitanding.mac
		mon_speech @24 2 2 1 我听到狼王濒死的哀嚎，真是鼓舞人心！继续前进！胜利就在前方！
		EFFECT_ID 0 @24 playMontion(%d,"1204001")
		endif
	get_pworld_var 52 32 1 15
	ADD 15 1
	set_pworld_var 52 32 @15 1
	endif
	
	If @14 = 284009
	GET_PWORLD_VAR 8 32 1 12
	ADD 12 1
	set_pworld_var 8 32 @12 1
	GET_PWORLD_VAR 8 32 1 12
	GET_PWORLD_VAR 0 32 1 10
		if 12 >= 15 and 10 = 0
		var 10 1
		set_pworld_var 0 32 @10 1
		del_npc 306018 2
		NEW_NPC 2 306018 19248 3548 90 ozkangsitanding.mac
		endif
	endif


ENDPROC

PROC 5
	effect 2 0 playEffct(7200,11000,"effect\\entity\\9598_16.ent")
	GET_PWORLD_VAR 0 32 1 10
	if 10 = 3
	EFFECT_ID 0 0 setOverlayRegion("区域_2")
	endif

ENDPROC




