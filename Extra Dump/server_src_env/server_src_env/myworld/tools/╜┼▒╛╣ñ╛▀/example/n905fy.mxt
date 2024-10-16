;子训小徒清心的药方任务
;任务jq001、jq002、jq003：这三个任务不允许同时进行。 
;任务mq003:通玄先生(408) 仅作判断
;
PROC 1
;获取通玄先生的任务完成情况
GET_TASK_STATUS 408 158
GET_LEVEL 5 
    ;未完成通玄先生(进行中或未接) 
    IF 158 = 2 or 158 = 0 
       DIALOG 1
       ADD_TALK $_npc$：
       ADD_TALK      你知道啦,天下药师都是从咱家出来的，病者有求，无不应验。我的主人就是龙城内外鼎鼎大名的蓟子训先生哦!他可是铁拐大仙的徒弟呢!
       add_talk      咱师傅的药方，在整个神州岛，不，在环岛诸大陆上，可都是鼎鼎大名!
       OPTION 11     哦，是这样啊。
       TALK
       return
    endif
    If 158 = 1
    GET_LEVEL 5    
    IF 5 < 15
       DIALOG 3
       ADD_TALK $_npc$：
       ADD_TALK 本来想拜托你去帮我做些重要的工作。可是以你现在的实力，恐怕还不行哦。
       TALK 
       RETURN
    ENDIF 
    IF 5 >= 15 and 5 < 25 
       GET_TASK_STATUS 101 281
       ;jq001的未接判断     
       IF 281 = 0      
          DIALOG 0
          ADD_TALK $_npc$：
          ADD_TALK 子训师傅的病人实在是太多了，每个人俄病状不同，所需的药方也不尽相同，我一个人要准备这许多药方，实在是忙不过来呢！
          NEXT 2
          TALK 
          RETURN
       ENDIF 
       ;jq001的进行中判断
       IF 281 = 2
    	  CALL 7
    	  RETURN
       ENDIF
       ;jq001的已完成判断
       IF 281 = 1
    	  DIALOG 1
          ADD_TALK $_npc$:
          ADD_TALK 　　你找到了一些药方的原材料，真不错啊，可是对神州岛上的病人来说，这还是远远未够班的呵！子训师傅还需要更多的药材！
          OPTION 10 好，让我来做吧！
          OPTION 11 还有别的事。
          TALK 
          RETURN 
       ENDIF
    ELSE
       IF 5 < 40
          GET_TASK_STATUS 101 281 
          GET_TASK_STATUS 102 289
          ;jq001的进行中判断
          IF 281 = 2
    	     CALL 7
    	     RETURN
          ELSE 
             ;jq002的未接判断     
             IF 289 = 0      
                DIALOG 0
                ADD_TALK $_npc$：
                ADD_TALK 你还在神州岛上转悠呢？可真不容易，要不，你还是去帮我找我子训师傅那名扬四海的药方吧。
                NEXT 4
                TALK 
                RETURN 
             ENDIF 
             ;jq002的进行中判断         
             IF 289 = 2
    	        CALL 8
    	        RETURN
	     ENDIF    	
             ;jq002的已完成判断
             IF 289 = 1
    	        DIALOG 1
                ADD_TALK $_npc$:
                ADD_TALK 啊哈，干得不错，你是我见过的人当中，算是比较快的一个啦！我觉得你很适合做药师这行哦，怎么样，要不要继续干收集药材这份很有前途的职业呢？
                OPTION 12 没问题！
                OPTION 11 还有别的事。
                TALK 
                RETURN 
             ENDIF
          ENDIF
       ELSE
         ;IF 5 <= 99 
            GET_TASK_STATUS 107 281 
            GET_TASK_STATUS 108 289
            GET_TASK_STATUS 109 297
            ;jq001的进行中判断
            IF 281 = 2
    	       CALL 7
    	       RETURN
            ELSE 
               ;jq002的进行中判断
               IF 289 = 2
                  CALL 8
                  RETURN
               ELSE
                  ;jq003的未接判断     
                  IF 297 = 0      
                     DIALOG 0
                     ADD_TALK $_npc$：
                     ADD_TALK 看起来你气色不错，作个快乐的药师，是我的梦想哦。
                     NEXT 5
                     TALK 
                     RETURN 
                  ENDIF 
                  ;jq003的进行中判断         
                  IF 297 = 2
    	             CALL 9
    	             RETURN
	          ENDIF    	
                  ;jq003的已完成判断
                  IF 297 = 1
    	             DIALOG 1
                     ADD_TALK $_npc$:
                     ADD_TALK 由于你的缘故，子训师傅的药方，已经在这世界内成为享誉第一的品牌了，我想，收集药材的工作还是继续由你承担比较好呢！
                     
                     OPTION 13 没问题！
                     OPTION 11 还有别的事。
                     TALK 
                     RETURN 
                  ENDIF
               ENDIF 
            ENDIF 
       ENDIF     
    ENDIF    
    ENDIF      
