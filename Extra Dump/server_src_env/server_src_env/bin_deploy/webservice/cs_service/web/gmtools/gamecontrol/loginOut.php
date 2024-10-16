<?php 


require_once('../inc/config.inc.php');
@session_start();
$_SESSION = array();
session_destroy();
echo '<script type="text/javascript">window.top.location.href="login.php";</script>';


?>