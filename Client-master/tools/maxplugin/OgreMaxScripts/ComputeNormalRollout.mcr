
global dummypoint
global obj
cmpnormal = newRollOutFloater "自动生成法线工具" 154 220
utility ComputeNormalRollout "自动生成法线工具" width:140 height:220
(
	label lbl1 "dummy节点命名为:normal_a, normal_b ......" pos:[2,10] width:140 height:40
	label lbl2 "d树叶的mesh命名为:a_01,a_02 ......" pos:[2,50] width:140 height:40
	button cal "计算法线" pos:[36,110] width:75 height:30
	label lbl3 "2009-9-12" pos:[2,180] width:140 height:40
	
	on cal pressed  do
	(
		for i in objects do
		(
			names = filterString i.name "_"
			for j=1 to names.count do
			(
				if(names[j] == "normal") do
				(
					dummypoint = i.center
				)	
			)
		)
		
		for i in Geometry do
		(
			names = filterString i.name "_"
			for j=1 to names.count do
			(
				if(names[j] == "a") then
				(
					obj=i
					convertToMesh obj
					collapseStack obj
					select obj
					addModifier obj (Edit_Normals())
					max modify mode
					subobjectLevel = 1
					modPanel.setCurrentObject $.modifiers[#Edit_Normals]
					num_verts = obj.numverts
					num_faces = obj.numfaces					
					for v = 1 to num_faces do
					(
						face = getFace obj v
						for k=1 to 3 do
						(
							local vert
							if (k==1) do 
							(
								vert = getVert obj Face.x
							)
							if (k==2) do 
							(
								vert = getVert obj Face.y
							)
							if (k==3) do
							(
								vert = getVert obj Face.z
							)							
							normalid = $.modifiers[#Edit_Normals].GetNormalID v k
							$.modifiers[#Edit_Normals].selectBy = 1
							$.modifiers[#Edit_Normals].EditNormalsMod.Select #{normalid}
							normal = vert
							normal = normal - dummypoint
							normal = normalize normal
							$.modifiers[#Edit_Normals].EditNormalsMod.setnormal normalid normal
							$.modifiers[#Edit_Normals].EditNormalsMod.MakeExplicit()
						)
						
					)
				)else exit
			)
		)
	)
)
addrollout ComputeNormalRollout cmpnormal