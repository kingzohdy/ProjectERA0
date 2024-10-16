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
<link href="style/admin.css" rel="stylesheet" type="text/css" />
<style type="text/css">
 a{ color:#00F;}
 </style>
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/jscript">
function ShowDIV(thisObjID) {
	var x = event.clientX;
	var y = event.clientY;
    var yscroll = document.documentElement.scrollTop;
	//$("#" + thisObjID).show().css({"left":x,"top":y});
	$("#" + thisObjID).show();
    //$("#" + thisObjID ).css("top", "100px");
    //$("#" + thisObjID ).css("display", "block");
    document.documentElement.scrollTop = 0;
 }
 function ColseDIV(thisObjID){
    $("#" + thisObjID ).hide();	
}
</script>
<body>
<table width="100%" border="1" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist" >
		<tr align="center" class="mgr_tr">
            <td  height="15"  align="center" colspan="4" >公会名称:<a href="#" >卖萌为主</a></td>
         	<td  height="15"  align="center" colspan="4" >公会等级:<a href="#" >1</a></td>
		</tr>
        <tr align="center" class="mgr_tr">
        	<td  height="15"  align="center" colspan="4" >官阶:<a href="#" >会长</a></td>
         	<td  height="15"  align="center" colspan="4" >贡献度:<a href="#" >1563/30000</a></td>
        </tr>
        <tr align="center" class="mgr_tr">
        	<td  height="15"  align="center" colspan="4" >资金:<a href="#" >300000</a></td>
         	<td  height="15"  align="center" colspan="4" >繁荣度:<a href="#" >10000</a></td>
        </tr>
        <tr align="center" class="mgr_tr">
        	<td  height="15"  align="center" colspan="4" >安定度:<a href="#" >10000/10000</a></td>
         	<td  height="15"  align="center" colspan="4" >科技点:<a href="#" >523</a></td>
        <tr align="center" class="mgr_tr">
        	<td  height="15"  align="center" colspan="4" >在线人数:<a href="#" >23/50</a></td>
         	<td  height="15"  align="center" colspan="4" >城市等级:<a href="#" >1</a></td>
        </tr>
	</table>
</body>
</html>