ENDPROC
;
;
PROC 2
      DIALOG 1
      ADD_TALK $_npc$:
      ADD_TALK  我希望有人能给我搭把手，你能帮我去收集药材吗？
      OPTION 10 当然可以！
      OPTION 11 还有别的事
      TALk    
ENDPROC
;
;
PROC 4
      DIALOG 1
      ADD_TALK $_npc$:
      ADD_TALK 不仅是在神州岛，子训师傅的药方现在已名扬四海，你愿意继续为百姓的健康而努力吗？
      OPTION 12 当然可以！
      OPTION 11 还有别的事
      TALK    
ENDPROC
;
;
PROC 5
      DIALOG 1
      ADD_TALK $_npc$:
      ADD_TALK  我又看到你，为了药师职业的光荣，为了子训师傅的名誉，拜托你继续收集药方哦？
      OPTION 13 难不倒我
      OPTION 11 还有别的事
      TALK   
ENDPROC
;
;
PROC 7
   GET_TASK_VAR 101 0 282
   IF 282 = 1
      GETNUM_GOODS 4103 283
      IF 283 < 10 
	 DIALOG 3
    	 ADD_TALK $_npc$：
    	 ADD_TALK 　　你还没收集到子训师傅的药方中的/C0410/C00只/C05世界树枝/C00吗？
	 TALK 
	 RETURN
      ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$：
    	 ADD_TALK  啊，不错哦，你已经拿回了药方的素材，我代神州岛生病的百姓感谢你，嘻嘻。
    	 OPTION 20 #领取奖励
    	 TALK
    	 RETURN		
      ENDIF 
   ENDIF
   if 282 = 2
   GETNUM_GOODS  4159 284
      IF 284 < 10 
	 DIALOG 3
    	 ADD_TALK $_npc$：
    	 ADD_TALK 　　你还没收集到子训师傅的药方中的/C0410/C00只/C05鬼火/C00吗？
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$：
    	 ADD_TALK 啊，不错哦，你已经拿回了药方的素材，我代神州岛生病的百姓感谢你，嘻嘻。
    	 OPTION 20 #领取奖励
    	 TALK
    	 RETURN	
     ENDIF	
   ENDIF
   if 282 = 3
   GETNUM_GOODS 4151 285
      IF 285 < 10 
	 DIALOG 3
    	 ADD_TALK $_npc$：
    	 ADD_TALK 　　你还没收集到子训师傅的药方中的/C0410/C00只/C05地藏珠/C00吗？
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$：
    	 ADD_TALK 啊，不错哦，你已经拿回了药方的素材，我代神州岛生病的百姓感谢你，嘻嘻。
    	 OPTION 20 #领取奖励
    	 TALK
    	 RETURN
       ENDIF		
   ENDIF
   if 282 = 4
   GETNUM_GOODS 3005 286
      IF 286 < 5 
	 DIALOG 3
    	 ADD_TALK $_npc$：
    	 ADD_TALK 　　你还没收集到子训师傅的药方中的/C045/C00只/C05叉烧包/C00吗？
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$：
    	 ADD_TALK 啊，不错哦，你已经拿回了药方的素材，我代神州岛生病的百姓感谢你，嘻嘻。
    	 OPTION 20 #领取奖励
    	 TALK
    	 RETURN
    	ENDIF		
   ENDIF 
   if 282 = 5
   GETNUM_GOODS 3006 287
      IF 287 < 5 
	 DIALOG 3
    	 ADD_TALK $_npc$：
    	 ADD_TALK 　　你还没收集到子训师傅的药方中的/C045/C00瓶/C05人参酒/C00吗？
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$：
    	 ADD_TALK 啊，不错哦，你已经拿回了药方的素材，我代神州岛生病的百姓感谢你，嘻嘻。
    	 OPTION 20 #领取奖励
    	 TALK
    	 RETURN		
      ENDIF   
   ENDIF   	
