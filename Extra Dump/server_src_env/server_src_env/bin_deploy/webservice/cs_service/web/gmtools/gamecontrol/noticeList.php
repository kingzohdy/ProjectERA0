<?php 
	require_once(dirname(__FILE__).'/../inc/config.inc.php'); 
//检测用户是否有权限
if($_SESSION['admin1'] == "游戏管理员")
{
	$_SESSION = array();
	session_destroy();	
	echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
	exit();
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>公告查询</title>
<link href="style/admin.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" >
var serverlist = -1;//用于保存服务器列表,若值为-1表示未获得列
$(document).ready(function(){
		$("#xdq").empty();
		if(-1 == serverlist)
		{
			$.ajax({
				url : "conduct.php",
				type:'post',
				dataType:'json',
				data: {"fwq": "z"},
				success:function(msg)
				{
					serverlist = msg;
					if(msg.ErrorCode == -6)
					{
						alert("找不到指定信息");
					}
					else if(msg.ErrorCode)
					{
						alert("ErrorCode: "+msg.ErrorCode+"\nErrorText: "+msg.ErrorText);
					}
					else
					{
						if(msg.AccountCount==1)
						{
							$("#xdq")[0].options.add(new Option(msg.AccountInfos.RegionName, msg.AccountInfos.RegionID, false, false));
						}
						else
						{
							for(var i = 0; i < msg.AccountInfos.length; i++)
							{
								$("#xdq")[0].options.add(new Option(msg.AccountInfos[i].RegionName, msg.AccountInfos[i].RegionID, false, false));
							}
						}
					}
				}
			});
			var c = "<option value='-请选择-' selected='selected' >-请选择-</option>";
			$("#xdq").append(c);
		}
		else
		{
			if(serverlist.AccountCount==1)
			{
				$("#xdq")[0].options.add(new Option(msg.AccountInfos.RegionName, msg.AccountInfos.RegionID, false, false));
			}
			else
			{
				for(var i = 0; i < serverlist.AccountInfos.length; i++)
				{
					$("#xdq")[0].options.add(new Option(serverlist.AccountInfos[i].RegionName, serverlist.AccountInfos[i].RegionID, false, false));
				}
			}
			var c = "<option value='-请选择-' selected='selected' >-请选择-</option>";
			$("#xdq").append(c);
		}
});
function selectdq()
{
	var j=$("#xdq").val();
	var f;
	$("#dqlist").empty();
	if("-请选择-"== j)
	{
		$("#dqlist").hide();
	}
	else
	{
		if(serverlist.WorldCount == 1)
		{
			$("#dqlist")[0].options.add(new Option(serverlist.WorldInfos.WorldName,serverlist.WorldInfos.WorldID,false, false));
		}
		else
		{
			for(var i = 0; i < serverlist.WorldInfos.length; i++)
			{
				 if(serverlist.WorldInfos.length > 1 && j==serverlist.WorldInfos[i].RegionID)
				 {
					 $("#dqlist")[0].options.add(new Option(serverlist.WorldInfos[i].WorldName,serverlist.WorldInfos[i].WorldID,false, false));
				 } 	
			}
		}
		$("#dqlist").show();
	}
}
function selectReg()
{
	var d = "";
	var q=$("#xdq").val();
	var f=$("#dqlist").val();
	if(!f)
	{
		alert("请选择区·服");
	}
	else
	{
		$.ajax(
		{
			url : "conduct.php",
			type:'post',
			dataType:'json',
			data: {"noticelist": "","q":q,"f":f},
			success:function(msg)
			{
				$("#ajaxlist2").empty();
				if(msg.ErrorText )
				{
					alert("ErrorCode: "+msg.ErrorCode+"\nErrorText: "+msg.ErrorText);
				}
				else{
					if(msg.Num <= 0){
						alert("暂无最新公告");
					}
					if(msg.Num == 1)
					{
						if(msg.BulletinArray.Core.Type ==0){ msg.BulletinArray.Core.Type="正常公告";}
						if(msg.BulletinArray.Core.Type ==1){ msg.BulletinArray.Core.Type="强制消息弹框";}
						if(msg.BulletinArray.Core.Type ==2){ msg.BulletinArray.Core.Type="强制图标";}
						d ='<tr align=center><td  height=32 width=7%>'+msg.BulletinArray.Core.ID+'</td><td width=40% >'+msg.BulletinArray.Core.Text+'</td><td width=6% >'+msg.BulletinArray.Core.RollInterval+'(秒)</td><td width=6% >'+msg.BulletinArray.ActiveTime+'(秒)</td></td><td width=7% >'+msg.BulletinArray.Core.Type+'</td><td width=5% >'+msg.BulletinArray.Core.TStart+'</td><td width=25% >[<a href=# onclick="delNotice('+msg.BulletinArray.Core.ID+')" >删除(仅此服务器)</a>][<a href=#  onclick="delNoticeAll('+msg.BulletinArray.Core.ID+')">删除(所有服务器)</a>]</td></tr>';
					}
					else
					{
						for(var i = 0; i < msg.BulletinArray.length; i++)
						{
							if(msg.BulletinArray[i].Core.Type ==0){ msg.BulletinArray[i].Core.Type="正常公告";}
							if(msg.BulletinArray[i].Core.Type ==1){ msg.BulletinArray[i].Core.Type="强制消息弹框";}
							if(msg.BulletinArray[i].Core.Type ==2){ msg.BulletinArray[i].Core.Type="强制图标";}
							d +='<tr align=center class=mgr_tr onmouseover=this.className=mgr_tr_on onmouseout=this.className=mgr_tr><td  height=32 width=7%>'+msg.BulletinArray[i].Core.ID+'</td><td width=40% >'+msg.BulletinArray[i].Core.Text+'</td><td width=6% >'+msg.BulletinArray[i].Core.RollInterval+'(秒)</td><td width=6% >'+msg.BulletinArray[i].ActiveTime+'(秒)</td></td><td width=7% >'+msg.BulletinArray[i].Core.Type+'</td><td width=5% >'+msg.BulletinArray[i].Core.TStart+'</td><td width=25% >[<a href=# onclick="delNotice('+msg.BulletinArray[i].Core.ID+')" >删除(仅此服务器)</a>][<a href=# onclick="delNoticeAll('+msg.BulletinArray[i].Core.ID+')" >删除(所有服务器)</a>]</td></tr>';
						}
						
					}
					$("#ajaxlist2").append(d);
					$("#ajaxlist2").show();

				}
			}
		});
	}
}
function delNotice(id)
{
	var Nid=id;
	var q=$("#xdq").val();
	var f=$("#dqlist").val();
	$.ajax(
		{
			url : "conduct.php",
			type:'post',
			dataType:'json',
			data: {"delnotice": Nid,"qu":q,"fu":f},
			 beforeSend:function(){
				    $("#BgDiv").css({ display: "block", height: $(document).height() });
					$('#DialogDiv').show();
			 },
			success:function(msg)
			{
				if(msg == 0)
				{
					alert("成功");
					window.location.reload(); 
				}
				else
				{
					alert("ErrorCode: "+msg.ErrorCode+"\nErrorText: "+msg.ErrorText);
					window.location.reload(); 
				}
			}		
		});
		
}
function delNoticeAll(id)
{
	var Nid=id;
	$.ajax(
		{
			url : "conduct.php",
			type:'post',
			dataType:'json',
			data: {"delnoticeall": Nid},
			 beforeSend:function(){
				    $("#BgDiv").css({ display: "block", height: $(document).height() });
					$('#DialogDiv').show();
			 },
			success:function(msg)
			{
				if(msg == 0)
				{
					alert("成功");
					window.location.reload(); 
				}
				else
				{
					alert("ErrorCode: "+msg.ErrorCode+"\nErrorText: "+msg.ErrorText);
					window.location.reload(); 
				}
			}		
		});
		
}
</script>
 <style type="text/css">  
#BgDiv{background-color:#F7F7F7; position:absolute; z-index:99; left:0; top:0; display:none; width:100%; height:100%;opacity:0.5;filter: alpha(opacity=50);-moz-opacity: 0.5;} 
#DialogDiv{position:absolute;width:300px; left:55%; top:20%; margin-left:-200px; height:50px; z-index:100; border:0px #ccc solid; padding:1px;}
</style>
</head>
<body >
 <div id="BgDiv"></div>
<div id="DialogDiv" style="display:none">
	<h2 style="color:#F00">删除中,请稍等...</h2>
</div>
<div class="mgr_header"> <span class="title">最新公告查询</span>
    <span class="reload"><a href="javascript:location.reload();">刷新</a></span>
</div>
<!--/header end-->
<div class="mgr_divt">
<table width="100%" height="100%">
	<tr>
		<td >&nbsp;&nbsp;&nbsp;&nbsp;
        	<select name="xdq" id="xdq" style="vertical-align:top; margin-top:6px;"  onchange="selectdq()" >
        	 </select>
             <select name="dqlist" id="dqlist" style="vertical-align:top; display:none; margin-top:6px; margin-left:0px;" >
        	 </select>
             <input type="button" style="vertical-align:top; margin-top:6px;" value="查看" onclick="selectReg()" />
         </td>
     </tr>
</table>
</div>
<div id="mgr_header">
<form name="form" id="form" method="post">
    	<table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist1">
		<tr class="thead2" align="center">
                <td width="7%">公告ID</td>
                <td width="40%">内容</td>
                <td width="6%">滚动时间</td>
                <td width="6%">剩余时间</td>
                <td width="7%">公告类型</td>
                <td width="5%">发布时间</td>
                <td width="25%">操作</td>
		</tr>
 		</table>
        <table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist2">
 		</table>
</form>
</div>
<!--/list end-->
</body>
</html>