<?php	require_once(dirname(__FILE__).'/../inc/config.inc.php');
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
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>商城操作</title>
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
function jc()
{
	 var stp = $("#us").val();
	if(stp == '')
	{
		$("#us1").html("请输入角色名");
		//$("#tt1").html("请输入角色名");
		//$("#ttt").show();
		return;
	}
	if(stp != '')
	{
		$("#us1").html(""); 
		j=1;
	}
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
//搜索
function sear()
{
	 if(i ==1 && j == 1&& t==1){
	var qu=$("#dq").val();
	var fu=$("#fw").val();
	 //选择服务器
	var f = qu+'_'+fu;
	//时间
	var tb=$("#posttime").val();
	var tb1=$("#posttime1").val();
	var fag = $("#jz").val();
	//角色名
	var jsm=$("#us").val();
		
	$.ajax({
		type: 'POST',
		url: 'select_log.php',
		dataType: 'json',
		data: {"opBuyShop":"","xzdb": f,"time":tb, "time1":tb1, "us":jsm,"fag":fag,"qq":fu},
		success: function(msg){
			if( null ==msg || ""== msg)
			{
				$("#tabr").empty();
				$("#tt1").html("未检索到数据......");
				$("#ttt").show();
				exit();
			}
			if("不存在数据表"==msg)
			{
				$("#tabr").empty();
				$("#tt1").html("不存在数据表");
				$("#ttt").show();
				exit();
			}
			if("结束时间不能低于开始时间"==msg)
			{
				$("#tabr").empty();
				$("#tt1").html("结束时间不能低于开始时间!!!");
				$("#ttt").show();
				exit();
			}
			if("结束时间与开始时间相差不能超过3天"==msg)
			{
				$("#tabr").empty();
				$("#tt1").html("结束时间与开始时间相差不能超过3天");
				$("#ttt").show();
				exit();
			}
			else
			{
				$("#ttt").hide();
				$("#tabr").empty();
				//处理检索到的数据......
				$.each(msg, function (index, value) {
					//alert(value.RoleName);
					var s = '<tr class=mgr_tr align=center ><td height=32 width="15%" >'+value['LoginName']+'</td><td width="13%" >'+value['RoleName']+'</td><td width="8%">'+value['OpType']+'</td><td width="10%" >'+value['Shop']+'</td><td width="8%">'+value['ShopNum']+'</td><td width="8%" >'+value['MoneyNum']+'</td><td width="8%">'+value['GiftFlag']+'</td><td width="15%" >'+value['GiftToRoleID']+'</td><td width="15%" >'+value['LogTime']+'</td></tr>';
					$("#tabr").append(s);
				});
			}
		}
	});
	}
	else{
		alert(msg)
		$("#tabr").empty();
		$("#tt1").html("未检索到数据......");
		$("#ttt").show();
	}
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
<div class="mgr_header"> <span class="title">商城操作</span>
    <span class="reload"><a href="javascript:location.reload();">刷新</a></span>
</div>
<div class="mgr_divt" id="ttt">
<span id="tt1" style="color:#F00; font-size:16px; font-weight:bold; margin-left:180px;"></span>
</div>
<div class="mgr_divt">
<table width="100%" height="100%">
	<tr>
		<td>&nbsp;&nbsp;&nbsp;请选择·······大区：<select name="dq" id="dq" onchange="xf()">
        <option value="-请选择-" selected="selected">-请选择-</option>
        <?php
        $dosql->Execute("select distinct regid,regname from `reg_world` order by `regid` ");
        while($row=$dosql->GetArray()){?>
        				<option value="<?php echo $row['regid']; ?>"><?php echo $row['regname']; ?></option>
        <?php } ?> 
                     </select><span style="color:#F00" id="qu"></span>&nbsp;&nbsp;
    		&nbsp;&nbsp;&nbsp;服务器：<select name="fw" id="fw" >
        	 </select>&nbsp;&nbsp;&nbsp;
             <select id="jz">
            	<option value="1" selected="selected">角色</option>
                <option value="2" >帐号</option>
            </select>
            <input type="text" id="us" name="us"  onblur="jc()" />
            <span style="color:#F00" id="us1"></span>&nbsp;&nbsp;&nbsp;
        </td>
     </tr>
</table>
</div>
<div class="mgr_divt" id="sea">
<table width="100%" height="100%">
	<tr class="tcss">
		<td>&nbsp;&nbsp;&nbsp;
          开始时间：<input name="time" type="text" id="posttime" class="input_short" onblur="stime()" />
				<script type="text/javascript">
				date = new Date();
				Calendar.setup({
					inputField     :    "posttime",
					ifFormat       :    "%Y-%m-%d %H:%M:%S",
					showsTime      :    true,
					timeFormat     :    "24"
				});
				</script> <span style="color:#F00" id="sta"></span>
         结束时间：<input name="time1" type="text" id="posttime1" class="input_short" />
				<script type="text/javascript">
				date = new Date();
				Calendar.setup({
					inputField     :    "posttime1",
					ifFormat       :    "%Y-%m-%d %H:%M:%S",
					showsTime      :    true,
					timeFormat     :    "24"
				});
				</script>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<input type="button" id="s1" title="搜索" value="搜索" onclick="sear()" style="width:50px; height:22px;"/> 
                &nbsp;&nbsp;<span  name="ts" id="ts" style="color:#F00"></span>
        </td>
     </tr>
</table></div>
<table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="bd">
		<tr class="thead2" align="center">
				<td width="15%">账号</td>
				<td width="13%">角色名</td>
				<td width="8%">购买类型</td>
               	<td width="10%">商品</td>
                <td width="8%">数量</td>
                <td width="8%">总价</td>
                <td width="8%">是否赠送</td>
                <td width="15%">赠送人ID</td>
                <td width="15%">时间</td>
		</tr>
</table>
<table width="100%" border="0" cellpadding="0" id="tabr" cellspacing="0" class="mgr_table"></table>

 <div class="page_area">
</div>
</body>
</html>