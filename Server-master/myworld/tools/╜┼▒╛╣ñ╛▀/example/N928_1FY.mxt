;龙城NPC“绿色女神”
;158 “春之雨水”任务
;
PROC 1
		GET_TASK_STATUS 158 208
		IF 208 = 0
			DIALOG 1
			ADD_TALK $_npc$:
			ADD_TALK 　　春天的甘露降临人间，给大地带来绿色的希望，请为我采集/C04五份春之雨露/C00，我需要用它们来灌溉幼嫩的小树苗。
			OPTION 2 乐意为您效劳！
			OPTION 3 我暂时没有时间。
			TALK
			RETURN
		ENDIF
		
		IF 208 = 2
			GET_TASK_VAR 158 0 209
			GET_TASK_VAR 158 1 210
			GET_TASK_VAR 158 2 211
			GET_TASK_VAR 158 3 212
			GET_TASK_VAR 158 4 213
			
			IF 209 = 1 and 210 = 1 and 211 = 1 and 212 = 1 and 213 = 1
				DIALOG 1
				ADD_TALK $_npc$:
				ADD_TALK 　　谢谢你为我收集雨露，有了雨露的浇灌，小树苗们很快就可以长成参天大树啦！
				OPTION 4 交出雨露
				TALK
			ELSE
				DIALOG 1
				ADD_TALK $_npc$:
				ADD_TALK 　　幼嫩的小树苗正等着灌溉呢，请快些采集来/C04五份春之雨露/C00。在龙城的/C05城主成隍、鬼谷先生、小姑娘莺莺、观音院主持无恙和小米/C00那里可以采集到。
				OPTION 5 #知道了
				TALK
			ENDIF 
			
			RETURN
		ENDIF
		
		IF 208 = 1
			GETNUM_GOODS 8806 997
			IF 997 = 0
				DIALOG 1
				ADD_TALK $_npc$:
				ADD_TALK 　　灌溉春天的/C10小树苗/C00，必须使用/C04花洒/C00，其它工具对小树苗可是无效的哦！
				OPTION 7	领取花洒
				OPTION 10	兑换99颗普通种籽。
				OPTION 11	兑换1颗金种籽。
				OPTION 5	#稍后再来
				TALK
			ELSE
				DIALOG 1
				ADD_TALK $_npc$:
				ADD_TALK 　　西方的火焰岛原本也是大陆上的一片绿洲，从此大地干裂，草木枯死，变成了一片荒漠，为了让火焰岛重新拥有绿色的植被，我正等着你为我带来新鲜的种籽。每/C0499颗/C00普通种籽可兑换一次奖励，/C04金种籽/C00则是每一颗兑换一次。
				OPTION 10	兑换99颗普通种籽。
				OPTION 11	兑换1颗金种籽。
				OPTION 5	#暂时不兑换。
				TALK
			ENDIF
		ENDIF
	
ENDPROC

PROC 2
		GET_TASK_NUM 0  
		IF 0 >= 10
			DIALOG 3
      ADD_TALK /C04系统提示：/C00
      ADD_TALK /C04对不起，你有十个任务在进行中，无法再领取新的任务。/C00
      TALK 
		ELSE
			DIALOG 3
			ADD_TALK $_npc$:
			ADD_TALK 　　在龙城的五个居民那里可以采集到/C04春之雨露/C00，他们是/C05城主成隍、鬼谷先生、小姑娘莺莺、观音院主持无恙和小米/C00，幼嫩的小树苗正等着你采集的/C04春之雨露/C00呢！
			TALK
			ACCEPT_TASK 158 1
			ADD_TASK_HINT 158 01
		ENDIF
ENDPROC

PROC 3
		DIALOG 3
		ADD_TALK $_npc$:
		ADD_TALK 　　幼嫩的小树苗正等着春之雨露的灌溉，也许你下一次有时间。
		TALK
		RETURN
ENDPROC

PROC 4
		GETNUM_GOODS 12017 997
		DEL_GOODS 12017 @997 1
		
		IF 1 = 0			
			ADD_HONOR 10
			START_SCREENMSG
			ADD_HONOR_SCREENMSG 10
			SHOW_SCREENMSG   
			FINISH_TASK 158
				
			DIALOG 1
			ADD_TALK $_npc$:
			ADD_TALK 　　西方的火焰岛原本也是大陆上的一片绿洲，五百年前的东海大战中，哪吒大仙用烽火轮燃起巨火将四海之水蒸干，残留的火星飘落到火焰岛上，燃烧起熊熊烈焰，从此大地干裂，草木枯死，只剩下一片荒漠，为了让火焰岛重新拥有绿色的植被，需要大量的树种。
			OPTION 6 请继续
			TALK
	
		ELSE
			DIALOG 3
			ADD_TALK $_npc$:
			ADD_TALK 　　咦？你身上的雨露怎么不见啦？
			TALK
		ENDIF
