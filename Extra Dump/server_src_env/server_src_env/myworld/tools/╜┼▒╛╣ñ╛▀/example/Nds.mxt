;开心果，兑换抽奖
;
;
PROC 1
 SKIP_CHECK  
 ;“开心果”只能在2006年3月23日00：00--2006年4月20日23：59进行兑换  
  CMP_TIME 1000 20060323000000 20060420235900
  IF 1000 = 0
         DIALOG 1
         ADD_TALK $_npc$：
         ADD_TALK 　　万物回春，Q哥哥Q妹妹们，向春天的田野进军吧！4月1、2、8、9、15、16日，每天下午14：30～16：30；晚上19：30～21：30魔牙活动期间，所有世界怪物有几率掉落魔牙，春之手环。
         OPTION 10 #1个魔牙换取宠物经验
         OPTION 20 #1个魔牙换取经验
         OPTION 30 #10个魔牙抽卡片
         OPTION 40 活动说明
         OPTION 50 #再见！
         TALK
  ENDIF
ENDPROC

;宠物经验值公式，：(-3*PET_LV*PET_LV*PET_LV+830*PET_LV*PET_LV-5950*PET_LV+21644)*27/700
PROC 10
  SKIP_CHECK 
            GET_PET_ATTR 3 0 4            
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
					      MUL 5 27
					      DEV 5 700 
					                
                ADD_PET_EXP @5 1            
               	IF 1 = 0   
							      MSG_PLAYER 2 2 你的宠物吸收了魔牙的能量，经验值获得了提升！
							      DEL_GOODS 12009 1 1
							  ELSE   
							      MSG_PLAYER 2 2 对不起，只有带在身边的宠物才能用魔牙提升经验！
							  ENDIF 
							  RETURN  
ENDPROC
    

;25级以上经验很少，25级以下换取经验很爽
;25级以上1颗魔牙换取经验值公式，：(-3*LV*LV*LV+830*LV*LV-5950*LV+21644)/100
;25级以下1颗魔牙换取经验值公式，：(-3*LV*LV*LV+830*LV*LV-5950*LV+21644)/50
PROC 20
  SKIP_CHECK 
  GET_LEVEL 8
  IF 8 >= 26      
      SKIP_CHECK  
      DEL_GOODS 12009 1 1
      IF 1 = 0
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
				      DEV 5 100 
				     
				      ADD_EXP @5
	         
			      MSG_PLAYER 2 2 你吸收了魔牙的能量，经验值获得了提升！！
	ELSE  
			      MSG_PLAYER 2 2 你没有足够的魔牙兑换！
	ENDIF 
  ELSE
     	 SKIP_CHECK  
	 DEL_GOODS 12009 1 1
		      IF 1 = 0
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
					      DEV 5 50 
					     
					      ADD_EXP @5
		         
				      MSG_PLAYER 2 2 你吸收了魔牙的能量，经验值获得了提升！！
			ELSE  
			              MSG_PLAYER 2 2 扣除魔牙失败！
		        ENDIF 			   
  ENDIF
ENDPROC


