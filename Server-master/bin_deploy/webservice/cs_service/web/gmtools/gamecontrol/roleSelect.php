<?php 
	require_once(dirname(__FILE__).'/../inc/config.inc.php'); 
//检测用户是否有权限
if($_SESSION['admin1'] == "")
{
	$_SESSION = array();
	session_destroy();	
	echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
	exit();
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml"><head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>输入角色查找账号</title>
<link href="style/admin.css" rel="stylesheet" type="text/css" />
<style type="text/css">
 a{ color:#00F;}
.thickdiv{position:fixed;top:0;left:0;z-index:10000001;width:100%;height:100%;background:#F7F7F7;border:0;filter:alpha(opacity=70);opacity:0.70;}
.thickbox{position:absolute;z-index:100000002;overflow:hidden;box-shadow:0 10px 8px #6c6c6c; background-color:#fff;}
.thicktitle{height:30px;padding:0 15px; margin-top:8px; text-align:left;line-height:27px;font-family:arial,"宋体";font-size:14px;font-weight:bold;color:#F00;}

.thickclose:link,.thickclose:visited{display:block;position:absolute;z-index:100000;top:10px;right:12px;overflow:hidden;width:21px;height:21px;background:url(images/close_ico_dis.gif) no-repeat 0 3px;font-size:0;line-height:100px;}
.thickclose:hover{ background-position:left -18px;}
.thickcon{overflow:auto;padding:10px;text-align:center; vertical-align:top;}
</style>
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="js/jquery.kangThickBoxV2.js" ></script>
<script type="text/javascript" src="js/dateInt.js"></script>
<script type="text/javascript" >
var FindId=-1;//当前句柄
var RegArray=new Array(); //查询出来的所有结果.-1表示未查询
var page = 0; //当前页
var pageSum = 0; //最后一页的页数
var tp = "";//判断用户类型
function jingyan(AccName,q)
{
	var qu=q;
	var AccName = AccName;
	$.kangThickBox({
		type: "iframe",
		title:"禁言账号",
		source: 'SilenceAccount.php?q='+qu+'&&AccName='+AccName+'',
		width: 240,
		height: 100,
		_autoReposi: true
	});
}
function dongjie(AccName,q)
{
	var qu=q;
	var AccName = AccName;
	$.kangThickBox({
		type: "iframe",
		title:"冻结账号",
		source: 'FreezeAccount.php?FreezeAccount=0&&q='+qu+'&&AccName='+AccName+'',
		width: 300,
		height: 260,
		_autoReposi: true
	});
}
function fck(AccName,q)
{
	var qu=q;
	var AccName = AccName;
	$.kangThickBox({
		type: "iframe",
		title:"修改安全锁密码",
		source: 'ContainerLock.php?q='+qu+'&&AccName='+AccName+'',
		width: 240,
		height: 100,
		_autoReposi: true
	});
}
function coord()
{
	$.kangThickBox({
		type: "iframe",
		title:"修改坐标",
		source: 'UpdateCoord.php',
		width: 200,
		height: 170,
		_autoReposi: true
	});
}
function mingxi()
{
	//代表账户类型()
	tp = $("#tp").val();
	RegArray = Array();		
	page = 0;
	var d = '';
	var cfs = $(":radio.[name='cxfs']:checked").val(); // 1.精确查 2,.模糊查
	var rolename=$("#account").val();
	if(FindId != -1)
	{
		//关闭句柄
		$.ajax
		({
			type: 'POST',
			url: 'conduct.php',
			dataType: 'json',
			data: {"FindIdColse":FindId},
			async:false,
			beforeSend:function(){
				$("#BgDiv").css({ display: "block", height: $(document).height() });
				$('#DialogDiv').show();
			},
			success: function(array){
				$('#DialogDiv').hide();
				$("#BgDiv").hide();
				if(array == 0)
				{
					FindId = -1;
				}
				else if(array.ErrorCode)
				{
					alert("句柄关闭错误\nErrorCode: "+array.ErrorCode+"\nErrorText: "+array.ErrorText);
				}
			}
		});
	}
	$.ajax
	({
			type: 'POST',
			url: 'conduct.php',
			dataType: 'json',
			async:false,
			data: {"roleselect1":"","rolename": rolename,"cfs":cfs},
			beforeSend:function(){
				$("#BgDiv").css({ display: "block", height: $(document).height() });
				$('#DialogDiv').show();
			}, 
			success: function(array)
			{
				var msg = array.data;
				$('#DialogDiv').hide();
				$("#BgDiv").hide();
				$("#ajaxlist").empty();
				if(array.ErrorCode == -6)
				{
					alert("未找到......");
				}
				else if(array.ErrorCode)
				{
					alert("ErrorCode: "+array.ErrorCode+"\nErrorText: "+array.ErrorText);
				}

				if(msg.ResNum <= 0)
				{
					alert("未找到");
				}
				if(msg.ResNum == 1)
				{
					msg.ReginID = parseInt(msg.ReginID);
					if(JSON.stringify(msg.AccountLiteArray.FreezeReason) == "{}"){ msg.AccountLiteArray.FreezeReason = "未冻结";}
					//[<a href="rolelist.php?account='+msg.ErrorMessage+'" >角色列表</a>]新加...
					d ='<tr align=center ><td  height=32 width="10%">'+msg.AccountLiteArray.Pass9Account+'</td><td  height=32 width="10%">'+msg.AccountLiteArray.Uin+'</td><td  height=32 width="10%">'+date('Y-m-d H:i:s',msg.AccountLiteArray.AccCreateTime)+'</td><td  height=32 width="10%">'+msg.AccountLiteArray.FreezeReason+'</td><td  height=32 width="10%">'+date('Y-m-d H:i:s',msg.AccountLiteArray.FreezeEndTime)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray.FreezeStartTime)+'</td><td width="10%" >'+numberToIp(msg.AccountLiteArray.LastLoginIP)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray.LastLoginTime)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray.SilenceEndTime)+'</td>';
				 if("高级游戏管理员"==tp || "超级游戏管理员"==tp)
					{
						//[<a href="#" class="box a2" onclick="fck()"  >改安全锁</a>]第二版新加
						d += '<td width="10%" >[<a href="#" onclick = jingyan("'+msg.AccountLiteArray.Pass9Account+'","'+msg.ReginID+'") >禁言</a>][<a href="#" onclick = dongjie("'+msg.AccountLiteArray.Pass9Account+'","'+msg.ReginID+'") >冻结此帐号</a>][<a href="#" onclick = sfyz("'+msg.AccountLiteArray.Pass9Account+'") >修改安全邮箱</a>]';
					}
					if("超级游戏管理员"==tp)
					{
						d += '[<a href="#" onclick=fck("'+msg.AccountLiteArray.Pass9Account+'","'+msg.ReginID+'")  >改安全锁</a>]'
					}
					if("高级游戏管理员"==tp || "超级游戏管理员"==tp)
					{
						d += '</td>';
					}
				 d+='</tr>';
				}
				if(msg.ResNum > 1)
				{
					msg.ReginID = parseInt(msg.ReginID);
					for(var i = 0; i < msg.AccountLiteArray.length; i++){
						if(JSON.stringify(msg.AccountLiteArray[i].FreezeReason) == "{}"){ msg.AccountLiteArray[i].FreezeReason = "未冻结";}
						d +='<tr align=center ><td  height=32 width="10%">'+msg.AccountLiteArray[i].Pass9Account+'</td><td  height=32 width="10%">'+msg.AccountLiteArray[i].Uin+'</td><td  height=32 width="10%">'+date('Y-m-d H:i:s',msg.AccountLiteArray[i].AccCreateTime)+'</td><td  height=32 width="10%">'+msg.AccountLiteArray[i].FreezeReason+'</td><td  height=32 width="10%">'+date('Y-m-d H:i:s',msg.AccountLiteArray[i].FreezeEndTime)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray[i].FreezeStartTime)+'</td><td width="10%" >'+numberToIp(msg.AccountLiteArray[i].LastLoginIP)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray[i].LastLoginTime)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray[i].SilenceEndTime)+'</td>';
					 if("高级游戏管理员"==tp || "超级游戏管理员"==tp)
						{
							//[<a href="#" class="box a2" onclick="fck()"  >改安全锁</a>]第二版新加
							d += '<td width="10%" >[<a href="#" onclick = jingyan("'+msg.AccountLiteArray[i].Pass9Account+'","'+msg.ReginID+'") >禁言</a>][<a href="#" onclick = dongjie("'+msg.AccountLiteArray[i].Pass9Account+'","'+msg.ReginID+'") >冻结此帐号</a>][<a href="#" onclick = sfyz("'+msg.AccountLiteArray[i].Pass9Account+'") >修改安全邮箱</a>]';
						}
						if("超级游戏管理员"==tp)
						{
							d += '[<a href="#" onclick=fck("'+msg.AccountLiteArray[i].Pass9Account+'","'+msg.ReginID+'")  >改安全锁</a>]'
						}
						if("高级游戏管理员"==tp || "超级游戏管理员"==tp)
						{
							d += '</td>';
						}
					 d+='</tr>';
					}
				}
				$("#ajaxlist").append(d);
				RegArray[page] = array.data;
				//alert(RegArray[page].AccountLiteArray[0].Pass9Account);
				page=1;//当前页
				pageSum=1; // 最后一页的页数
				FindId=array.FindHandleID;//句柄赋值
			},
		error:function(array)
		{
			alert(msg+'<br/>发送数据出错');
		}
	});
}
function pup()
{
	var d = '';
	if(page == 0)
	{
		alert("请输入角色查询");
	}
	if(page == 1)
	{
		alert("已在第一页");
	}
	if(page > 1)
	{
		var msg = RegArray[page-2];
		msg.ReginID = parseInt(msg.ReginID);
		$("#ajaxlist").empty();
		if(msg.ResNum == 1)
		{
			if(JSON.stringify(msg.AccountLiteArray.FreezeReason) == "{}"){ msg.AccountLiteArray.FreezeReason = "未冻结";}
			d ='<tr align=center ><td  height=32 width="10%">'+msg.AccountLiteArray.Pass9Account+'</td><td  height=32 width="10%">'+msg.AccountLiteArray.Uin+'</td><td  height=32 width="10%">'+date('Y-m-d H:i:s',msg.AccountLiteArray.AccCreateTime)+'</td><td  height=32 width="10%">'+msg.AccountLiteArray.FreezeReason+'</td><td  height=32 width="10%">'+date('Y-m-d H:i:s',msg.AccountLiteArray.FreezeEndTime)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray.FreezeStartTime)+'</td><td width="10%" >'+numberToIp(msg.AccountLiteArray.LastLoginIP)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray.LastLoginTime)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray.SilenceEndTime)+'</td>';
			 if("高级游戏管理员"==tp || "超级游戏管理员"==tp)
				{
					//[<a href="#" class="box a2" onclick="fck()"  >改安全锁</a>]第二版新加
					d += '<td width="10%" >[<a href="#" onclick = jingyan("'+msg.AccountLiteArray.Pass9Account+'","'+msg.ReginID+'") >禁言</a>][<a href="#" onclick = dongjie("'+msg.AccountLiteArray.Pass9Account+'","'+msg.ReginID+'") >冻结此帐号</a>][<a href="#" onclick = sfyz("'+msg.AccountLiteArray.Pass9Account+'") >修改安全邮箱</a>]';
				}
				if("超级游戏管理员"==tp)
				{
					d += '[<a href="#" onclick=fck("'+msg.AccountLiteArray.Pass9Account+'","'+msg.ReginID+'")  >改安全锁</a>]'
				}
				if("高级游戏管理员"==tp || "超级游戏管理员"==tp)
				{
					d += '</td>';
				}
			 d+='</tr>';
		}
		if(msg.ResNum > 1)
		{
			for(var i = 0; i < msg.AccountLiteArray.length; i++)
			{
				if(JSON.stringify(msg.AccountLiteArray[i].FreezeReason) == "{}"){ msg.AccountLiteArray[i].FreezeReason = "未冻结";}
				d +='<tr align=center ><td  height=32 width="10%">'+msg.AccountLiteArray[i].Pass9Account+'</td><td  height=32 width="10%">'+msg.AccountLiteArray[i].Uin+'</td><td  height=32 width="10%">'+date('Y-m-d H:i:s',msg.AccountLiteArray[i].AccCreateTime)+'</td><td  height=32 width="10%">'+msg.AccountLiteArray[i].FreezeReason+'</td><td  height=32 width="10%">'+date('Y-m-d H:i:s',msg.AccountLiteArray[i].FreezeEndTime)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray[i].FreezeStartTime)+'</td><td width="10%" >'+numberToIp(msg.AccountLiteArray[i].LastLoginIP)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray[i].LastLoginTime)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray[i].SilenceEndTime)+'</td>';
			 if("高级游戏管理员"==tp || "超级游戏管理员"==tp)
				{
					//[<a href="#" class="box a2" onclick="fck()"  >改安全锁</a>]第二版新加
					d += '<td width="10%" >[<a href="#" onclick = jingyan("'+msg.AccountLiteArray[i].Pass9Account+'","'+msg.ReginID+'") >禁言</a>][<a href="#" onclick = dongjie("'+msg.AccountLiteArray[i].Pass9Account+'","'+msg.ReginID+'") >冻结此帐号</a>][<a href="#" onclick = sfyz("'+msg.AccountLiteArray[i].Pass9Account+'") >修改安全邮箱</a>]';
				}
				if("超级游戏管理员"==tp)
				{
					d += '[<a href="#" onclick=fck("'+msg.AccountLiteArray[i].Pass9Account+'","'+msg.ReginID+'")  >改安全锁</a>]'
				}
				if("高级游戏管理员"==tp || "超级游戏管理员"==tp)
				{
					d += '</td>';
				}
				d+='</tr>';
			}
		}
		$("#ajaxlist").append(d);
		page-=1;//当前页
	}
}
function pnext()
{
	var d = '';
	if(page == 0)
	{
		alert("请输入角色查询");
	}
	if(page > 0 && page == RegArray.length)
	{
		$.ajax
		({
			type: 'POST',
			url: 'conduct.php',
			dataType: 'json',
			data: {"roleselect1np":"","page": FindId}, 
			beforeSend:function(){
				$("#BgDiv").css({ display: "block", height: $(document).height() });
				$('#DialogDiv').show();
			 },
			success: function(array)
			{
				var msg = array.data;
				$('#DialogDiv').hide();
				$("#BgDiv").hide();
				if(array.ErrorCode == -6)
				{
					alert("没有了......");
				}
				/*if(array.ErrorCode == -4)
				{
					alert("参数传递错误");
				}*/
				else if(array.ErrorCode)
				{
					alert("ErrorCode: "+array.ErrorCode+"\nErrorText: "+array.ErrorText);
				}
				if(msg.ResNum <= 0)
				{
					alert("无结果"); 
				}
				if(msg.ResNum == 1)
				{
					msg.ReginID = parseInt(msg.ReginID);
					$("#ajaxlist").empty();
					//alert(date('Y-m-d H:i:s',msg.AccountLiteArray.AccCreateTime));
					if(JSON.stringify(msg.AccountLiteArray.FreezeReason) == "{}"){ msg.AccountLiteArray.FreezeReason = "未冻结";}
					d ='<tr align=center ><td  height=32 width="10%">'+msg.AccountLiteArray.Pass9Account+'</td><td  height=32 width="10%">'+msg.AccountLiteArray.Uin+'</td><td  height=32 width="10%">'+date('Y-m-d H:i:s',msg.AccountLiteArray.AccCreateTime)+'</td><td  height=32 width="10%">'+msg.AccountLiteArray.FreezeReason+'</td><td  height=32 width="10%">'+date('Y-m-d H:i:s',msg.AccountLiteArray.FreezeEndTime)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray.FreezeStartTime)+'</td><td width="10%" >'+numberToIp(msg.AccountLiteArray.LastLoginIP)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray.LastLoginTime)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray.SilenceEndTime)+'</td>';
				 if("高级游戏管理员"==tp || "超级游戏管理员"==tp)
					{
						//[<a href="#" class="box a2" onclick="fck()"  >改安全锁</a>]第二版新加
						d += '<td width="10%" >[<a href="#" onclick = jingyan("'+msg.AccountLiteArray.Pass9Account+'","'+msg.ReginID+'") >禁言</a>][<a href="#" onclick = dongjie("'+msg.AccountLiteArray.Pass9Account+'","'+msg.ReginID+'") >冻结此帐号</a>][<a href="#" onclick = sfyz("'+msg.AccountLiteArray.Pass9Account+'") >修改安全邮箱</a>]';
					}
					if("超级游戏管理员"==tp)
					{
						d += '[<a href="#" onclick=fck("'+msg.AccountLiteArray.Pass9Account+'","'+msg.ReginID+'")  >改安全锁</a>]'
					}
					if("高级游戏管理员"==tp || "超级游戏管理员"==tp)
					{
						d += '</td>';
					}
						d+='</tr>';
					}
				if(msg.ResNum > 1)
				{
					msg.ReginID = parseInt(msg.ReginID);
					$("#ajaxlist").empty();
					for(var i = 0; i < msg.AccountLiteArray.length; i++){
						if(JSON.stringify(msg.AccountLiteArray[i].FreezeReason) == "{}"){ msg.AccountLiteArray[i].FreezeReason = "未冻结";}
						d +='<tr align=center><td  height=32 width="10%">'+msg.AccountLiteArray[i].Pass9Account+'</td><td  height=32 width="10%">'+msg.AccountLiteArray[i].Uin+'</td><td  height=32 width="10%">'+date('Y-m-d H:i:s',msg.AccountLiteArray[i].AccCreateTime)+'</td><td  height=32 width="10%">'+date('Y-m-d H:i:s',msg.AccountLiteArray[i].AccCreateTime)+'</td><td  height=32 width="10%">'+msg.AccountLiteArray[i].FreezeReason+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray[i].FreezeStartTime)+'</td><td width="10%" >'+numberToIp(msg.AccountLiteArray[i].LastLoginIP)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray[i].LastLoginTime)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray[i].SilenceEndTime)+'</td>';
					 if("高级游戏管理员"==tp || "超级游戏管理员"==tp)
						{
							//[<a href="#" class="box a2" onclick="fck()"  >改安全锁</a>]第二版新加
							d += '<td width="10%" >[<a href="#" onclick = jingyan("'+msg.AccountLiteArray[i].Pass9Account+'","'+msg.ReginID+'") >禁言</a>][<a href="#" onclick = dongjie("'+msg.AccountLiteArray[i].Pass9Account+'","'+msg.ReginID+'") >冻结此帐号</a>][<a href="#" onclick = sfyz("'+msg.AccountLiteArray[i].Pass9Account+'") >修改安全邮箱</a>]';
						}
						if("超级游戏管理员"==tp)
						{
							d += '[<a href="#" onclick=fck("'+msg.AccountLiteArray[i].Pass9Account+'","'+msg.ReginID+'")  >改安全锁</a>]'
						}
						if("高级游戏管理员"==tp || "超级游戏管理员"==tp)
						{
							d += '</td>';
						}
						d+='</tr>';
					}
				}
				$("#ajaxlist").append(d);
				RegArray[page] = array.data;
				page+=1;//当前页
			},
			error:function(array)
			{
				alert(msg+'<br/>发送数据出错，请重新发送');
			}
		});
	}
	else if(page > 0 && page < RegArray.length)
	{
		var msg = RegArray[page];
		$("#ajaxlist").empty();
		if(msg.ResNum == 1)
		{
			//alert(date('Y-m-d H:i:s',msg.AccountLiteArray.AccCreateTime));
			msg.ReginID = parseInt(msg.ReginID);
			if(JSON.stringify(msg.AccountLiteArray.FreezeReason) == "{}"){ msg.AccountLiteArray.FreezeReason = "未冻结";}
			d ='<tr align=center ><td  height=32 width="10%">'+msg.AccountLiteArray.Pass9Account+'</td><td  height=32 width="10%">'+msg.AccountLiteArray.Uin+'</td><td  height=32 width="10%">'+date('Y-m-d H:i:s',msg.AccountLiteArray.AccCreateTime)+'</td><td  height=32 width="10%">'+msg.AccountLiteArray.FreezeReason+'</td><td  height=32 width="10%">'+date('Y-m-d H:i:s',msg.AccountLiteArray.FreezeEndTime)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray.FreezeStartTime)+'</td><td width="10%" >'+numberToIp(msg.AccountLiteArray.LastLoginIP)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray.LastLoginTime)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray.SilenceEndTime)+'</td>';
			 if("高级游戏管理员"==tp || "超级游戏管理员"==tp)
				{
					//[<a href="#" class="box a2" onclick="fck()"  >改安全锁</a>]第二版新加
					d += '<td width="10%" >[<a href="#" onclick = jingyan("'+msg.AccountLiteArray.Pass9Account+'","'+msg.ReginID+'") >禁言</a>][<a href="#" onclick = dongjie("'+msg.AccountLiteArray.Pass9Account+'","'+msg.ReginID+'") >冻结此帐号</a>][<a href="#" onclick = sfyz("'+msg.AccountLiteArray.Pass9Account+'") >修改安全邮箱</a>]';
				}
				if("超级游戏管理员"==tp)
				{
					d += '[<a href="#" onclick=fck("'+msg.AccountLiteArray.Pass9Account+'","'+msg.ReginID+'")  >改安全锁</a>]'
				}
				if("高级游戏管理员"==tp || "超级游戏管理员"==tp)
				{
					d += '</td>';
				}
				d+='</tr>';
		}
		if(msg.ResNum > 1)
		{
			$("#ajaxlist").empty();
			msg.ReginID = parseInt(msg.ReginID);
			for(var i = 0; i < msg.AccountLiteArray.length; i++)
			{
				if(JSON.stringify(msg.AccountLiteArray[i].FreezeReason) == "{}"){ msg.AccountLiteArray[i].FreezeReason = "未冻结";}
				d +='<tr align=center ><td  height=32 width="10%">'+msg.AccountLiteArray[i].Pass9Account+'</td><td  height=32 width="10%">'+msg.AccountLiteArray[i].Uin+'</td><td  height=32 width="10%">'+date('Y-m-d H:i:s',msg.AccountLiteArray[i].AccCreateTime)+'</td><td  height=32 width="10%">'+msg.AccountLiteArray[i].FreezeReason+'</td><td  height=32 width="10%">'+date('Y-m-d H:i:s',msg.AccountLiteArray[i].FreezeEndTime)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray[i].FreezeStartTime)+'</td><td width="10%" >'+numberToIp(msg.AccountLiteArray[i].LastLoginIP)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray[i].LastLoginTime)+'</td><td width="10%" >'+date('Y-m-d H:i:s',msg.AccountLiteArray[i].SilenceEndTime)+'</td>';
			 if("高级游戏管理员"==tp || "超级游戏管理员"==tp)
				{
					//[<a href="#" class="box a2" onclick="fck()"  >改安全锁</a>]第二版新加
					d += '<td width="10%" >[<a href="#" onclick = jingyan("'+msg.AccountLiteArray[i].Pass9Account+'","'+msg.ReginID+'") >禁言</a>][<a href="#" onclick = dongjie("'+msg.AccountLiteArray[i].Pass9Account+'","'+msg.ReginID+'") >冻结此帐号</a>][<a href="#" onclick = sfyz("'+msg.AccountLiteArray[i].Pass9Account+'") >修改安全邮箱</a>]';
				}
				if("超级游戏管理员"==tp)
				{
					d += '[<a href="#" onclick=fck("'+msg.AccountLiteArray[i].Pass9Account+'","'+msg.ReginID+'")  >改安全锁</a>]'
				}
				if("高级游戏管理员"==tp || "超级游戏管理员"==tp)
				{
					d += '</td>';
				}
				d+='</tr>';
			}
		}
				$("#ajaxlist").append(d);
				page+=1;//当前页
	}
}
//修改安全邮箱
function sfyz(acc)
{
	acct = acc;
	$("#BgDiv").css({ display: "block", height: $(document).height() });
	$("#DialogDivIDC").show();
}
function upel()
{
	var sfz = $("#sfz").val();
	if(sfz.length != 18 && sfz.length != 15)
	{
		$("#DialogDivIDC").hide();
		$("#BgDiv").hide();
		exit;
	}
	else
	{
		$.ajax
		({
			url: 'safetyEmail.php',
			type: 'post',
			dataType: 'json',
			data: {"idc":sfz,"acc":acct}, 
			success: function(msg)
			{
				$("#DialogDivIDC").hide();
				$("#BgDiv").hide();
				if(msg > 2)
				{
					alert("验证失败");
					exit;
				}
				else if(msg <= 2)
				{
					if(msg>0)
					{
						alert("错 "+msg+" 位身份证号码,请谨慎修改");
					}
					upmail(acct,23);
					exit;
				}
			}
		});
	}
}
function upmail(AccName,bj)
{
	var biaoji=bj;
	var AccName = AccName;
	if(biaoji == 23){
		$.kangThickBox({
			type: "iframe",
			title:"修改安全邮箱",
			source: 'safetyEmail.php?AccName='+AccName+'',
			width: 240,
			height: 100,
			_autoReposi: true
		});
	}
	else
	{
		alert("非法访问");
	}
}
</script>
<style type="text/css">  
#BgDiv{background-color:#F7F7F7; position:absolute; z-index:99; left:0; top:0; display:none; width:100%; height:100%;opacity:0.5;filter: alpha(opacity=50);-moz-opacity: 0.5;} 
#DialogDiv{position:absolute;width:300px; left:55%; top:20%; margin-left:-200px; height:50px; z-index:100; border:0px #ccc solid; padding:1px;}
#DialogDivIDC{position:absolute;width:240px;height:120px; left:35%; top:20%;z-index:100000002;overflow:hidden;box-shadow:0 10px 8px #6c6c6c; background-color:#fff;}
</style>
</head>
<body>
<div id="BgDiv"></div>
<div id="DialogDiv" style="display:none">
	<h2 style="color:#F00">查询中,请稍等...</h2>