ENDPROC 
;
;
PROC 8
   GET_TASK_VAR 102 0 290
   IF 290 = 1
      GETNUM_GOODS 4347 291
      IF 291 < 10 
	 DIALOG 3
    	 ADD_TALK $_npc$：
    	 ADD_TALK 　　你还没完成子训师傅名扬四海的药方中的/C0410/C00只/C05大蜗牛壳/C00啊。
	 TALK 
	 RETURN
      ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$：
    	 ADD_TALK  干得不错！照我看，你应该能成为一个优秀的药师的，祝你好运！
    	 OPTION 30 #领取奖励
    	 TALK
    	 RETURN
     ENDIF		
   ENDIF
   IF 290 = 2
      GETNUM_GOODS 4304 292
      IF 292 < 10 
	 DIALOG 3
    	 ADD_TALK $_npc$：
    	 ADD_TALK 　　你还没完成子训师傅名扬四海的药方中的/C0410/C00只/C05山鼠尾巴/C00啊。
	 TALK 
	 RETURN
      ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$：
    	 ADD_TALK 干得不错！照我看，你应该能成为一个优秀的药师的，祝你好运！
    	 OPTION 30 #领取奖励
    	 TALK
    	 RETURN		
      ENDIF 
   ENDIF
   if 290 = 3
   GETNUM_GOODS 4122 293
      IF 293 < 10 
	 DIALOG 3
    	 ADD_TALK $_npc$：
    	 ADD_TALK 　　你还没完成子训师傅名扬四海的药方中的/C0410/C00只/C05白虎皮/C00啊。
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$：
    	 ADD_TALK 干得不错！照我看，你应该能成为一个优秀的药师的，祝你好运！
    	 OPTION 30 #领取奖励
    	 TALK
    	 RETURN		
    ENDIF
   ENDIF
   if 290 = 4
   GETNUM_GOODS 4312 294
      IF 294 < 5 
	 DIALOG 3
    	 ADD_TALK $_npc$：
    	 ADD_TALK 　　你还没完成子训师傅名扬四海的药方中的/C045/C00只/C05符咒/C00啊。
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$：
    	 ADD_TALK 干得不错！照我看，你应该能成为一个优秀的药师的，祝你好运！
    	 OPTION 30 #领取奖励
    	 TALK
    	 RETURN		
      ENDIF
   ENDIF 
   if 290 = 5
   GETNUM_GOODS 3007 295
      IF 295 < 5 
	 DIALOG 3
    	 ADD_TALK $_npc$：
    	 ADD_TALK 　　你还没完成子训师傅名扬四海的药方中的/C045/C00只/C05烤乳猪/C00啊。
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$：
    	 ADD_TALK 干得不错！照我看，你应该能成为一个优秀的药师的，祝你好运！
    	 OPTION 30 #领取奖励
    	 TALK
    	 RETURN		
      ENDIF   
   ENDIF   	
