rollout MyRollout "NoteTrackManager" width:233 height:408
(
	listbox animlist "动画列表" pos:[17,32] width:186 height:18
	button SaveNT "保存当前文件的NoteTrack" pos:[25,328] width:160 height:21
	button ReadNT "读取文件的NoteTrack" pos:[25,352] width:160 height:21
	button RemNoteTrack "删除所有的NoteTrack" pos:[25,376] width:160 height:21
	label lbl1 "Ogre NoteTrack的管理" pos:[53,7] width:165 height:24
	button refreshbtn "刷新动画列表" pos:[103,24] width:94 height:19
	button StartPlay "开始播放" pos:[31,296] width:70 height:21
	button StopPlay "停止播放" pos:[116,295] width:70 height:21

	on StartPlay pressed do
	(
		PlayAnimation immediateReturn:true
	)	
	
	on StopPlay pressed do
	(
		StopAnimation()
	)	
	on animlist doubleClicked itm do
	(
		StopAnimation()

		strings = filterstring animlist.items[itm] "----"
		
		local starfrval
		local endfrval
		
		value = ""
		index = 1
		for string in strings do
		(
			if (index == 2) do
			(
				starfrval = strings[index] as float
			)
			
			if (index == 3) do
			(
				endfrval = strings[index] as float
			)
			
	
			index = index + 1
		)
		
		if starfrval == endfrval do endfrval += 1
		animationRange = interval starfrval endfrval
		redrawviews()
	)
	on SaveNT pressed do
	(	
		out_name = getSaveFileName types:"Data(*.dat)|*.dat"
		
		if out_name != undefined do
		(
			out_file = createfile out_name
			if out_file != undefined do
			(
				for obj in objects do
				(
					count = numNoteTracks obj
					if(count > 0) do
					(
						format "%" obj.name to:out_file
						format "\n" to: out_file
									
						for i=1 to count do
						(
							nt = getNoteTrack obj i
								index = 1
								for key in nt.keys do
								(
									if (((findString (key.value as string)  "OgreBegin") != undefined) or ((findString (key.value as string) "OgreEnd") != undefined)) then
									(
										time = getNoteKeyTime nt index
										index = index + 1
										format "%" time to:out_file
										format "\n" to: out_file
			
										format "%" key.value to:out_file
										format "\n" to: out_file									
									)
								)
						)
						close out_file
					)
				)			
			)
		)
	)
	on ReadNT pressed do
	(
		for obj in objects do
		(
			count = numNoteTracks obj
			if(count > 0) do
			(
				for i=1 to count do
				(
					k = getNoteTrack obj (count+1-i)
					
					deleteNoteTrack obj k
				)
			)
		)
	
		in_name = getOpenFileName types:"Data(*.dat)|*.dat|Excel(*.csv)|*.csv|All|*.*|"
		if in_name != undefined do
		(
			in_file = openfile in_name
			if in_file!=undefined do
			(
				temp= readline in_file
				bFound = false
				for obj in objects do
				(
					if (obj.name == temp) do
					(
						bFound = true
					
						nt = notetrack "ActorManager"
						addNoteTrack obj nt					
						while not eof in_file do
						(
							temp= readline in_file
							
							if temp != "" do
							(
								key = addNewNoteKey nt.keys (temp as float)
								
								temp = readline in_file
								if temp != "" do
								(
									key.value = temp
								)								
							)
						)						
					)
				)
		
				if (bFound == false) do
				(
					messagebox "该Max文件与保存notetrack的文件不同"
				)
			)
		)		
	)
	on RemNoteTrack pressed do
	(	
		for obj in objects do
		(
			count = numNoteTracks obj
			if(count > 0) do
			(
				for i=1 to count do
				(
					k = getNoteTrack obj (count+1-i)
					
					deleteNoteTrack obj k
				)
			)
		)
	)
	on refreshbtn pressed do
	(
		allitem=#()
		animlist.items = #()
		anchoridlist = #()
	
		for obj in objects do
		(
			count = numNoteTracks obj
			if(count > 0) do
			(
				item = ""	
				for i=1 to count do
				(
					nt = getNoteTrack obj i
					index = 1
					for key in nt.keys do
					(
						if (((findString (key.value as string)  "OgreBegin") != undefined)) do
						(
							time = getNoteKeyTime nt index
							
							strings = filterstring key.value " "
							id = 1
							for string in strings do
							(
								--print string
								--bEquals = EqualsNoCase string "-name"
								if (string == "-name") then
								(
									item = item + strings[id+1]
									item = item + "----"
									break
								)
								id = id + 1
							)
							
							key.value
							item = item + time as String
							item = item + "----"
							index = index + 1
						)
						
						if (((findString (key.value as string) "OgreEnd") != undefined)) do
						(
							time = getNoteKeyTime nt index
							item = item + time as String
							
							append allitem item
							item = ""
							index = index + 1						
						)
					)
				)
				break
			)
		)
		
		for q=1 to allitem.count do
		(
			itemq = allitem[q]
			animlist.items = append animlist.items itemq
		)
	
	)
)createdialog MyRollout