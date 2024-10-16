;祭祖神殿 ,潜伏者1
;潜伏者1
proc 1
	dialog 1
	ADD_TALK $_npc$:
	ADD_TALK #b#b你们来得正好，事态紧急！据我探查的最新情报，黑暗议会正在这里举行着某种神秘的仪式，如果成功了，他们将可能探寻到史前世界末日背后的秘密！
	ADD_TALK #b#b我们必须赶在他们行动前阻止阴谋！
	option 100 怎么阻止黑暗议会的阴谋？
	option 101 背后的秘密是什么？
	option 102 为什么这里的原著民会攻击我们？
	
	talk
	
endproc
proc 100
	dialog 1
	ADD_TALK $_npc$:
	ADD_TALK #b#b阿萨曼利用原著民协助黑暗议会，并且在黑暗祭坛上张开暗影天幕魔阵，为仪式提供能量。
	ADD_TALK #b#b为了阻止阿萨曼的行为，我们必须占领黑暗祭坛把守门者阿萨曼引出来。
	ADD_TALK #b#b暗影天幕由三层暗影屏障保护着，每破坏一个结界便会减少一层屏障，在太阳神殿里总共有水纹、火焰、暗影三个结界。
	option 200 水纹结界在哪？
	option 201 火焰结界在哪？
	option 203 暗影结界在哪？
	option 202 如何找到守门者阿萨曼？
	option 1 还有其它情报吗？

	talk
endproc
proc 101
	dialog 1
	ADD_TALK $_npc$:
	ADD_TALK #b#b详细的情况仍在探查中，但初步了解，黑暗议会似乎发现有关史前玛雅太阳历与世界末日背后的秘密，并在此暗中寻找着什么，试图破解玛雅密码获取不为人知的力量。

	option 1 还有其它情报吗？

	talk
endproc
proc 102
	dialog 1
	ADD_TALK $_npc$:
	ADD_TALK #b#b黑暗议会与这里的原著民达成了一项交易，他们赐予这些原著民强大的力量，而代价则是为黑暗议会效力，并保护他们的仪式顺利进行。

	option 1 还有其它情报吗？

	talk
endproc
proc 200
	dialog 1
	ADD_TALK $_npc$:
	ADD_TALK #b#b水纹结界就在太阳神殿西边的水之节点处，由欺诈者萨罗弥和他的手下保护着。
	ADD_TALK #b#b只要破坏了水纹结界，保护着暗影天幕的暗影屏障便会减少一层。
	
	option 201 火焰结界在哪？
	option 203 暗影结界在哪？
	option 202 如何找到守门者阿萨曼？
	option 1 还有其它情报吗？

	talk
endproc
proc 201
	dialog 1
	ADD_TALK $_npc$:
	ADD_TALK #b#b火焰结界就在沿路直走的火之节点处，由蛮荒者塞纳尔和他的手下保护着。
	ADD_TALK #b#b只要破坏了火焰结界，保护着暗影天幕的暗影屏障便会减少一层。

	option 200 水纹结界在哪？

	option 203 暗影结界在哪？

	option 202 如何找到守门者阿萨曼？
	option 1 还有其它情报吗？

	talk
endproc
proc 202
	dialog 1
	ADD_TALK $_npc$:
	ADD_TALK #b#b暗影天幕的能量来源于原著民的生命献祭，狡诈的阿萨曼控制了这些原著民，利用他们作为活祭品。
	ADD_TALK #b#b只要我们把黑暗祭坛占领，就能断绝能量供输引蛇出洞，届时阿萨曼必将派出他的爪牙前来阻止。
	ADD_TALK #b#b而我们要做的就是阻止他们继续为能量献祭，并坚守着进攻，直到阿萨曼现身，把他消灭为止。
	ADD_TALK #b#b要成功必须做好堵上性命的觉悟，暗影天幕就在太阳神殿南边的黑暗祭坛上
	option 200 水纹结界在哪？
	option 201 火焰结界在哪？
	option 203 暗影结界在哪？
	option 1 还有其它情报吗？

	talk
endproc
proc 203
	dialog 1
	ADD_TALK $_npc$:
	ADD_TALK #b#b暗影结界就在太阳神殿东边的暗之节点处，由撕裂者基尔里斯和他的手下保护着。
	ADD_TALK #b#b只要破坏了暗影结界，保护着暗影天幕的暗影屏障便会减少一层。

	option 200 水纹结界在哪？
	option 201 火焰结界在哪？
	option 202 如何找到守门者阿萨曼？
	option 1 还有其它情报吗？

	talk
endproc