ENDPROC
;
;
PROC 9
   GET_TASK_VAR 103 0 298
   if 298 = 1
   GETNUM_GOODS 4311 299
      IF 299 < 10 
	 DIALOG 3
    	 ADD_TALK $_npc$：
    	 ADD_TALK 　　你还没收集到子训师傅无人不知、无人不晓的的药方中的/C0410/C00只/C05孟婆汤/C00吗？
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$：
    	 ADD_TALK 哇，像你这么厉害的人，还能做这事的可不多哦，好心有好报，我不会让你失望的，嘿嘿。
    	 OPTION 40 #领取奖励
    	 TALK
    	 RETURN		
      ENDIF   
   ENDIF
   if 298 = 2
   GETNUM_GOODS 4352 300
      IF 300 < 10 
	 DIALOG 3
    	 ADD_TALK $_npc$：
    	 ADD_TALK 　　你还没收集到子训师傅无人不知、无人不晓的的药方中的/C0410/C00只/C05风筝线/C00吗？
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$：
    	 ADD_TALK 哇，像你这么厉害的人，还能做这事的可不多哦，好心有好报，我不会让你失望的，嘿嘿。
    	 OPTION 40 #领取奖励
    	 TALK
    	 RETURN		
      ENDIF   
   ENDIF
   if 298 = 3
   GETNUM_GOODS 4315 301
      IF 301 < 10 
	 DIALOG 3
    	 ADD_TALK $_npc$：
    	 ADD_TALK 　　你还没收集到子训师傅无人不知、无人不晓的的药方中的/C0410/C00只/C05赶马绳/C00吗？
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$：
    	 ADD_TALK 哇，像你这么厉害的人，还能做这事的可不多哦，好心有好报，我不会让你失望的，嘿嘿。
    	 OPTION 40 #领取奖励
    	 TALK
    	 RETURN		
      ENDIF   
   ENDIF
   if 298 = 4
   GETNUM_GOODS 3012 302
      IF 302 < 5 
	 DIALOG 3
    	 ADD_TALK $_npc$：
    	 ADD_TALK 　　你还没收集到子训师傅无人不知、无人不晓的的药方中的/C045/C00只/C05（小）生命药水/C00吗？
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$：
    	 ADD_TALK 哇，像你这么厉害的人，还能做这事的可不多哦，好心有好报，我不会让你失望的，嘿嘿。
    	 OPTION 40 #领取奖励
    	 TALK
    	 RETURN		
      ENDIF   
   ENDIF
   if 298 = 5
   GETNUM_GOODS 3013 303
      IF 303 < 5 
	 DIALOG 3
    	 ADD_TALK $_npc$：
    	 ADD_TALK 　　你还没收集到子训师傅无人不知、无人不晓的的药方中的/C045/C00只/C05（小）法力药水/C00吗？
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$：
    	 ADD_TALK 哇，像你这么厉害的人，还能做这事的可不多哦，好心有好报，我不会让你失望的，嘿嘿。
    	 OPTION 40 #领取奖励
    	 TALK
    	 RETURN		
      ENDIF   
   ENDIF
