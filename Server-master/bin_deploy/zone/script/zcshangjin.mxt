
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b我在想，我是不是能够回到过去的日子？在我那两把著名的小手枪威慑作用下，没有什么赏金任务能构成麻烦。
	add_talk #b#b
	add_talk #b#b#b#b#c186506<翡翠徽章特权完成次数+1>
 ;   add_talk #b#b#c186506每个阶段每天可以完成3次#n
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	strvar Lv
	GET_PLAYER_BASE_ATT 1 0 Lv 0
	if Lv < 30
		option 200 # 为了你的安全，30级以后再来找我吧
	endif
	
		OPTION 100 #(买卖)打开商店
		
	task_add_option 2263 1 101
	task_add_option 2264 1 102
	task_add_option 2265 1 103
	
	task_add_option 3351 1 108
	task_add_option 3352 1 109
	task_add_option 3353 1 110
	task_add_option 3354 1 111
	task_add_option 3355 1 112
	task_add_option 3356 1 113
	task_add_option 3357 1 114
	task_add_option 3358 1 115

	task_add_option 3359 1 116
	task_add_option 3360 1 117
	task_add_option 3361 1 118
	
	task_add_option 30100 1 119
	task_add_option 30101 1 120
	task_add_option 30102 1 121
	task_add_option 30103 1 122
	task_add_option 30104 1 123
	task_add_option 30105 1 124
	task_add_option 30106 1 125
	task_add_option 30107 1 126
	task_add_option 30108 1 127
	task_add_option 30109 1 128
	task_add_option 30110 1 129
	task_add_option 30111 1 130
	task_add_option 30120 1 131
	task_add_option 30121 1 132
	task_add_option 30122 1 133
	task_add_option 30123 1 134
	task_add_option 30124 1 135
	task_add_option 30125 1 136
	task_add_option 30126 1 137
	task_add_option 30127 1 138
	task_add_option 30128 1 139
	task_add_option 30129 1 140
	task_add_option 30140 1 141
	task_add_option 30141 1 142
	task_add_option 30142 1 143
	task_add_option 30143 1 144
	task_add_option 30144 1 145
	task_add_option 30145 1 146
	task_add_option 30146 1 147
	task_add_option 30147 1 148
	task_add_option 30148 1 149
	task_add_option 30149 1 150
	task_add_option 30150 1 151
	task_add_option 30160 1 152
	task_add_option 30161 1 153
	task_add_option 30162 1 154
	task_add_option 30163 1 155
	task_add_option 30164 1 156
	task_add_option 30165 1 157
	task_add_option 30166 1 158
	task_add_option 30167 1 159
	task_add_option 30168 1 160
	task_add_option 30169 1 161
	task_add_option 30300 1 162
	task_add_option 30301 1 163
	task_add_option 30302 1 164
	task_add_option 30303 1 165
	task_add_option 30304 1 166
	
	;;;;;
	
	task_add_option 30305 1 300
	task_add_option 30306 1 301
	task_add_option 30307 1 302
	task_add_option 30308 1 303
	task_add_option 30309 1 304
	;;;;
	
	
	task_add_option 30310 1 167
	task_add_option 30311 1 168
	task_add_option 30312 1 169
	task_add_option 30313 1 170
	task_add_option 30314 1 171
	
	;;;;;;;;;;;
	task_add_option 30315 1 305
	task_add_option 30316 1 306
	task_add_option 30317 1 307
	task_add_option 30318 1 308
	task_add_option 30319 1 309
	;;;;;;;;;;;;;;;;;;;;
	
	task_add_option 30320 1 172
	task_add_option 30321 1 173
	task_add_option 30322 1 174
	task_add_option 30323 1 175
	task_add_option 30324 1 176
	;;;;;
	
	task_add_option 30325 1 310
	task_add_option 30326 1 311
	task_add_option 30327 1 312
	task_add_option 30328 1 313
	task_add_option 30329 1 314
	
	;;;;
	task_add_option 30330 1 177
	task_add_option 30331 1 178
	task_add_option 30332 1 179
	task_add_option 30333 1 180
	task_add_option 30334 1 181
	task_add_option 30400 1 182
	task_add_option 30410 1 183
	task_add_option 30420 1 184
	task_add_option 30430 1 185
	
	task_add_option 30170 1 186
	task_add_option 30171 1 187
	task_add_option 30172 1 188
	task_add_option 30173 1 189
	task_add_option 30174 1 190
	
	task_add_option 30335 1 191
	task_add_option 30336 1 192
	task_add_option 30337 1 193
	task_add_option 30338 1 194
	task_add_option 30339 1 195
	
	task_add_option 30340 1 196
	task_add_option 30341 1 197
	task_add_option 30342 1 198
	;;;;;;;
	task_add_option 30343 1 315
	task_add_option 30344 1 316
	task_add_option 30345 1 317
	task_add_option 30346 1 318
	task_add_option 30347 1 319
	task_add_option 30348 1 320
	task_add_option 30349 1 321
	task_add_option 30350 1 322
	task_add_option 30351 1 323
	task_add_option 30352 1 324
	task_add_option 30353 1 325
	task_add_option 30354 1 326
	task_add_option 30355 1 327
	task_add_option 30356 1 328
	task_add_option 30357 1 329
	task_add_option 30358 1 330
	task_add_option 30359 1 331
	task_add_option 30360 1 332
	task_add_option 30361 1 333
	task_add_option 30362 1 334
	task_add_option 30363 1 335
	task_add_option 30364 1 336
	task_add_option 30365 1 337
	task_add_option 30366 1 338
	task_add_option 30367 1 339
	task_add_option 30368 1 340
	task_add_option 30369 1 341
	task_add_option 30370 1 342
	task_add_option 30371 1 343
	task_add_option 30372 1 344
	task_add_option 30373 1 345
	task_add_option 30374 1 346
	task_add_option 30375 1 347
	task_add_option 30376 1 348
	task_add_option 30377 1 349
	task_add_option 30378 1 350
	task_add_option 30379 1 351
	task_add_option 30380 1 352
	task_add_option 30381 1 353
	task_add_option 30382 1 354
	task_add_option 30383 1 355
	task_add_option 30384 1 356
	task_add_option 30385 1 357
	task_add_option 30386 1 358
	task_add_option 30387 1 359
	task_add_option 30388 1 360
	task_add_option 30389 1 361
	task_add_option 30390 1 362
	task_add_option 30391 1 363
	task_add_option 30392 1 364
	task_add_option 30393 1 365
	task_add_option 30394 1 366
	task_add_option 30395 1 367
	task_add_option 30396 1 368
	task_add_option 30397 1 369
	task_add_option 30398 1 370
	task_add_option 30399 1 371