ENDPROC

PROC 5
		RETURN
ENDPROC

PROC 6
		DIALOG 1
		ADD_TALK $_npc$:
		ADD_TALK 　　在神州岛和瀛洲岛的野外，有许多春天新生的/C10小树苗/C00。请用这把/C04花洒/C00的雨露浇灌它们，等小树苗长大后，它们会赐给你神奇的/C04种籽/C00。
		ADD_TALK 　　每当你收集齐/C0499颗/C00的时候，就可以拿到我这里来兑换丰厚的奖励。如果运气好的话，你可能会采集到非常稀少的/C04金种籽/C00，交给我，更会得到超值的奖励哦！
		OPTION  7 领取花洒
		OPTION	5 #知道了
		TALK
ENDPROC

PROC 7
		ADD_GOODS 8806 1 1
		IF 1 = 0
			DIALOG 1
			ADD_TALK $_npc$:
			ADD_TALK 　　春天新生的/C10小树苗/C00分布在神州岛和瀛洲岛的野外，快去用花洒浇灌它们吧，其它的工具对小树苗可是无效的哦！祝你好运！
			OPTION 5 #知道了
			TALK
		ELSE
			DIALOG 1
			ADD_TALK $_npc$:
			ADD_TALK 　　你身上的东西太多了，放不下花洒，请先清理一下背包然后再来领取吧！
			OPTION 5 #知道了
			TALK
		ENDIF
ENDPROC

PROC 10
		GET_LEVEL 5
		GETNUM_GOODS 12010 997
		GET_VAR 48 16 998
		IF 997 < 99
			DIALOG 1
			ADD_TALK $_npc$:
			ADD_TALK 　　对不起，你身上没有兑换要求的足够数量的种籽。
			OPTION 5 #稍后再来。
			TALK
		ELSE
			SKIP_CHECK
			DEL_GOODS 12010 99 1
			IF 1 != 0
				DIALOG 3
				ADD_TALK $_npc$:
				ADD_TALK 　　删除物品失败，请稍后再试！
				TALK
				RETURN
			ELSE
				;赠送经验和物品
				DIALOG 3
				ADD_TALK $_npc$:
				ADD_TALK 　　恭喜您，兑换成功了！再去收集多一些种子吧！
				TALK
				IF 998 >= 0 and 998 <= 395
					IF 5 >= 20
						CALL 21
					ELSE
						CALL 20
					ENDIF
					
					IF 998 = 0
						SET_RANK 5606 0 3600
					ENDIF
					MSG_PLAYER 3 2 环保小卫士经验加成，获得一些经验！
				ENDIF
				
				IF 998 >= 396 and 998 <= 890 
					IF 5 >= 20
						CALL 22
					ELSE
						CALL 20
					ENDIF
					
					IF 998 = 396
						SET_RANK 5607 0 3600
					ENDIF
					MSG_PLAYER 3 2 护林小天使经验加成，获得更多经验！
				ENDIF
				
				IF 998 >= 891 and 998 <= 1385
					IF 5 >= 20
						CALL 23
					ELSE
						CALL 20
					ENDIF
					
					IF 998 = 891
						SET_RANK 5608 0 3600
					ENDIF
					MSG_PLAYER 3 2 绿色守护者经验加成，获得很多经验！
				ENDIF
				
				IF 998 >= 1386
					IF 5 >= 20
						CALL 24
					ELSE
						CALL 20
					ENDIF
					
					IF 998 = 1386
						SET_RANK 5609 0 3600
					ENDIF
					MSG_PLAYER 3 2 春之精灵经验加成，获得非常多经验！
				ENDIF
				
				;累积种子数目
				ADD 998 99
				SET_VAR 48 16 @998
				RETURN 
			ENDIF
		ENDIF
		
ENDPROC

PROC 11
		GETNUM_GOODS 12011 997
		IF 997 < 1
			DIALOG 1
			ADD_TALK $_npc$:
			ADD_TALK 　　对不起，你身上没有金种籽。
			OPTION 5 #稍后再来
			TALK
		ELSE
			DIALOG 1
			ADD_TALK $_npc$:
			ADD_TALK 　　一次只能兑换一个金种籽。
			OPTION 12 #开始兑换。
			TALK
		ENDIF