</div>
<div id="DialogDivIDC" style="display:none">
	<table width="100%" height="100%">
    <tr align="center"><td  style="color:#F00; font-size:16px;">请输入该账户的身份证：</td></tr>
    <tr align="center"><td><input type="text" id="sfz" style="width:70%"/></td></tr>
    <tr align="center"><td><input type="button" value="确定" style="width:40px; height:25px;" onclick="upel()" /></td></tr></table>
</div>
<div id="DialogDiv" style="display:none">
	<div style="background:#FFF; border:1px solid #39B8C6; height:370px; width:450px;">
    <center style=" margin-top:5px;font-size:16px">订单详情</center>
    <br />
    <table style="padding:5px;" width="98%" id="mx">
    </table>
    </div>
</div>
<div class="mgr_divt">
<table width="100%" height="100%">
	<tr>
		<td style="float:right;vertical-align:top; margin-top:8px;" >
        	请输入角色名:&nbsp;&nbsp;
        </td>
         <td style=" padding:0px; margin:0px;vertical-align:top; ">
				<div id="search" class="search" style="float:left;vertical-align:middle;"> <span class="s">
				<input style="float:left;vertical-align:middle;" name="account"  id="account" type="text" title="输入角色进行搜索"/>
				</span> <span class="b"><a href="javascript:;" onclick="mingxi()"><img src="images/search_btn.png" title="搜索" /></a></span></div>
            <span style="float:left;vertical-align:top; margin-top:6px;" ><input type="radio" value="1" name="cxfs" />&nbsp;&nbsp;精确查&nbsp;&nbsp;<input type="radio" value="2" name="cxfs" checked="checked"/>&nbsp;&nbsp;模糊查</span>
        </td>
     </tr>