;	task_add_option 30400 1 372
	task_add_option 30401 1 373
	task_add_option 30402 1 374
	task_add_option 30403 1 375
	task_add_option 30404 1 376
	task_add_option 30405 1 377
	task_add_option 30406 1 378
	task_add_option 30407 1 379
	task_add_option 30408 1 380
	task_add_option 30409 1 381
;	task_add_option 30410 1 382
	task_add_option 30411 1 383
	task_add_option 30412 1 384
	task_add_option 30413 1 385
	task_add_option 30414 1 386
	task_add_option 30415 1 387
	task_add_option 30416 1 388
	task_add_option 30417 1 389
	task_add_option 30418 1 390
	task_add_option 30419 1 391
;	task_add_option 30420 1 392
	task_add_option 30421 1 393
	task_add_option 30422 1 394
	task_add_option 30423 1 395
	task_add_option 30424 1 396
	task_add_option 30425 1 397
	task_add_option 30426 1 398
	task_add_option 30427 1 399
	task_add_option 30428 1 400
	task_add_option 30429 1 401
;	task_add_option 30430 1 402
	task_add_option 30431 1 403
	task_add_option 30432 1 404
	task_add_option 30433 1 405
	task_add_option 30434 1 406
	task_add_option 30435 1 407
	task_add_option 30436 1 408
	task_add_option 30437 1 409
	task_add_option 30438 1 410
	task_add_option 30439 1 411
	task_add_option 30440 1 412
	task_add_option 30441 1 413
	task_add_option 30442 1 414
	task_add_option 30443 1 415
	task_add_option 30444 1 416

;	TASKCYCLE_ADD_OPTION 123423199 1 100
;	TASKCYCLE_ADD_OPTION 124424200 1 101
;	TASKCYCLE_ADD_OPTION 125425201 1 102
;	TASKCYCLE_ADD_OPTION 126426202 1 103
;	TASKCYCLE_ADD_OPTION 127427 1 104
;	TASKCYCLE_ADD_OPTION 128428 1 105
;	TASKCYCLE_ADD_OPTION 129429 1 106
;	TASKCYCLE_ADD_OPTION 130430 1 107
;	TASKCYCLE_ADD_OPTION 131431 1 108
;	TASKCYCLE_ADD_OPTION 132432 1 109
                           
	strvar lv               
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv > 40            
		option 200 我要立刻完成一次【赏善罚恶】
	endif                  
                      
	talk                  
