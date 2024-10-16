rollout  Ogrerollout  "Ogreanimation" 
(
	listbox Ogrebox  "motion clip" height:18 width:200
	button display "showclip" pos:[230,30] width:60 height:30
	button test "test" pos:[230,100] width:60 height:30
	on test pressed do
	(
		Ogrebox.items=#()
	)
	
	fn helpitem cc string=
	(
		item=finditem cc string
		item=item+1
		cc[item]
	)
		local test
		
	on display pressed do
	(
		if classof $==Biped_Object then
		(
			obj=biped.getNode $ #vertical
			numN=numNoteTracks obj
			for i=1 to numN do
			(
				nn=getnotetrack obj i
				if nn.keys.count>1 then exit
			)
			Ogrebox.items=#()
			nn.keys
			allitem=#()
			for i=1 to nn.keys.count do
			(
				boxitem=#()
				cc=#()
				dd=#()
				cc=filterstring nn.keys[i].value " "
				test=finditem cc "OgreStart"
				if test==1 then
				(
					ii=i+1
					cc=filterstring nn.keys[i].value " "
					dd=filterstring nn.keys[ii].value " "
					Mname=helpitem cc "-name"
					Mloop=helpitem cc "-loop"
					Mid=helpitem dd "-seqID"
					Mstring=nn.keys[i].time as string+"      "+nn.keys[ii].time as string
					Mstring+="      "+Mname+"      "+Mid+"    "+Mloop
					append allitem Mstring
				)
			)
			--print allitem
			
			for q=1 to allitem.count do
			(
				if q==1 then
				(
					inform="start   end     name       ID      loop"
					Ogrebox.items=append Ogrebox.items inform
				)
				notstring=allitem[q]
				Ogrebox.items=append Ogrebox.items notstring
			)
		)
	)
)
createdialog Ogrerollout width:300 height:300