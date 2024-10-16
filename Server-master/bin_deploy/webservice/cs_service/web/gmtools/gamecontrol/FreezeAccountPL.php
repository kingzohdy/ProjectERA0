<?php require_once(dirname(__FILE__).'/../inc/config.inc.php');
//检测用户是否有权限
if($_SESSION['admin1'] == "游戏管理员")
{
	$_SESSION = array();
	session_destroy();	
	echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
	exit();
}
if(isset($_POST['classid']))
{
	$classid = $_POST['classid'];
	$second = $_POST['second'];
	$account = $_POST['account'];
	if("" == $account || "" == $second || "" == $classid)
	{
		echo "数据输入错误";
		exit;
	}
	else
	{
		$s = explode("|",$account);
		print_r($s);
		exit;
	}
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>批量封停账号</title>
<link href="style/admin.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="js/serverlist.js"></script>
<script type="text/jscript" >
var sl = 0;
var serverlist = -1;//用于保存服务器列表,若值为-1表示未获得列表
$(document).ready(function(){
		$("#classid").empty();
		if(-1 == serverlist)
		{
			$.ajax(
			{
				url : "conduct.php",
				type:'post',
				//url: 'select_log.php',
				dataType:'json',
				data: {"fwq": "z"},
				success:function(msg)
				{
					serverlist= msg;
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
							$("#classid")[0].options.add(new Option(msg.AccountInfos.RegionName, msg.AccountInfos.RegionID, false, false));
						}
						else
						{
							for(var i = 0; i < msg.AccountInfos.length; i++)
							{
							$("#classid")[0].options.add(new Option(msg.AccountInfos[i].RegionName, msg.AccountInfos[i].RegionID, false, false));
							}
						}
					}
				}
			});
		}
		else
		{
			if(serverlist.AccountCount==1)
			{
				$("#classid")[0].options.add(new Option(msg.AccountInfos.RegionName, msg.AccountInfos.RegionID, false, false));
			}
			else
			{
				for(var i = 0; i < serverlist.AccountInfos.length; i++)
				{
					$("#classid")[0].options.add(new Option(serverlist.AccountInfos[i].RegionName, serverlist.AccountInfos[i].RegionID, false, false));
				}
			}
		}
});

function checkint(id,id1)
{
	var i=$("#"+id).val();
	var re= new RegExp("^[0-9]{1,}$");
	if(isNaN(i))
	{
		$("#"+id1).html("请输入正整数");
	}
	else
	{
		if(re.test(i))
		{
			$("#"+id1).html("输入正确");
			sl+=1;
		}
		else
		{
			$("#"+id1).html("提供的参数不是一个有效的正整数");
			sl-=1;
		}
	}	
}
function djs()
{
	var classid = $('#classid').val();
	var second = $('#second').val();
	var account = $('#account').val();
	var liyou = $('#liyou').val();
	$.ajax
	({
		type: 'POST',
		url: 'conduct.php',
		dataType: 'json',
		data: {"dongjies":"","jintime": second,"daqu":classid,"account":account,"cause":liyou}, 
		beforeSend:function(){
			$("#BgDiv").css({ display: "block", height: $(document).height() });
			$('#DialogDiv').show();
		 },
		success: function(array)
		{
			$('#DialogDiv').hide();
			$("#BgDiv").hide();
			alert(array);
		}
	});
	
}
/*
function feng()
{
	var j=$("#classid").val();	
	var gdtime=$("#second").val();//时间
	var liyou=$("#liyou").val();
	var kstime=$("#account").val();		
	$.ajax({
	  url : "conduct.php",
	  type:'post',
	  dataType:'json',
	  data: {"noticeAdd":"","only":"","daqu":j,"gdtime": gdtime,"hytime":hytime,"kstime":kstime,"content":content},
	  beforeSend:function(){
			$("#BgDiv").css({ display: "block", height: $(document).height() });
			$('#DialogDiv').show();
	  },
	  success:function(msg){
		  $('#DialogDiv').hide();
		  $("#BgDiv").hide()
		  if(msg ==0)
		  {
			alert("成功");
			//window.location.reload();   
		  }
		  else
		  {
			  alert("ErrorCode: "+msg.ErrorCode+"\nErrorText: "+msg.ErrorText);
			  //window.location.reload(); 
		  }
	  }
	});
}
*/
</script>
<style type="text/css">  
#BgDiv{background-color:#F7F7F7; position:absolute; z-index:99; left:0; top:0; display:none; width:100%; height:100%;opacity:0.5;filter: alpha(opacity=50);-moz-opacity: 0.5;} 
#DialogDiv{position:absolute;width:300px; left:55%; top:20%; margin-left:-200px; height:50px; z-index:100; border:0px #ccc solid; padding:1px;}
</style>
</head>
<body> 
<div id="BgDiv"></div>
<div id="DialogDiv" style="display:none">
	<h2 style="color:#F00">发送中,请稍等...</h2>
</div>
<div class="gray_header"> <span class="title">批量封账号</span> <span class="reload"><a href="javascript:location.reload();">刷新</a></span> </div>
<form name="form" id="form" onsubmit="return cfm_infolm();" >
	<table width="100%" border="0" cellspacing="0" cellpadding="0" class="form_table">
		<tr>
			<td width="25%" height="35" align="right">选择大区：</td>
			<td width="75%"><select name="classid" id="classid" >
				</select>
            </td>
		</tr>
        <tr>
			<td height="100"align="right">封号理由：</td>
			<td><textarea name="liyou" id="liyou" class="class_areadesc" style="width:300px; height:50px;margin-top:10px; margin-bottom:10px;"></textarea></td>
		</tr>
        <tr>
			<td width="25%" height="35" align="right">封停时间：</td>
			<td width="75%"><input id="second" name="second" type="text" onblur="checkint('second','check1')" />（秒，正整数）&nbsp;&nbsp;<span id="check1" style="color:#F00"></span>
			</td>
		</tr>
		<tr>
			<td height="100"align="right">封停账号：</td>
			<td><textarea name="account" id="account" class="class_areadesc" style="width:300px; height:100px;margin-top:10px; margin-bottom:10px;"></textarea>（每个账号请用“|”分割，不要加空格、换行或其他字符）</td>
		</tr>
	</table>
	<div class="subbtn_area" style="width:55%">
		<input type="button" class="blue_submit_btn" value="" onclick="djs()" />
		<input type="button" class="blue_back_btn" value="" onclick="history.go(-1)"  />
	</div>
</form>
</body>
</html>