endproc                   

PROC 100                  
	OPEN_NPC_SHOP
ENDPROC 

PROC 101                  
	task_do_option 2263 1
ENDPROC
PROC 102                  
	task_do_option 2264 1
ENDPROC
PROC 103                  
	task_do_option 2265 1
ENDPROC

PROC 108                  
	task_do_option 3351 1  
ENDPROC                   
PROC 109
	task_do_option 3352 1
ENDPROC
PROC 110
	task_do_option 3353 1
ENDPROC
PROC 111
	task_do_option 3354 1
ENDPROC
PROC 112
	task_do_option 3355 1
ENDPROC
PROC 113
	task_do_option 3356 1
ENDPROC
PROC 114
	task_do_option 3357 1
ENDPROC
PROC 115
	task_do_option 3358 1
ENDPROC
PROC 116
	task_do_option 3359 1
ENDPROC
PROC 117
	task_do_option 3360 1
ENDPROC
PROC 118
	task_do_option 3361 1
ENDPROC

proc 119
	task_do_option 30100 1
endproc            
proc 120           
	task_do_option 30101 1
endproc              
proc 121           
	task_do_option 30102 1
endproc              
proc 122           
	task_do_option 30103 1
endproc              
proc 123           
	task_do_option 30104 1
endproc              
proc 124           
	task_do_option 30105 1
endproc              
proc 125           
	task_do_option 30106 1
endproc              
proc 126           
	task_do_option 30107 1
endproc              
proc 127           
	task_do_option 30108 1
endproc              
proc 128           
	task_do_option 30109 1
endproc              
proc 129           
	task_do_option 30110 1
endproc              
proc 130           
	task_do_option 30111 1
endproc              
proc 131           
	task_do_option 30120 1
endproc              
proc 132           
	task_do_option 30121 1
endproc              
proc 133           
	task_do_option 30122 1
endproc              
proc 134           
	task_do_option 30123 1
endproc              
proc 135           
	task_do_option 30124 1
endproc              
proc 136           
	task_do_option 30125 1
endproc              
proc 137           
	task_do_option 30126 1
endproc              
proc 138           
	task_do_option 30127 1
endproc              
proc 139           
	task_do_option 30128 1
endproc              
proc 140           
	task_do_option 30129 1
endproc              
proc 141           
	task_do_option 30140 1
endproc              
proc 142           
	task_do_option 30141 1
endproc              
proc 143           
	task_do_option 30142 1
endproc              
proc 144           
	task_do_option 30143 1
endproc              
proc 145           
	task_do_option 30144 1
endproc              
proc 146           
	task_do_option 30145 1
endproc              
proc 147           
	task_do_option 30146 1
endproc              
proc 148           
	task_do_option 30147 1
endproc              
proc 149           
	task_do_option 30148 1
endproc              
proc 150           
	task_do_option 30149 1
endproc              
proc 151           
	task_do_option 30150 1
endproc              
proc 152           
	task_do_option 30160 1
endproc              
proc 153           
	task_do_option 30161 1
endproc              
proc 154           
	task_do_option 30162 1
endproc              
proc 155           
	task_do_option 30163 1
endproc              
proc 156           
	task_do_option 30164 1
endproc              
proc 157           
	task_do_option 30165 1
endproc              
proc 158           
	task_do_option 30166 1
endproc              
proc 159           
	task_do_option 30167 1
endproc              
proc 160           
	task_do_option 30168 1
endproc              
proc 161           
	task_do_option 30169 1
endproc              
proc 162           
	task_do_option 30300 1
endproc              
proc 163           
	task_do_option 30301 1
endproc              
proc 164           
	task_do_option 30302 1
endproc              
proc 165           
	task_do_option 30303 1
endproc              
proc 166           
	task_do_option 30304 1
endproc              
proc 167           
	task_do_option 30310 1
endproc              
proc 168           
	task_do_option 30311 1
endproc              
proc 169           
	task_do_option 30312 1
endproc              
proc 170           
	task_do_option 30313 1
endproc              
proc 171           
	task_do_option 30314 1
endproc              
proc 172           
	task_do_option 30320 1
endproc              
proc 173           
	task_do_option 30321 1
endproc              
proc 174           
	task_do_option 30322 1
endproc              
proc 175           
	task_do_option 30323 1
