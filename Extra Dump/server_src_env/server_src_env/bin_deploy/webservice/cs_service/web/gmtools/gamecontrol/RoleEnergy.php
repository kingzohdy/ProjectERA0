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
            <td  height="15"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" onMouseDown="ColseDIV('DialogDiv')"  >能源1</a></td>
         	<td rowspan="8" width="60%" style="background:#FFF;">
            	<table id="DialogDiv">
                	<tr align="left">
           				 <td  height="15"  align="left"  >名称：<a href="#"  >8阶绿晶石</a></td>
					</tr>
        			<tr align="left" >
            			<td  height="15"  align="left" >等级：<a href="#"  >6级</a></td>
					</tr>
       	 			<tr align="left" >
          				 <td  height="15"  align="left"  >经验：<a href="#"  >12536/52360</a></td>
					</tr>
                    <tr align="left" >
          				 <td  height="15"  align="left"  >属性：<a href="#"  >魔法攻击+421</a></td>
					</tr>
                </table>
            </td>
		</tr>
        <tr align="center" class="mgr_tr">
        <td  height="15"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" >能源2</a></td>
        </tr>
        <tr align="center" class="mgr_tr">
            <td  height="15"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');">能源3</a></td>
		</tr>
        <tr align="center" class="mgr_tr">
        <td  height="15"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" >能源4</a></td>
        </tr>
        <tr align="center" class="mgr_tr">
           <td  height="15"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" >能源5</a></td>
		</tr>
        <tr align="center" class="mgr_tr">
            <td  height="15"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" >能源6</a></td>
		</tr>
        <tr align="center" class="mgr_tr">
           <td  height="15"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" >能源7</a></td>
		</tr>
        <tr align="center" class="mgr_tr">
           <td  height="15"  align="center" colspan="4" >&nbsp;&nbsp;<a href="#" onClick="ShowDIV('DialogDiv');" >能源8</a></td>
		</tr>
	</table>
</body>
</html>