ENDPROC
;
;
;      
;选择jq001:神州岛的药方任务
PROC 10 
    GET_TASK_NUM 0  
    IF 0 >= 10
    	DIALOG 3
    	ADD_TALK /C04$系统提示$：/C00
    	ADD_TALK /C04对不起，你有十个任务在进行中，无法再领取新的任务。/C00
	TALK 
    ELSE
        RAND 9  5
        SWITCH 9
        CASE 0 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK 　　如果你是一个药师的话，那你就作对了！如果你不是药师，你也没作错！，那么，师傅的药方中需要/C0410/C00只/C05世界树枝/C00，请你帮我带回来吧！
           TALK
           ACCEPT_TASK 101 1
           SET_TASK_VAR 101 0 1
           ADD_TASK_HINT 101 01 
           ENDCASE
        CASE 1
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK 　　如果你是一个药师的话，那你就作对了！如果你不是药师，你也没作错！，那么，师傅的药方中需要/C0410/C00只/C05鬼火/C00，请你帮我带回来吧！
           TALK
           ACCEPT_TASK 101 1
           SET_TASK_VAR 101 0 2
           ADD_TASK_HINT 101 02
           ENDCASE 
        CASE 2 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK 　　如果你是一个药师的话，那你就作对了！如果你不是药师，你也没作错！，那么，师傅的药方中需要/C0410/C00只/C05地藏珠/C00，请你帮我带回来吧！
           TALK
           ACCEPT_TASK 101 1
           SET_TASK_VAR 101 0 3
           ADD_TASK_HINT 101 03
           ENDCASE
        CASE 3 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK 　　如果你是一个药师的话，那你就作对了！如果你不是药师，你也没作错！，那么，师傅的药方中需要/C045/C00只/C05叉烧包/C00，请你帮我带回来吧！
           TALK
           ACCEPT_TASK 101 1
           SET_TASK_VAR 101 0 4
           ADD_TASK_HINT 101 04 
           ENDCASE
        CASE 4 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK 　　如果你是一个药师的话，那你就作对了！如果你不是药师，你也没作错！，那么，师傅的药方中需要/C045/C00只/C05人参酒/C00，请你帮我带回来吧！
           TALK
           ACCEPT_TASK 101 1
           SET_TASK_VAR 101 0 5
           ADD_TASK_HINT 101 05 
           ENDCASE
        ENDSWITCH              
    ENDIF     
ENDPROC
;
;
;选择jq002:名扬四海的药方任务
PROC 12 
    GET_TASK_NUM 0  
    IF 0 >= 10
       DIALOG 3
    	ADD_TALK /C04$系统提示$：/C00
    	ADD_TALK /C04对不起，你有十个任务在进行中，无法再领取新的任务。/C00
       TALK 
    ELSE
        RAND 9  5
        SWITCH 9
        CASE 0 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK 　　收集药方，是成为一个好药师的必经之路哦！现在，子训师傅名扬四海的药方中需要/C0410/C00只/C05大蜗牛壳/C00，请你把它带回来吧！
           TALK
           ACCEPT_TASK 102 1
           SET_TASK_VAR 102 0 1
           ADD_TASK_HINT 102 01 
           ENDCASE
        CASE 1
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK 　　收集药方，是成为一个好药师的必经之路哦！现在，子训师傅名扬四海的药方中需要/C0410/C00只/C05山鼠尾巴/C00，请你把它带回来吧！
           TALK
           ACCEPT_TASK 102 1
           SET_TASK_VAR 102 0 2
           ADD_TASK_HINT 102 02 
           ENDCASE
        CASE 2 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK 　　收集药方，是成为一个好药师的必经之路哦！现在，子训师傅名扬四海的药方中需要/C0410/C00只/C05白虎皮/C00，请你把它带回来吧！
           TALK
           ACCEPT_TASK 102 1
           SET_TASK_VAR 102 0 3
           ADD_TASK_HINT 102 03
           ENDCASE
        CASE 3 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK 　　收集药方，是成为一个好药师的必经之路哦！现在，子训师傅名扬四海的药方中需要/C045/C00只/C05符咒/C00，请你把它带回来吧！
           TALK
           ACCEPT_TASK 102 1
           SET_TASK_VAR 102 0 4
           ADD_TASK_HINT 102 04 
           ENDCASE
        CASE 4 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK 　　收集药方，是成为一个好药师的必经之路哦！现在，子训师傅名扬四海的药方中需要/C045/C00只/C05烤乳猪/C00，请你把它带回来吧！
           TALK
           ACCEPT_TASK 102 1
           SET_TASK_VAR 102 0 5
           ADD_TASK_HINT 102 05 
           ENDCASE
        ENDSWITCH      
    ENDIF 
