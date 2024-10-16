;
;script filename: npc3
;
PROC 1
    DIALOG 0
    ADD_TALK $_npc$：
    ADD_TALK 你好，我是$_npc$，是负责和人聊天的。
    ADD_TALK 在我们的这个世界里，什么样的职业都有。
    NEXT 20
    TALK
    
ENDPROC
;
;
PROC 20
    DIALOG 1
    ADD_TALK $_npc$：
    ADD_TALK 你是刚刚到这个世界的吧？
    OPTION 21 是的。
    OPTION 22 不是。
    TALK
ENDPROC
;
;
;玩家选是新来的
PROC 21
    DIALOG 0
    ADD_TALK $_npc$：
    ADD_TALK 我就知道你是新新新新新新来的。你一定会觉得
    ADD_TALK 很奇怪为什么我会知道，其实，是因为我刚才看
    ADD_TALK 到很多陌生人，他们都说是新来的，所以我猜你
    ADD_TALK 也是的。
    NEXT 23
    TALK
    
ENDPROC
;
;
;玩家选不是新来的
PROC 22
    DIALOG 1
    ADD_TALK $_npc$：
    ADD_TALK 哦？为什么我以前没有看过你？
    ADD_TALK 这里的人我都认识的。你没有骗我吧？
    OPTION 28 没有骗你，我真的不是新来的。
    OPTION 29 你真厉害，被你看出来了。
    TALK
ENDPROC
;
;
PROC 23
    DIALOG 1
    ADD_TALK $_npc$：
    ADD_TALK 你不会觉得我很罗嗦吧？
    OPTION 24 不会的啦，你很健谈。
    OPTION 25 有点罗嗦哦。
    OPTION 26 离开。
    TALK
ENDPROC
;
;
;选很健谈
PROC 24
    DIALOG 0
    ADD_TALK $_npc$：
    ADD_TALK 呵呵，夸奖人也不会使用新的词语，我已经听了
    ADD_TALK NN次这个了，不过，还是谢谢你。
    NEXT 27
    TALK
    
ENDPROC
;
;选很罗嗦
PROC 25
    DIALOG 3
    ADD_TALK $_npc$：
    ADD_TALK 真伤心，呜呜。
    TALK
ENDPROC
;
;选离开
PROC 26
    DIALOG 3
    ADD_TALK $_npc$：
    ADD_TALK 再见，再见。
    TALK
ENDPROC
;
;
PROC 27
    DIALOG 3
    ADD_TALK $_npc$：
    ADD_TALK 谢谢你。
    TALK
ENDPROC
;
;
;OPTION 28 没有骗你，我真的不是新来的。
PROC 28
    DIALOG 3
    ADD_TALK $_npc$：
    ADD_TALK 哦，没有骗我就好，这年头啊，什么人都有。
    ADD_TALK 对了，对了，你是$_player$，我记起来了。
    TALK
ENDPROC
;
;
;OPTION 29 你真厉害，被你看出来了。
PROC 29
    DIALOG 0
    ADD_TALK $_npc$：
    ADD_TALK 呵，以为我是谁？年轻人，我见的人多了。
    ADD_TALK 还没有看错什么。
    NEXT 30
    TALK
    
ENDPROC
;
;
PROC 30
    DIALOG 0
    ADD_TALK $_npc$：
    ADD_TALK 甚至啊，我还能知道你的名字。
    NEXT 31
    TALK
    
ENDPROC
;
;
PROC 31
    DIALOG 0
    ADD_TALK $_npc$：
    ADD_TALK 而且绝不吹牛的。
    NEXT 32
    TALK
ENDPROC
;
;
PROC 32
    DIALOG 1
    ADD_TALK $_npc$：
    ADD_TALK 我说我知道你的名字，你想见证一下吗？
    OPTION 33 想见证啊，你说我叫什么？
    OPTION 34 没有功夫，不想见证
    TALK
ENDPROC
;
;
;OPTION 33 想见证啊，你说我叫什么？
PROC 33
    DIALOG 3
    ADD_TALK $_npc$：
    ADD_TALK 你啊，你的名字叫 $_player$ ！
    TALK
ENDPROC
;
;
;OPTION 34 没有功夫，不想见证
PROC 34
    DIALOG 3
    ADD_TALK $_npc$：
    ADD_TALK 没有关系，下次想了来找我。
    TALK
ENDPROC
;
;
;

