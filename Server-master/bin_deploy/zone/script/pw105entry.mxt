;70副本,入口npc,戴拓
proc 1
	strvar l
	GET_PLAYER_BASE_ATT 1 0 l 0
	if l < 68
		dialog 1
		ADD_TALK $_npc$:
		ADD_TALK #b#b咦，哪来的小#Race#，这里很危险，快到别处玩去，别妨碍我办正事。
		talk
		
	else
		dialog 1
		ADD_TALK $_npc$:
		ADD_TALK #b#b您终于来了，薇薇安已经进入铁翼基地很久了，到现在还没有出来，我提心她出了什么事，虽然她一再保证能安全回来……
		ADD_TALK #b#b据可靠消息，维奇教授正在进行一项秘密的实验，如果实验成功，整个荫川林地都被会毁灭，您赶紧去和她会合吧，她说会在其中留下线索，您稍加注意就会发现。
		option 100 #进入副本
		talk
		
	endif
	
endproc
proc 100
;	enter_pworld 105 0 1
	
	OPEN_PWORLD_UI 3
endproc