ENDPROC
;
;
;选择jq003:子训的药方任务
PROC 13
    GET_TASK_NUM 0  
    IF 0 >= 10
       DIALOG 3
    	ADD_TALK /C04$系统提示$：/C00
    	ADD_TALK /C04对不起，你有十个任务在进行中，无法再领取新的任务。/C00
       TALK 
    ELSE
        RAND 9  5
        SWITCH 9
        CASE 0 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK 　　一个高手总是有济世救民之心，子训师傅的药方中还需要/C0410/C00份/C05孟婆汤/C00，请你把它带回来给我吧！
           TALK
           ACCEPT_TASK 103 1
           SET_TASK_VAR 103 0 1
           ADD_TASK_HINT 103 01 
           ENDCASE
        CASE 1
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK 　　一个高手总是有济世救民之心，子训师傅的药方中还需要/C0410/C00份/C05风筝线/C00，请你把它带回来给我吧！
           TALK
           ACCEPT_TASK 103 1
           SET_TASK_VAR 103 0 2
           ADD_TASK_HINT 103 02 
           ENDCASE
        CASE 2 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK 　　一个高手总是有济世救民之心，子训师傅的药方中还需要/C0410/C00份/C05赶马绳/C00，请你把它带回来给我吧！
           TALK
           ACCEPT_TASK 103 1
           SET_TASK_VAR 103 0 3
           ADD_TASK_HINT 103 03
           ENDCASE
        CASE 3 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK 　　一个高手总是有济世救民之心，子训师傅的药方中还需要/C045/C00份/C05（小）生命药水/C00，请你把它带回来给我吧！
           TALK
           ACCEPT_TASK 103 1
           SET_TASK_VAR 103 0 4
           ADD_TASK_HINT 103 04
           ENDCASE 
        CASE 4 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK 　　一个高手总是有济世救民之心，子训师傅的药方中还需要/C045/C00份/C05（小）法力药水/C00，请你把它带回来给我吧！
           TALK
           ACCEPT_TASK 103 1
           SET_TASK_VAR 103 0 5
           ADD_TASK_HINT 103 05 
           ENDCASE
        ENDSWITCH      
    ENDIF 
ENDPROC
;
;
;选择退出对话
PROC 11
    DIALOG 3
    ADD_TALK $_npc$:
    ADD_TALK 　　不管你是否是药师，都应该有一颗济世救民的心呀！
    TALK
ENDPROC  
;
;
;
;jq001的奖励
PROC 20
   GET_TASK_VAR 101 0 282
   GET_JOB 1016
IF 1016 = 4
   If 282 = 1
    DEL_GOODS 4103 10 1
    ADD_MONEY 1000
    ADD_EXP 500
     ;计数   	             
    GET_VAR 14 0 288
       ADD 288 1                
       IF 288 < 20
    	     SET_VAR 14 0 @288
           MUL 288 200 
           ADD_MONEY @288
           ADD_EXP @288
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 101 0 0
    FINISH_TASK 101 
    ENDIF
    if 282 = 2
    DEL_GOODS 4159 10 1
    ADD_MONEY 1000
    ADD_EXP 500
     ;计数   	             
    GET_VAR 14 0 288
       ADD 288 1                
       IF 288 < 20
    	     SET_VAR 14 0 @288
           MUL 288 200 
           ADD_MONEY @288
           ADD_EXP @288
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 101 0 0
    FINISH_TASK 101 
    ENDIF
    if 282 = 3
    DEL_GOODS 4151 10 1
    ADD_MONEY 1000
    ADD_EXP 500
    ;计数
    GET_VAR 14 0 288
       ADD 288 1                
       IF 288 < 20
    	     SET_VAR 14 0 @288
           MUL 288 200 
           ADD_MONEY @288
           ADD_EXP @288
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 101 0 0
    FINISH_TASK 101 
    ENDIF
    if 282 = 4
    DEL_GOODS 3005 5 1
 ADD_MONEY 1000
    ADD_EXP 500
    ;计数
    GET_VAR 14 0 288
       ADD 288 1                
       IF 288 < 20
    	     SET_VAR 14 0 @288
           MUL 288 200 
           ADD_MONEY @288
           ADD_EXP @288
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 101 0 0
    FINISH_TASK 101 
    ENDIF
      if 282 = 5
    DEL_GOODS 3006 5 1
    ADD_MONEY 1000
    ADD_EXP 500
     ;计数   	             
   ADD_MONEY 1000
    ADD_EXP 500
    ;计数
    GET_VAR 14 0 288
       ADD 288 1                
       IF 288 < 20
    	     SET_VAR 14 0 @288
           MUL 288 200 
           ADD_MONEY @288
           ADD_EXP @288
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 101 0 0
    FINISH_TASK 101 
    ENDIF
