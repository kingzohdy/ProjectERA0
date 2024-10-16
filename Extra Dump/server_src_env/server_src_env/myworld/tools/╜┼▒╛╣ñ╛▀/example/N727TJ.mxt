;老翁
;发放火种
;
PROC 1
	GET_TASK_STATUS 456 270
	GET_TASK_VAR 456 0 271
	GETNUM_GOODS 12075 997
	if 270 = 2 and 271 = 20 and 997 != 0
		DIALOG 1
		ADD_TALK $_npc$：
    		ADD_TALK 　　好心人啊，看见我的女儿红苓了吗？她自从去了山洞给我采药就再也没有回来了，我可怜的孩子啊！
    		OPTION 10 关于火种
    		OPTION 20 #没什么事情，打扰了！
		TALK
		return	
	ENDIF 
	
	DIALOG 3
      	ADD_TALK $_npc$：
      	ADD_TALK 　　好心人啊，看见我的女儿红苓了吗？她自从去了山洞给我采药就再也没有回来了，我可怜的孩子啊！			
      	TALK    
ENDPROC
;----------
PROC 10
	DIALOG 1
	ADD_TALK $_npc$：
    	ADD_TALK 　　我的乖女儿找不到了，活着也没有什么意思。有没有火种也都没有关系了。不过还是谢谢你特意送来。
    	ADD_TALK 　　还有谁没有火种？好像东北角的/C05莫须/C00还没有吧，我不太清楚哦。     
    	OPTION 12 脑筋急转弯
    	OPTION 11 #交出火种
	TALK
	return	
ENDPROC
;----------------
PROC 11
	GET_TASK_STATUS 456 270
	GET_TASK_VAR 456 0 271
	GETNUM_GOODS 12075 997
	if 270 = 2 and 271 = 20 and 997 != 0
		DEL_GOODS 12075 1 1
		if 1 = 0
			SET_TASK_VAR 456 0 30
			ADD_TASK_HINT 456 03
		else
			MSG_PLAYER 3 2 删除火种失败，请稍后再试！
		ENDIF 
	ENDIF
ENDPROC
;---------
PROC 12
	DIALOG 1
	ADD_TALK $_npc$：
    	ADD_TALK 　　草地上画了一个直径十米的圆圈，内有牛一头，圆圈中心插了一根木桩。牛被一根五米长的绳子栓着，如果不割断绳子，也不解开绳子，那么此牛能否吃到圈外的草？    
    	OPTION 13 看答案
	TALK
ENDPROC
;---------
PROC 13
	DIALOG 3
	ADD_TALK $_npc$：
    	ADD_TALK 　　因为绳子没有绑在木桩上，当然可以吃草了。哈哈，你猜对了吗？
	TALK
ENDPROC
;--------
PROC 20
	return
ENDPROC