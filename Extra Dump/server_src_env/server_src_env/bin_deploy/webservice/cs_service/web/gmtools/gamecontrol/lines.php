<?php	require_once(dirname(__FILE__).'/../inc/config.inc.php');
//检测用户是否有权限
if($_SESSION['admin1'] != "超级游戏管理员")
{
	$_SESSION = array();
	session_destroy();	
	echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
	exit();
}
if(isset($_POST['lines']))
{
	$rwid = $_POST['lines'];
	$sql = "SELECT `http` FROM `line` where `r_wid` = '$rwid' ";
	$dosql->Execute($sql);
	$data = $dosql->GetArray();
	echo $data['http'];
	exit;
}
 ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>在线人数查看</title>
<link href="style/admin.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="../data/plugin/calendar/calendar1.js"></script>
<script type="text/javascript">
	var i = 0;
	var j = 0;
	var t = 0;
	var $tri=0;
$(document).ready(function(){
  $("#ttt").hide();
});
function xf()
{
	//$GLOBALS['db_name'];
	var keyword = $("#dq").val();
	if(keyword == '' || keyword == '-请选择-')
	{
		$("#qu").html("请选择大区");
		$("#fw").empty();
		return;
	}
	if(keyword != '' && keyword != '-请选择-')
	{
		$("#qu").html("");
		i=1;
	}

	$.ajax({
		url : "select_log.php",
		type:'post',
		dataType:'json',
		data:{"daqu":keyword},
		success:function(msg){
			 $(document).ready(function () {
 				 $("#fw").empty();
 				 $.each(msg, function (index, value) {
 				 	$("#fw")[0].options.add(new Option(value.worldname, value.worldid, false, false));
  				});
  			});
		}
	});
}
function stime()
{
	 var st = $("#posttime").val();
	if(st == '')
	{
		$("#sta").html("请选择开始时间");
		return;
	}
	if(st != '')
	{
		$("#sta").html(""); 
		t=1;
	}
}
function sear()
{
	if(i !=1 && t !=1){
		alert("信息提交不完整");
		return;
	}
	var qu=$("#dq").val();
	var fu=$("#fw").val();
	 
	var f = qu+'_'+fu;
		
	$.ajax({
		type: 'POST',
		url: 'lines.php',
		dataType: 'html',
		data: {"lines": f},
		success: function(msg){	
		
			$("#form1").attr("action",msg);   
			$("#form1").attr("method","POST");   
			$("#form1").submit();     			   
		}
	});
}
</script>
<style type="text/css">
body
{
	margin:0 auto;
	padding:0 auto;
	background:#FFF;
}
.tcss
{
	border:0px;
	margin-left:70px;
}
</style>
</head>

<body>
<div class="mgr_header"> <span class="title">在线人数查看</span>
</div>
<div class="mgr_divt" id="ttt">
<span id="tt1" style="color:#F00; font-size:16px; font-weight:bold; margin-left:180px;"></span>
</div>
<div class="mgr_divt" style="height:60px; padding-top:26px;">
<form id="form1">
<table width="100%" height="100%">
	<tr>
		<td>&nbsp;&nbsp;&nbsp;请选择·······大区：<select name="REGIONID" id="dq" onchange="xf()">
        <option value="-请选择-" selected="selected">-请选择-</option>
        <?php
        $dosql->Execute("select distinct regid,regname from `reg_world` order by `regid` ");
        while($row=$dosql->GetArray()){?>
        				<option value="<?php echo $row['regid']; ?>"><?php echo $row['regname']; ?></option>
        <?php } ?> 
                     </select><span style="color:#F00" id="qu"></span>&nbsp;&nbsp;
    		&nbsp;&nbsp;&nbsp;服务器：<select name="WORLDID" id="fw" >
        	 </select>&nbsp;&nbsp;&nbsp;
          开始时间：<input name="DAY" type="text" id="posttime" class="input_short" onblur="stime()" />
				<script type="text/javascript">
				date = new Date();
				Calendar.setup({
					inputField     :    "posttime",
					ifFormat       :    "%Y-%m-%d",
					showsTime      :    true,
					timeFormat     :    "24"
				});
				</script> <span style="color:#F00" id="sta"></span>
         结束时间：<input name="ENDDAY" type="text" id="posttime1" class="input_short" />
				<script type="text/javascript">
				date = new Date();
				Calendar.setup({
					inputField     :    "posttime1",
					ifFormat       :    "%Y-%m-%d",
					showsTime      :    true,
					timeFormat     :    "24"
				});
				</script>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<input type="button" id="s1" title="生成" value="生成" onclick="sear()" style="width:50px; height:22px;"/> 
                &nbsp;&nbsp;<span  name="ts" id="ts" style="color:#F00"></span>
        </td>
     </tr>
</table>
</form>
</div>
</body>
</html>