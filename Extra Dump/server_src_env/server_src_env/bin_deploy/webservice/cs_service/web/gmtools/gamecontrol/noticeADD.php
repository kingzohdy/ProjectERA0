<?php require_once(dirname(__FILE__).'/../inc/config.inc.php');
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
<title>发布公告</title>
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
			var c = "<option value='all' selected='selected' >全部</option>";
			$("#classid").append(c);
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
			var c = "<option value='all' selected='selected' >全部</option>";
			$("#classid").append(c);
		}
});
function sever()
{
	var j=$("#classid").val();
	var f='';
	$("#severlist1").empty();
	if("all"== j)
	{
		$("#severlist").hide();
	}
	else
	{
		if(serverlist.WorldCount == 1)
		{
			f+="<input type='radio' name='fs' value="+serverlist.WorldInfos.WorldID+"/>&nbsp;"+serverlist.WorldInfos.WorldName+"&nbsp;";
		}
		else
		{
				for(var i = 0; i < serverlist.WorldInfos.length; i++)
				{
					 if(j==serverlist.WorldInfos[i].RegionID)
					 {
						f+="<input type='radio' name='fs' value="+serverlist.WorldInfos[i].WorldID+"/>&nbsp;"+serverlist.WorldInfos[i].WorldName+"&nbsp;";
					 }
				}
		}
		$("#severlist1").append(f);
		$("#severlist").show();
	}
}
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
function fabu()
{
	var j=$("#classid").val();
	//滚动时间，活跃时间，开始时间
	var gdtime=$("#second").val();
	var hytime=$("#second1").val();
	var kstime=$("#second2").val();
	var content=$("#description").val();
	//if(sl<3){
		//alert("参数提供不完整"+sl);
	//}
	//else
	//{
		if("all"== j)
		{
			$.ajax({
			  url : "conduct.php",
			  type:'post',
			  dataType:'json',
			  data: {"noticeAdd":"","all":"","gdtime": gdtime,"hytime":hytime,"kstime":kstime,"content":content},
			  beforeSend:function(){
				    $("#BgDiv").css({ display: "block", height: $(document).height() });
					$('#DialogDiv').show();
			  },
			  success:function(msg){
				  $('#DialogDiv').hide();
				  $("#BgDiv").hide();
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
		else
		{
			var fu= $(":radio.[name='fs']:checked").val();
			
			$.ajax({
			  url : "conduct.php",
			  type:'post',
			  dataType:'json',
			  data: {"noticeAdd":"","only":"","daqu":j,"fu":fu,"gdtime": gdtime,"hytime":hytime,"kstime":kstime,"content":content},
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
	//}
}
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
<div class="gray_header"> <span class="title">发布公告</span> <span class="reload"><a href="javascript:location.reload();">刷新</a></span> </div>
<form name="form" id="form" method="post" action="infolist_save.php" onsubmit="return cfm_infolm();">
	<table width="100%" border="0" cellspacing="0" cellpadding="0" class="form_table">
		<tr>
			<td width="25%" height="35" align="right">选择大区：</td>
			<td width="75%"><select name="classid" id="classid"  onchange="sever()">
				</select>
				<span class="maroon">*</span><span class="cnote">带<span class="maroon">*</span>号表示为必填项</span></td>
		</tr>
        <tr style="display:none" id="severlist">
			<td width="25%" height="35" align="right">服 务 器：&nbsp;</td>
			<td width="75%" id="severlist1">
                <!--<input type="radio" name="fs[]" value="all" />&nbsp;全部 -->
			</td>
		</tr>
        <tr>
			<td width="25%" height="35" align="right">滚动时间：</td>
			<td width="75%"><input id="second" type="text" style="width:60px;" onblur="checkint('second','check1')" /><span class="maroon">*</span>(秒,表示隔几秒滚动一次)&nbsp;&nbsp;<span id="check1" style="color:#F00"></span>
			</td>
		</tr>
        <tr>
			<td width="25%" height="35" align="right">活跃时间：</td>
			<td width="75%"><input id="second1" type="text" style="width:60px;" onblur="checkint('second1','check2')" /><span class="maroon">*</span>(秒,表示显示多长时间)&nbsp;&nbsp;<span id="check2" style="color:#F00"></span>
			</td>
		</tr>
        <tr>
			<td width="25%" height="35" align="right">开始时间：</td>
			<td width="75%"><input id="second2" type="text" style="width:60px;" onblur="checkint('second2','check3')" /><span class="maroon">*</span>(秒,表示多久开始发布,0表示马上发布)&nbsp;&nbsp;<span id="check3" style="color:#F00"></span>
			</td>
		</tr>
		<tr>
			<td height="100"align="right">内　&nbsp;&nbsp;  容：</td>
			<td><textarea name="description" id="description" class="class_areadesc" style="width:300px; height:100px;margin-top:10px; margin-bottom:10px;"></textarea></td>
		</tr>
	</table>
	<div class="subbtn_area" style="width:55%">
		<input type="button" class="blue_submit_btn" value="" onclick="fabu()" />
		<input type="button" class="blue_back_btn" value="" onclick="history.go(-1)"  />
	</div>
</form>
</body>
</html>