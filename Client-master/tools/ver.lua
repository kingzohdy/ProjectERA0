require "lfs"
p = require "pl.path"
t = {}
if #arg == 4 then
	v1,v2,path,ver = arg[1],arg[2],arg[3],arg[4]
	if lfs.attributes( "d:/myworldvers/" .. ver ) == nil and lfs.attributes( "d:/过往版本/" .. ver ) == nil then
		local buildPath = string.gsub( path,"\\bin","\\client\\build\\build_client.sln" )
		local ret = os.execute( "buildConsole " .. buildPath .. " /rebuild /cfg=\"Deploy_Release|win32" )
		if ret ~= 0 then
			print( "编译错误！！！" )
			return
		end
		print( "svn.exe log " .. path .." -v -r " .. v1 .. ":" .. v2 .. " > d:\\log.txt" )
		os.execute( "svn.exe up " .. path )
		os.execute( "svn.exe log " .. path .." -v -r " .. v1 .. ":" .. v2 .. " > d:\\log.txt")
		f = io.open("d:\\log.txt","rt")
		for l in f:lines() do
			if string.find( l,"/bin/",1,true ) then
				start,pos = string.find(l,"/bin/")
				s = string.sub( l, pos )
				local bHas = false
				for i=1,#t do
					if t[i] == s then
						bHas = true
						break;
					end
				end
				if not bHas then
					table.insert(t,s)
				end
			end
		end
		lfs.mkdir( "d:/myworldvers/" .. ver )
		lfs.mkdir( "d:/过往版本/" .. ver )
		for i=1,#t do
			src = string.gsub( path .. t[i], "/" , "\\" )
			if lfs.attributes( src ) ~= nil and lfs.attributes( src ).mode ~= "directory" then
				des = string.gsub( "d:/myworldvers/" .. ver .. p.splitpath( t[i] ) .. "\\", "/", "\\" )
				os.execute( "xcopy " .. "\"" .. src .. "\" \"" .. des .. "\"")
				des = string.gsub( "d:/过往版本/" .. ver .. p.splitpath( t[i] ) .. "\\", "/", "\\" )
				os.execute( "xcopy " .. "\"" .. src .. "\" \"" .. des .. "\"")
			end
		end
		-- 加壳
		--os.execute( "d:/aspack/ASPack.exe " ..  string.gsub( "d:/myworldvers/" .. ver .. "\\", "/" , "\\" ).. "Client.exe" )
		-- 签名
		os.execute( "d:/signtool/sign.bat " ..  string.gsub( "d:/myworldvers/" .. ver .. "\\", "/" , "\\" ).. "Client.exe" )
		os.execute( "d:/signtool/sign.bat " ..  string.gsub( "d:/myworldvers/" .. ver .. "\\", "/" , "\\" ).. "offline2.exe" )
		os.execute( "d:/signtool/sign.bat " ..  string.gsub( "d:/myworldvers/" .. ver .. "\\", "/" , "\\" ).. "OgreMain.dll" )
		os.execute( "d:/signtool/sign.bat " ..  string.gsub( "d:/myworldvers/" .. ver .. "\\", "/" , "\\" ).. "RenderSystem_Direct3D9.dll" )
		os.execute( "d:/signtool/sign.bat " ..  string.gsub( "d:/myworldvers/" .. ver .. "\\", "/" , "\\" ).. "CrashSender1300.exe" )
		os.execute( "d:/signtool/sign.bat " ..  string.gsub( "d:/myworldvers/" .. ver .. "\\", "/" , "\\" ).. "CrashRpt1300.dll" )
		os.execute( "d:/signtool/sign.bat " ..  string.gsub( "d:/myworldvers/" .. ver .. "\\", "/" , "\\" ).. "Launcher.exe" )
		os.execute( "d:/clean.bat " .. ver )
		
		os.execute( "echo [version] > d:/verTest/version.txt" )
		os.execute( "echo.ver = " .. ver .. " >> d:/verTest/version.txt")
		os.execute( "d:/verTest/DesFile.exe d:/verTest/version.txt" )
		os.execute( "copy d:\\verTest\\version.data " .. string.gsub( "d:/myworldvers/" .. ver .. "\\", "/" , "\\" ) .. "version.data" )
	else
		print( "版本错误，目录已存在" )
	end
else
	print("请输入正确的参数: 起始svn版本号 补丁svn版本号 bin路径 版本 \nExample：lua ver.lua 40000 41000 d:\\deployed\\bin 3.0.10")
end

