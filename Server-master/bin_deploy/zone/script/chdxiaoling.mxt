
;小白
proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b矮油，你是巨人么，为什么这么高呀，我都看不到你的脑袋了，你站远点，让我瞅瞅你脑袋上面是什么？
	add_talk #b#b妈妈说，比我高的要叫叔叔阿姨，长头发的是阿姨，短头发的是叔叔，你是叔叔还是阿姨呢？我看不到你的脑袋，要不你抱我上去看看吧。

		task_add_option 3316 1 100
	
		option 150 我是超级小天才，什么问题都能答上来！
		
		strvar num
		GET_ITEM_NUM_INPAK 5052208 1 num
		task_stat 3316 1 20
		if 20 = 2 and num = 0
		;	option 155 #我不小心把无相斗篷弄丢了，能再给我一个吗？
		endif
			OPTION 110 #打开商店
	talk
endproc
PROC 100
	task_do_option 3316 1
ENDPROC
proc 110

	OPEN_NPC_SHOP 
endproc 

proc 155
	ADD_ITEM_NUM_INPAK 5052208 1 1 0

endproc 


proc 150
	dialog 1
	add_talk $_npc$:
	add_talk #b#b我可不会白回答问题，你得付钱，妈妈说书中自有黄金屋！这黄金屋当然不是书给我的，书是死的怎么可能给我钱呢！
	add_talk #b#b你问我问题，我回答，你付钱，我攒黄金屋，嗯嗯，就是这样！

;	add_talk #b#b我的价格很公道，#c1865061000绑银一个问题！#n

		option 151 问什么问题好呢？
	talk
endproc 
proc 151
	
	dialog 1
	add_talk $_npc$:
;	add_talk #b#b我可不会白回答问题，你得付钱，妈妈说书中自有黄金屋！这黄金屋当然不是书给我的，书是死的怎么可能给我钱呢！
;	add_talk #b#b你问我问题，我回答，你付钱，我攒黄金屋，嗯嗯，就是这样！
;	add_talk #b#b	
	add_talk #b#b我的价格很公道，#c1865061000绑银一个问题！#n

		option 210 为什么小树不会走路呢？
		option 211 吃包子时，包子为什么会流油呢？
		option 212 为什么要下雨呢？
		option 213 为什么雨点往下掉，不往上掉呢？
		option 214 雨为什么又停了呢？
		option 215 为什么会打雷呢？
		option 216 月亮为什么有时胖，有时瘦呢？
		option 217 大海为什么不停地喊呢？
		option 218 什么是海啸呢？
		option 219 人为什么要坐飞机呢？
		option 220 为什么地上的飞机大，天上的飞机小呢？
		option 221 风筝为什么飞不远？
		option 222 为什么许多字我不认识呢？
		option 223 为什么小朋友坐车不用买票呢？
		option 224 为什么会有黑夜呢？
		option 225 爸爸为什么爱看电视上的足球赛？
		option 226 大熊猫为什么走路都慢腾腾的？
		option 227 长颈鹿脖子怎么那么长？
		option 228 人为什么有两只耳朵呢？
	
	talk
endproc 

proc 200
	ADD_SYS_MSG_START
	ADD_SYS_MSG 要先付钱，才能问问题哟！小玲可不想被吃霸王餐！
	SET_SYS_MSG_MODE 3 0 0
	MSG_INFO

endproc 
proc 209
endproc 

proc 210
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 1000
		choose 1 160 209 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 160
	endif
endproc 
proc 211
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 1000
		choose 1 161 209 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 161
	endif
endproc 
proc 212
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 1000
		choose 1 162 209 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 162
	endif
endproc 
proc 213
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 1000
		choose 1 163 209 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 163
	endif
endproc 
proc 214
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 1000
		choose 1 164 209 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 164
	endif
endproc 

proc 215
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 1000
		choose 1 165 209 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 165
	endif
endproc 
proc 216
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 1000
		choose 1 166 209 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 166
	endif
endproc 

proc 217
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 1000
		choose 1 167 209 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 167
	endif
endproc 
proc 218
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 1000
		choose 1 168 209 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 168
	endif
endproc 

proc 219
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 1000
		choose 1 169 209 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 169
	endif
endproc 
proc 220
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 1000
		choose 1 170 209 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 170
	endif
endproc 

proc 221
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 1000
		choose 1 171 209 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 171
	endif
endproc 
proc 222
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 1000
		choose 1 172 209 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 172
	endif
endproc 
proc 223
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 1000
		choose 1 173 209 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 173
	endif
endproc 
proc 224
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 1000
		choose 1 174 209 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 174
	endif
endproc 
proc 225
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 1000
		choose 1 175 209 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 175
	endif
endproc 

proc 226
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 1000
		choose 1 176 209 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 176
	endif
endproc 

proc 227
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 1000
		choose 1 177 209 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 177
	endif
endproc 

proc 228
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 160 209 绑银消耗已达到每日最大上限，继续操作将直接消耗银币，是否继续操作？
	elseif ret < 1000
		choose 1 178 209 绑银不足，继续操作将直接消耗银币，是否继续操作？
	else
		call 178
	endif
endproc 






proc 160
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		
		dialog 1
		add_talk $_npc$:
		add_talk #b#b因为它只有一条腿，我有两条。
			
			option 151 返回
		talk
	endif
endproc 
proc 161
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b对不起，是我把它咬痛了，它哭了。

			option 151 返回
		talk
	endif
endproc 
proc 162
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b天空被乌云弄脏了，得洗一洗了。

			option 151 返回
		talk
	endif
endproc 
proc 163
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b因为往下掉有地面给接着，地面是它们的妈妈。

			option 151 返回
		talk
	endif
endproc 
proc 164
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b准是下累了。

			option 151 返回
		talk
	endif
endproc
proc 165
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b黑云脾气坏，爱吵架。

			option 151 返回
		talk
	endif
endproc 
proc 166
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b它有时听妈妈的话，有时淘气，不好好吃饭。

			option 151 返回
		talk
	endif
endproc 
proc 167
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b有的浪跑得太远，大海叫它们回来。

			option 151 返回
		talk
	endif
endproc 
proc 168
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b平时呢，是我们去看大海，海啸呢，就是大海来看我们。

			option 151 返回
		talk
	endif
endproc 
proc 169
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b因为人没有翅膀。

			option 151 返回
		talk
	endif
endproc 
proc 170
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b到天上，要像小鸟一样才飞得快。

			option 151 返回
		talk
	endif
endproc 
proc 171
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b有人扯住了它们的身子。

			option 151 返回
		talk
	endif
endproc 
proc 172
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b它们没告诉你它们的名字。

			option 151 返回
		talk
	endif
endproc 
proc 173
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b他们可以坐在妈妈的腿上。

			option 151 返回
		talk
	endif
endproc 
proc 174
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b晚上太阳要休息。

			option 151 返回
		talk
	endif
endproc 
proc 175
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b因为他自己不会踢。

			option 151 返回
		talk
	endif
endproc 
proc 176
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b跟它们的爸爸妈妈学的。

			option 151 返回
		talk
	endif
endproc 
proc 177
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b因为它们老想吃树顶上的叶子。

			option 151 返回
		talk
	endif
endproc 

proc 178
	strvar Moy
	DEC_MONEY_TEST 0 1000 21 Moy
	if Moy = -1
		call 200
	else
		DEC_MONEY 0 1000 21 1
		dialog 1
		add_talk $_npc$:
		add_talk #b#b可以一个耳朵进，一个耳朵出，光进不出就装不下了。

			option 151 返回
		talk
	endif
endproc 

