endproc              
proc 176           
	task_do_option 30324 1
endproc              
proc 177           
	task_do_option 30330 1
endproc              
proc 178           
	task_do_option 30331 1
endproc              
proc 179           
	task_do_option 30332 1
endproc              
proc 180           
	task_do_option 30333 1
endproc              
proc 181           
	task_do_option 30334 1
endproc              
proc 182           
	task_do_option 30400 1
endproc              
proc 183           
	task_do_option 30410 1
endproc              
proc 184           
	task_do_option 30420 1
endproc              
proc 185           
	task_do_option 30430 1
endproc    


;;;;;
             
proc 186           
	task_do_option 30170 1
endproc            
proc 187           
	task_do_option 30171 1
endproc            
proc 188           
	task_do_option 30172 1
endproc            
proc 189           
	task_do_option 30173 1
endproc            
proc 190           
	task_do_option 30174 1
endproc            
proc 191           
	task_do_option 30335 1
endproc            
proc 192           
	task_do_option 30336 1
endproc            
proc 193           
	task_do_option 30337 1
endproc            
proc 194           
	task_do_option 30338 1
endproc            
proc 195           
	task_do_option 30339 1
endproc            
proc 196           
	task_do_option 30340 1
endproc            
proc 197           
	task_do_option 30341 1
endproc            
proc 198           
	task_do_option 30342 1
endproc
           
proc 300           
	task_do_option 30305 1
endproc
proc 301           
	task_do_option 30306 1
endproc
proc 302           
	task_do_option 30307 1
endproc
proc 303           
	task_do_option 30308 1
endproc
proc 304           
	task_do_option 30309 1
endproc

proc 305           
	task_do_option 30315 1
endproc
proc 306           
	task_do_option 30316 1
endproc
proc 307           
	task_do_option 30317 1
endproc
proc 308           
	task_do_option 30318 1
endproc
proc 309           
	task_do_option 30319 1
endproc


proc 310           
	task_do_option 30325 1
endproc
proc 311           
	task_do_option 30326 1
endproc
proc 312          
	task_do_option 30327 1
endproc
proc 313           
	task_do_option 30328 1
endproc
proc 314           
	task_do_option 30329 1
endproc

proc 315           
	task_do_option 30343 1
endproc            
proc 316           
	task_do_option 30344 1
endproc            
proc 317           
	task_do_option 30345 1
endproc            
proc 318           
	task_do_option 30346 1
endproc            
proc 319           
	task_do_option 30347 1
endproc            
proc 320           
	task_do_option 30348 1
endproc            
proc 321           
	task_do_option 30349 1
endproc            
proc 322           
	task_do_option 30350 1
endproc            
proc 323           
	task_do_option 30351 1
endproc            
proc 324           
	task_do_option 30352 1
endproc            
proc 325           
	task_do_option 30353 1
endproc            
proc 326           
	task_do_option 30354 1
endproc            
proc 327           
	task_do_option 30355 1
endproc            
proc 328           
	task_do_option 30356 1
endproc            
proc 329           
	task_do_option 30357 1
endproc            
proc 330           
	task_do_option 30358 1
endproc            
proc 331           
	task_do_option 30359 1
endproc            
proc 332           
	task_do_option 30360 1
endproc            
proc 333           
	task_do_option 30361 1
endproc            
proc 334           
	task_do_option 30362 1
endproc            
proc 335           
	task_do_option 30363 1
endproc            
proc 336           
	task_do_option 30364 1
endproc            
proc 337           
	task_do_option 30365 1
endproc            
proc 338           
	task_do_option 30366 1
endproc            
proc 339           
	task_do_option 30367 1
endproc            
proc 340           
	task_do_option 30368 1
endproc            
proc 341           
	task_do_option 30369 1
endproc            
proc 342           
	task_do_option 30370 1
endproc            
proc 343           
	task_do_option 30371 1
endproc            
proc 344           
	task_do_option 30372 1
endproc            
proc 345           
	task_do_option 30373 1
endproc            
proc 346           
	task_do_option 30374 1
endproc            
proc 347           
	task_do_option 30375 1
endproc            
proc 348           
	task_do_option 30376 1
endproc            
proc 349           
	task_do_option 30377 1
endproc            
proc 350           
	task_do_option 30378 1
endproc            
proc 351           
	task_do_option 30379 1
endproc            
proc 352           
	task_do_option 30380 1
endproc            
proc 353           
	task_do_option 30381 1
endproc            
proc 354           
	task_do_option 30382 1