</table>
</div>
<div id="list_area">

<!--/header end-->
<table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist1">
		<tr class="thead2" align="center">
               <td width="10%">账户名</td>
                <td width="10%">Uin</td>
                <td width="10%">账号创建时间</td>
                <td width="10%">冻结原因</td>
                <td width="10%">冻结结束时间</td>
                <td width="10%">冻结开始时间</td>
                <td width="10%">最后一次登录IP</td>
        		<td width="10%">最后一次登录时间</td>
				<td width="10%">禁言结束时间</td>
               <?php if($_SESSION['admin1'] == '游戏管理员'){echo "<!--" ;} ?>
                <td width="10%">操作</td>
               <?php if($_SESSION['admin1'] == '游戏管理员'){echo "-->" ;} ?>
		</tr>
 </table>
 <table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist">                           
</table>
<div class="mgr_divb"> 
</div>
<div class="page_area" style="margin-top:10px; text-align:center">
	<input type="button" value="上一页" id="up" onclick="pup()" />&nbsp;&nbsp;&nbsp;&nbsp;
    <input type="button" value="下一页" id="next" onclick="pnext()" />
    <input type="hidden" value="<?php echo $_SESSION['admin1']; ?>" id="tp"  />
</div>

<!--/list end-->
</div>
</body>
</html>