;本次活动抽取奖品失败后，不再补偿任何魔牙，80％中奖率。抽不到有文字提示你没有抽中奖品。
PROC 30
   RAND 9 100000
   IF 9 >= 0 and 9 < 20000 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         ADD_GOODS 3341 1 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ELSE
            MSG_PLAYER 2 2 你获得海盗装化妆卡一枚！ 
         ENDIF
      ENDIF  
   ENDIF 
   
   IF 9 >= 20000 and 9 < 40000 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         ADD_GOODS 3342 1 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ELSE
            MSG_PLAYER 2 2 你获得祭司装化妆卡一枚！ 
         ENDIF
      ENDIF  
   ENDIF 

   IF 9 >= 40000 and 9 < 60000 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         ADD_GOODS 3343 1 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ELSE
            MSG_PLAYER 2 2 你获得休闲装化妆卡一枚！ 
         ENDIF
      ENDIF  
   ENDIF 

   IF 9 >= 60000 and 9 < 70000 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         ADD_GOODS 3327 1 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ELSE
            MSG_PLAYER 2 2 你获得巨神卡一枚！ 
         ENDIF
      ENDIF  
   ENDIF 
   
   IF 9 >= 70000 and 9 < 70010 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9353 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF 

   IF 9 >= 70010 and 9 < 70020 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9338 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF 

   IF 9 >= 70020 and 9 < 70030 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9354 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF 

   IF 9 >= 70030 and 9 < 70040 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9356 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF 

   IF 9 >= 70040 and 9 < 70050 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9339 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70050 and 9 < 70060 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9355 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70060 and 9 < 70070 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9333 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70070 and 9 < 70080 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9348 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70080 and 9 < 70090 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9334 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70090 and 9 < 70100 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9344 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70100 and 9 < 70110 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9343 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70110 and 9 < 70120 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9349 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70120 and 9 < 70130 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9341 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70130 and 9 < 70140 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9345 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70140 and 9 < 70150 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9351 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70150 and 9 < 70160 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9335 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70160 and 9 < 70170 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9340 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70170 and 9 < 70180 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9350 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70180 and 9 < 70220 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         GET_LEVEL 8
         IF 8 <= 26  
         		APPLY_GIFT my 0 1 1022 1
         		IF 1 != 0 
          		MSG_PLAYER 2 2 你没有抽中奖品
          	  RETURN
						ENDIF
				 ELSE
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70220 and 9 < 70260 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         GET_LEVEL 8
         IF 8 <= 26  
         		APPLY_GIFT my 0 1 1122 1
         		IF 1 != 0 
          		MSG_PLAYER 2 2 你没有抽中奖品
          	  RETURN
						ENDIF
				 ELSE
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70260 and 9 < 70300 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         GET_LEVEL 8
         IF 8 <= 26  
         		APPLY_GIFT my 0 1 1521 1
         		IF 1 != 0 
          		MSG_PLAYER 2 2 你没有抽中奖品
          	  RETURN
						ENDIF
				 ELSE
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70300 and 9 < 70340 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         GET_LEVEL 8
         IF 8 <= 26  
         		APPLY_GIFT my 0 1 1621 1
         		IF 1 != 0 
          		MSG_PLAYER 2 2 你没有抽中奖品
          	  RETURN
						ENDIF
				 ELSE
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70340 and 9 < 70380 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         GET_LEVEL 8
         IF 8 <= 26  
         		APPLY_GIFT my 0 1 1821 1
         		IF 1 != 0 
          		MSG_PLAYER 2 2 你没有抽中奖品
          	  RETURN
						ENDIF
				 ELSE
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70380 and 9 < 70420 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         GET_LEVEL 8
         IF 8 <= 26  
         		APPLY_GIFT my 0 1 1923 1
         		IF 1 != 0 
          		MSG_PLAYER 2 2 你没有抽中奖品
          	  RETURN
						ENDIF
				 ELSE
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70420 and 9 < 70500 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         GET_LEVEL 8
         IF 8 <= 26  
         		APPLY_GIFT my 0 1 1321 1
         		IF 1 != 0 
          		MSG_PLAYER 2 2 你没有抽中奖品
          	  RETURN
						ENDIF
				 ELSE
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70500 and 9 < 70540 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 3331 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70540 and 9 < 70560 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9365 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70560 and 9 < 70580 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9366 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70580 and 9 < 80580 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE
         ADD_GOODS 3328 1 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
         ELSE
            MSG_PLAYER 2 2 你获得暗月卡一枚！ 
         ENDIF
      ENDIF  
   ENDIF 
   
   IF 9 >= 80580 and 9 < 100000
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 魔牙不足10枚，不能参加抽奖，活动时间内所有世界怪物都会掉落魔牙！
         RETURN 
      ELSE  	
          	MSG_PLAYER 2 2 你没有抽中奖品
          	RETURN
      ENDIF 
ENDIF 

ENDPROC



PROC 40
     DIALOG 3
     ADD_TALK $_npc$：
     ADD_TALK 　　/C154月1、2、8、9、15、16日下午/C0514：30-16：30/C00，晚上/C0519：30-21：30/C00进行，只有这个时段内，所有怪物都有几率掉落魔牙和春之手环，每1天活动最多获得/C0450个魔牙/C00。神州岛和瀛洲岛还有稀少的地鼠，100％掉落魔牙。
     ADD_TALK 　　每1天活动最多获得50个魔牙。可以在我这里抽化妆卡片或兑换宠物经验（请召唤出宠物）、经验，只在3月30日-4月20日之间有效，过期之后只能售卖给商店或者等候下一次活动开放。
     TALK   
ENDPROC
;
;
PROC 50
   RETURN 
ENDPROC 
;