endproc            
proc 355           
	task_do_option 30383 1
endproc            
proc 356           
	task_do_option 30384 1
endproc            
proc 357           
	task_do_option 30385 1
endproc            
proc 358           
	task_do_option 30386 1
endproc            
proc 359           
	task_do_option 30387 1
endproc            
proc 360           
	task_do_option 30388 1
endproc            
proc 361           
	task_do_option 30389 1
endproc            
proc 362           
	task_do_option 30390 1
endproc            
proc 363           
	task_do_option 30391 1
endproc            
proc 364           
	task_do_option 30392 1
endproc            
proc 365           
	task_do_option 30393 1
endproc            
proc 366           
	task_do_option 30394 1
endproc            
proc 367           
	task_do_option 30395 1
endproc            
proc 368           
	task_do_option 30396 1
endproc            
proc 369           
	task_do_option 30397 1
endproc            
proc 370           
	task_do_option 30398 1
endproc            
proc 371           
	task_do_option 30399 1
endproc            
proc 372           
	task_do_option 30400 1
endproc            
proc 373           
	task_do_option 30401 1
endproc            
proc 374           
	task_do_option 30402 1
endproc            
proc 375           
	task_do_option 30403 1
endproc            
proc 376           
	task_do_option 30404 1
endproc            
proc 377           
	task_do_option 30405 1
endproc            
proc 378           
	task_do_option 30406 1
endproc            
proc 379           
	task_do_option 30407 1
endproc            
proc 380           
	task_do_option 30408 1
endproc            
proc 381           
	task_do_option 30409 1
endproc            
proc 382           
	task_do_option 30410 1
endproc            
proc 383           
	task_do_option 30411 1
endproc            
proc 384           
	task_do_option 30412 1
endproc            
proc 385           
	task_do_option 30413 1
endproc            
proc 386           
	task_do_option 30414 1
endproc            
proc 387           
	task_do_option 30415 1
endproc            
proc 388           
	task_do_option 30416 1
endproc            
proc 389           
	task_do_option 30417 1
endproc            
proc 390           
	task_do_option 30418 1
endproc            
proc 391           
	task_do_option 30419 1
endproc            
proc 392           
	task_do_option 30420 1
endproc            
proc 393           
	task_do_option 30421 1
endproc            
proc 394           
	task_do_option 30422 1
endproc            
proc 395           
	task_do_option 30423 1
endproc            
proc 396           
	task_do_option 30424 1
endproc            
proc 397           
	task_do_option 30425 1
endproc            
proc 398           
	task_do_option 30426 1
endproc            
proc 399           
	task_do_option 30427 1
endproc            
proc 400           
	task_do_option 30428 1
endproc            
proc 401           
	task_do_option 30429 1
endproc            
proc 402           
	task_do_option 30430 1
endproc            
proc 403           
	task_do_option 30431 1
endproc            
proc 404           
	task_do_option 30432 1
endproc            
proc 405           
	task_do_option 30433 1
endproc            
proc 406           
	task_do_option 30434 1
endproc            
proc 407           
	task_do_option 30435 1
endproc            
proc 408           
	task_do_option 30436 1
endproc            
proc 409           
	task_do_option 30437 1
endproc            
proc 410           
	task_do_option 30438 1
endproc            
proc 411           
	task_do_option 30439 1
endproc            
proc 412           
	task_do_option 30440 1
endproc            
proc 413           
	task_do_option 30441 1
endproc            
proc 414           
	task_do_option 30442 1
endproc            
proc 415           
	task_do_option 30443 1
endproc            
proc 416           
	task_do_option 30444 1
endproc           




proc 200
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	
	if lv >= 40 and lv < 45
		PLAYER_BUSY_UI 6 1
	endif
	if lv >= 45 and lv < 50
		PLAYER_BUSY_UI 7 1
	endif
	if lv >= 50 and lv < 55
		PLAYER_BUSY_UI 8 1
	endif
	if lv >= 55 and lv < 60
		PLAYER_BUSY_UI 9 1
	endif
	if lv >= 60 and lv < 65
		PLAYER_BUSY_UI 10 1
	endif
	if lv >= 65 and lv < 70
		PLAYER_BUSY_UI 11 1
	endif
	if lv >= 70 and lv < 75
		PLAYER_BUSY_UI 12 1
	endif
	if lv >= 75 and lv < 80
		PLAYER_BUSY_UI 13 1
	endif
	if lv >= 80
		PLAYER_BUSY_UI 29 1
	endif
endproc       