ENDPROC

PROC 12
		SKIP_CHECK
		DEL_GOODS 12011 1 1
		IF 1 = 0
			GET_LEVEL 5
			IF 5 >= 20	
				GET_LEVEL 4
				MUL 4 @4
				GET_LEVEL 7      
				MUL 4 @7
				MUL 4 3  
				GET_LEVEL 5
				MUL 5 @5
				MUL 5 830
				GET_LEVEL 6
				MUL 6 5950
				SUB 5 @6 
				SUB 5 @4              
				ADD 5 21644
				DEV 5 20
				MUL 5 3
				ADD_EXP @5
				START_SCREENMSG
				ADD_EXP_SCREENMSG @5
				SHOW_SCREENMSG
			ELSE
				CALL 25
			ENDIF
		ELSE
			MSG_PLAYER 3 2 兑换失败！请稍后再试！
		ENDIF
ENDPROC


PROC 20
		SKIP_CHECK
		GET_LEVEL 4
		MUL 4 @4
		GET_LEVEL 7      
		MUL 4 @7
		MUL 4 3  		
		GET_LEVEL 5
		MUL 5 @5
		MUL 5 830		
		GET_LEVEL 6
		MUL 6 5950		
		SUB 5 @6 
		SUB 5 @4              
		ADD 5 21644
		DEV 5 20
		MUL 5 3				     
		ADD_EXP @5
		START_SCREENMSG
		ADD_EXP_SCREENMSG @5
		SHOW_SCREENMSG
ENDPROC

PROC 21
		SKIP_CHECK
		GET_LEVEL 4
		MUL 4 @4
		GET_LEVEL 7      
		MUL 4 @7
		MUL 4 3  
		GET_LEVEL 5
		MUL 5 @5
		MUL 5 830
		GET_LEVEL 6
		MUL 6 5950
		SUB 5 @6 
		SUB 5 @4              
		ADD 5 21644
		MUL 5 3
		DEV 5 5
		ADD_EXP @5		
		START_SCREENMSG
		ADD_EXP_SCREENMSG @5
		SHOW_SCREENMSG
ENDPROC

PROC 22
		SKIP_CHECK
		GET_LEVEL 4
		MUL 4 @4
		GET_LEVEL 7      
		MUL 4 @7
		MUL 4 3  
		GET_LEVEL 5
		MUL 5 @5
		MUL 5 830
		GET_LEVEL 6
		MUL 6 5950
		SUB 5 @6 
		SUB 5 @4              
		ADD 5 21644
		MUL 5 7
		DEV 5 10
		ADD_EXP @5
		START_SCREENMSG
		ADD_EXP_SCREENMSG @5
		SHOW_SCREENMSG
ENDPROC

PROC 23
		SKIP_CHECK
		GET_LEVEL 4
		MUL 4 @4
		GET_LEVEL 7      
		MUL 4 @7
		MUL 4 3  
		GET_LEVEL 5
		MUL 5 @5
		MUL 5 830
		GET_LEVEL 6
		MUL 6 5950
		SUB 5 @6 
		SUB 5 @4              
		ADD 5 21644
		ADD_EXP @5	
		START_SCREENMSG
		ADD_EXP_SCREENMSG @5
		SHOW_SCREENMSG
ENDPROC

PROC 24
		SKIP_CHECK
		GET_LEVEL 4
		MUL 4 @4
		GET_LEVEL 7      
		MUL 4 @7
		MUL 4 3  
		GET_LEVEL 5
		MUL 5 @5
		MUL 5 830
		GET_LEVEL 6
		MUL 6 5950
		SUB 5 @6 
		SUB 5 @4              
		ADD 5 21644
		ADD_EXP @5
		START_SCREENMSG
		ADD_EXP_SCREENMSG @5
		SHOW_SCREENMSG
ENDPROC

PROC 25
		SKIP_CHECK
		GET_LEVEL 4
		MUL 4 @4
		GET_LEVEL 7      
		MUL 4 @7
		MUL 4 3  
		GET_LEVEL 5
		MUL 5 @5
		MUL 5 830
		GET_LEVEL 6
		MUL 6 5950
		SUB 5 @6 
		SUB 5 @4              
		ADD 5 21644
		DEV 5 20
    MUL 5 3
		ADD_EXP @5
		START_SCREENMSG
		ADD_EXP_SCREENMSG @5
		SHOW_SCREENMSG
ENDPROC