;
ELSE
if 282 = 1
    DEL_GOODS 4103 10 1
    ADD_MONEY 500
    SET_TASK_VAR 101 0 0
    FINISH_TASK 101 
    ENDIF
    if 282 = 2
    DEL_GOODS 4159 10 1
    ADD_MONEY 500
    SET_TASK_VAR 101 0 0
    FINISH_TASK 101 
    ENDIF
    if 282 = 3
    DEL_GOODS 4151 10 1
    ADD_MONEY 500
    SET_TASK_VAR 101 0 0
    FINISH_TASK 101 
    ENDIF
    if 282 = 4
    DEL_GOODS 3005 5 1
    ADD_MONEY 500
    SET_TASK_VAR 101 0 0
    FINISH_TASK 101 
    ENDIF
      if 282 = 5
    DEL_GOODS 3006 5 1
    ADD_MONEY 500
    SET_TASK_VAR 101 0 0
    FINISH_TASK 101 
    ENDIF
ENDIF
ENDPROC
;
;
;
;jq002的奖励
PROC 30
   GET_TASK_VAR 102 0 290
   GET_JOB 1016
IF 1016 = 4
   If 290 = 1
    DEL_GOODS 4347 10 1
    ADD_MONEY 1500
    ADD_EXP 1000
     ;计数   	             
    GET_VAR 14 0   296
       ADD 296 1                
       IF 296 < 20
    	     SET_VAR 14 0 @296
           MUL 296 500 
           ADD_MONEY @296
           ADD_EXP @296
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 102 0 0
    FINISH_TASK 102 
    ENDIF
    if 290 = 2
    DEL_GOODS 4304 10 1
    ADD_MONEY 1500
    ADD_EXP 1000
     ;计数   	             
    GET_VAR 14 0   296
       ADD 296 1                
       IF 296 < 20
    	     SET_VAR 14 0 @296
           MUL 296 500 
           ADD_MONEY @296
           ADD_EXP @296
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 102 0 0
    FINISH_TASK 102 
    ENDIF
    if 290 = 3
    DEL_GOODS 4122 10 1
    ADD_MONEY 1500
    ADD_EXP 1000
     ;计数   	             
    GET_VAR 14 0   296
       ADD 296 1                
       IF 296 < 20
    	     SET_VAR 14 0 @296
           MUL 296 500 
           ADD_MONEY @296
           ADD_EXP @296
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 102 0 0
    FINISH_TASK 102 
    ENDIF
    if 290 = 4
    DEL_GOODS 4312 5 1
    ADD_MONEY 1500
    ADD_EXP 1000
     ;计数   	             
    GET_VAR 14 0   296
       ADD 296 1                
       IF 296 < 20
    	     SET_VAR 14 0 @296
           MUL 296 500 
           ADD_MONEY @296
           ADD_EXP @296
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 102 0 0
    FINISH_TASK 102 
    ENDIF
      if 290 = 5
    DEL_GOODS 3007 5 1
    ADD_MONEY 1500
    ADD_EXP 1000
     ;计数   	             
    GET_VAR 14 0   296
       ADD 296 1                
       IF 296 < 20
    	     SET_VAR 14 0 @296
           MUL 296 500 
           ADD_MONEY @296
           ADD_EXP @296
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 102 0 0
    FINISH_TASK 102 
    ENDIF
