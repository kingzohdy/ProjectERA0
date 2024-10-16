<?php 
	require_once(dirname(__FILE__).'/../inc/config.inc.php'); 

if($_SESSION['id'] != "11")
{
$_SESSION = array();
session_destroy();	
echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
exit();
}

$tbname="gmlog";
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>baobao</title>
<link href="style/admin.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="js/listajax.js"></script>
<script type="text/javascript" >
function ShowDIV(thisObjID)
{
	  $("#BgDiv").css({ display: "block", height: $(document).height() });
	  var yscroll = document.documentElement.scrollTop;
	  $("#" + thisObjID ).css("top", "100px");
	  $("#" + thisObjID ).css("display", "block");
	document.documentElement.scrollTop = 0;
}
function closeDIV(thisObjID)
{
	  $("#BgDiv").hide();
	  $("#" + thisObjID ).hide();;
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
			$("#"+id1).html("");
			sl+=1;
		}
		else
		{
			$("#"+id1).html("提供的参数不是一个有效的正整数");
			sl-=1;
		}
	}	
}
</script>
<style type="text/css">  
#BgDiv{background-color:#F7F7F7; position:absolute; z-index:99; left:0; top:0; display:none; width:100%; height:100%;opacity:0.5;filter: alpha(opacity=50);-moz-opacity: 0.5;} 
.DialogDiv{position:absolute;width:380px; left:55%; top:55%; margin-left:-200px; height:auto; z-index:100; border:0px #ccc solid; padding:1px;}
.dotline {BORDER-BOTTOM-STYLE: dotted; BORDER-LEFT-STYLE: dotted;BORDER-RIGHT-STYLE: dotted; BORDER-TOP-STYLE: dotted; size:1; color:green; width:90%;}

</style>
</head>
<body >

<!--/header end-->
<form name="form"  method="post" action="baobao.php">
<div class="mgr_divt" style="height:50px;">
<table width="100%" height="100%">
	<tr>
		<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;请选择&middot;&middot;&middot;&middot;&middot;&middot;&middot;大区：
        <select name="dq" id="dq" onchange="xf()">
        <?php
			$dosql->Execute("select distinct regid,regname from `reg_world` order by `regid` DESC ");
			while($row=$dosql->GetArray())
			{				
				echo "<option value=".$row['regid'].">".$row['regname']."</option>";
			}
         ?> 
         </select>
         <span style="color:#F00" id="qu"></span>&nbsp;&nbsp;       
        输入物品帐号：<input type="text" id="account" name="account"  />&nbsp;&nbsp;
        输入物品ID：<input type="text" id="itemid" name="itemid" value="5052901"  onblur="checkint('itemid','ts')" />&nbsp;&nbsp;
        输入物品名称：<input type="text" name="itemname" value="金券袋"  />
        输入数量：<input type="text" id="nums" name="nums" onblur="checkint('nums','ts')" style="width:60px;" />&nbsp;&nbsp;
        输入物品描述：<input type="text" name="miaoshu" value="充值返金券活动"  />
        <input type="submit" title="执行" value="执行" style="width:50px; height:22px;"/> 
        <span  name="ts" id="ts" style="color:#F00"></span>
        </td>
     </tr>
</table>
</div>
</form>
<div id="BgDiv"></div>
<div id="mgr_header">
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist1">
    <tr class="thead2" align="center">
            <td>操作者</td>
            <td>操作类型</td>
            <td>被操作帐号</td>
            <td>被操作Uin</td>
			<td>物品ID</td>
			<td>数量</td>
            <td>操作时间</td>
            <td>操作大区</td>
            <td>执行情况</td>
            <td>错误信息</td>
     </tr>
    <?php	
        $sql = "select * from `baobaolog` ";
        $dopage->GetPage($sql,13,'LogId DESC');
        while($row = $dosql->GetArray()){				
     ?>
         <tr align="center" height="32">
             <td width="8%">
                <?php  
                    $sql2 = "select * from `adminor` where `id` = '$row[UserId]' ";
                    $rr = mysql_query($sql2);
                    $user = mysql_fetch_array($rr); echo $user['username']
                 ?>
             </td>
             <td ><?php echo $row['OperateType'] ?></td>
             <td ><?php echo $row['Account'] ?></td>
             <td ><?php echo $row['Uin'] ?></td>
			 <td ><?php echo $row['itemID'] ?></td>
			 <td ><?php echo $row['nums'] ?></td>
             <td ><?php echo $row['time'] ?></td>
             <td ><?php echo $row['RegionName'] ?></td>
             <td ><?php if(1 == $row['OperateState']){ echo "成功";} else if(0 == $row['OperateState']){echo "失败";} ?></td>
             <div id="<?php echo $row['LogId'] ?>" class="DialogDiv" style="display:none; background:#FFF; margin-top:20px; border:1px solid #ccc">
                <div style="height:10px"></div>
                <div align="center" width="80%" height="80%">
                <pre style="font-size:14px;"><?php echo $row['log']; ?></pre>
                <input style="text-align:center; margin-top:8px; margin-bottom:8px;" type="button" value="确认" onclick="closeDIV(<?php echo $row['LogId'] ?>)" />
                </div>
            </div>
            <td ><a href="#" onclick="ShowDIV(<?php echo $row['LogId'] ?>)">详细</a></td> 
         </tr>
     <?php } ?>
    </table>
<div class="page_area">
	<?php echo $dopage->AjaxPage(); ?>
</div>
</div>
<!--/list end-->
</body>
</html>