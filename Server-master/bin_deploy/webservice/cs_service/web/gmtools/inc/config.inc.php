<?php
define('ADMIN_INC',  preg_replace("/[\/\\\\]{1,}/", '/', dirname(__FILE__)));
define('ADMIN_ROOT', preg_replace("/[\/\\\\]{1,}/", '/', substr(ADMIN_INC, 0, -3)));
define('ADMIN_TEMP', ADMIN_ROOT.'/templates');
require_once(ADMIN_ROOT.'/include/common.inc.php');
require_once(ADMIN_INC.'/admin.func.php');
require_once(ADMIN_INC.'/page.class.php');
@session_start();
//检测是否登录
if(!isset($_SESSION['admin1'])){
	$_SESSION = array();
	session_destroy();
	echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
	exit();
}
?>