;
ELSE
if 290 = 1
    DEL_GOODS 4347 10 1
    ADD_MONEY 800
    SET_TASK_VAR 102 0 0
    FINISH_TASK 102
    ENDIF
    if 290 = 2
    DEL_GOODS 4304 10 1
    ADD_MONEY 800
    SET_TASK_VAR 102 0 0
    FINISH_TASK 102
    ENDIF
    if 290 = 3
    DEL_GOODS 4122 10 1
    ADD_MONEY 800
    SET_TASK_VAR 102 0 0
    FINISH_TASK 102
    ENDIF
    if 290 = 4
    DEL_GOODS 4312 5 1
   ADD_MONEY 800
    SET_TASK_VAR 102 0 0
    FINISH_TASK 102
    ENDIF
      if 290 = 5
    DEL_GOODS 3007 5 1
   ADD_MONEY 800
    SET_TASK_VAR 102 0 0
    FINISH_TASK 102
    ENDIF
ENDIF
ENDPROC
;
;
;
;jq003的奖励
PROC 40
   GET_TASK_VAR 103 0 298
   GET_JOB 1016
IF 1016 = 4
   If 298 = 1
    DEL_GOODS 4311 10 1
    ADD_MONEY 2000
    ADD_EXP 1000
     ;计数   	             
    GET_VAR 14 0   304
       ADD 304 1                
       IF 304 < 20
    	     SET_VAR 14 0 @304
           MUL 304 1000 
           ADD_MONEY @304
           ADD_EXP @304
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 103 0 0
    FINISH_TASK 103 
    ENDIF
    if 298 = 2
    DEL_GOODS 4352 10 1
    ADD_MONEY 2000
    ADD_EXP 1000
     ;计数   	             
    GET_VAR 14 0   304
       ADD 304 1                
       IF 304 < 20
    	     SET_VAR 14 0 @304
           MUL 304 1000 
           ADD_MONEY @304
           ADD_EXP @304
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 103 0 0
    FINISH_TASK 103 
    ENDIF
    if 298 = 3
    DEL_GOODS 4315 10 1
    ADD_MONEY 2000
    ADD_EXP 1000
     ;计数   	             
    GET_VAR 14 0   304
       ADD 304 1                
       IF 304 < 20
    	     SET_VAR 14 0 @304
           MUL 304 1000 
           ADD_MONEY @304
           ADD_EXP @304
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 103 0 0
    FINISH_TASK 103 
    ENDIF
    if 298 = 4
    DEL_GOODS 3012 5 1
    ADD_MONEY 2000
    ADD_EXP 1000
     ;计数   	             
    GET_VAR 14 0   304
       ADD 304 1                
       IF 304 < 20
    	     SET_VAR 14 0 @304
           MUL 304 1000 
           ADD_MONEY @304
           ADD_EXP @304
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 103 0 0
    FINISH_TASK 103 
    ENDIF
      if 298 = 5
    DEL_GOODS 3013 5 1
     ADD_MONEY 2000
    ADD_EXP 1000
     ;计数   	             
    GET_VAR 14 0   304
       ADD 304 1                
       IF 304 < 20
    	     SET_VAR 14 0 @304
           MUL 304 1000 
           ADD_MONEY @304
           ADD_EXP @304
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 103 0 0
    FINISH_TASK 103 
    ENDIF
;
ELSE
if 298 = 1
    DEL_GOODS 4311 10 1
    ADD_MONEY 1000
    SET_TASK_VAR 103 0 0
    FINISH_TASK 103
    ENDIF
    if 298 = 2
    DEL_GOODS 4352 10 1
    ADD_MONEY 1000
    SET_TASK_VAR 103 0 0
    FINISH_TASK 103
    ENDIF
    if 298 = 3
    DEL_GOODS 4315 10 1
    ADD_MONEY 1000
    SET_TASK_VAR 103 0 0
    FINISH_TASK 103
    ENDIF
    if 298 = 4
    DEL_GOODS 3012 5 1
    ADD_MONEY 1000
    SET_TASK_VAR 103 0 0
    FINISH_TASK 103
    ENDIF
      if 298 = 5
    DEL_GOODS 3013 5 1
    ADD_MONEY 1000
    SET_TASK_VAR 103 0 0
    FINISH_TASK 103
    ENDIF
ENDIF
ENDPROC
;
;

 	