;包袱怪物

;刷出怪物的事件
proc 1
endproc

;怪物死亡的事件
proc 2
    strvar x2 y2
	GET_MON_INFO 0 0 0 0 0 0 x2 y2
	NEW_MON 230052 1 @x2 @y2 0 0 0 0 0
endproc

proc 4

endproc
