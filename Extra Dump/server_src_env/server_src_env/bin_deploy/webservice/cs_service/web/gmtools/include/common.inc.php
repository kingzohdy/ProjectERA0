<?php	
@header("Content-Type:text/html;charset=utf-8");

/*
**************************
(C)2010-2012 phpMyWind.com
update: 2012-8-28 13:47:05
person: Feng
**************************
*/

define('PHPMYWIND_INC', preg_replace("/[\/\\\\]{1,}/", '/', dirname(__FILE__)));
define('PHPMYWIND_ROOT', preg_replace("/[\/\\\\]{1,}/", '/', substr(PHPMYWIND_INC, 0, -8)));
define('PHPMYWIND_DATA', PHPMYWIND_ROOT.'/data');
define('PHPMYWIND_UPLOAD', PHPMYWIND_ROOT.'/../upload');
define('PHPMYWIND_BACKUP', PHPMYWIND_DATA.'/backup');
define('IN_PHPMYWIND', TRUE);


require_once(PHPMYWIND_INC.'/common.func.php');  //全局常用函数
require_once('../cfg/conn.inc.php');     //引入数据库连接信息
require_once(PHPMYWIND_INC.'/mysql.class.php');


//检查外部传递的值并转义
function _RunMagicQuotes(&$svar)
{
    if(!get_magic_quotes_gpc())
    {
        if(is_array($svar))
        {
            foreach($svar as $_k => $_v) $svar[$_k] = _RunMagicQuotes($_v);
        }
        else
        {
            if(strlen($svar)>0 && preg_match('#^(cfg_|GLOBALS|_GET|_POST|_COOKIE)#',$svar) )
            {
              exit('不允许请求的变量!');
            }

            $svar = addslashes($svar);
        }
    }
    return $svar;
}

//直接应用变量名称替代
foreach(array('_GET','_POST','_COOKIE') as $_request)
{
	foreach($$_request as $_k => $_v) ${$_k} = _RunMagicQuotes($_v);
}


//Session保存路径
$sess_savepath = PHPMYWIND_DATA.'/sessions/';

if(is_writable($sess_savepath) &&
   is_readable($sess_savepath))
   session_save_path($sess_savepath);

//文件上传全局变量
$cfg_upload_img_type = 'gif|png|jpg|bmp';
$cfg_upload_soft_type = 'zip|gz|rar|iso|doc|xls|ppt|wps|txt';
$cfg_upload_media_type = 'swf|flv|mpg|mp3|rm|rmvb|wmv|wma|wav';
$cfg_max_file_size = '2097152';
//上传文件保存路径
$cfg_image_dir = PHPMYWIND_UPLOAD.'/image';
$cfg_soft_dir  = PHPMYWIND_UPLOAD.'/soft';
$cfg_media_dir = PHPMYWIND_UPLOAD.'/media';

?>