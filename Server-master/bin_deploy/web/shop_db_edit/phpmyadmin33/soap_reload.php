<?php
/* vim: set expandtab sw=4 ts=4 sts=4: */
/**
 *
 * @version $Id$
 * @package phpMyAdmin
 */

/**
 *
 */
require_once './libraries/common.inc.php';
require_once './soap_comm.php';
/**
 * Runs common work
 */
require './libraries/db_common.inc.php';
require_once './libraries/sql_query_form.lib.php';

// After a syntax error, we return to this script
// with the typed query in the textarea.
$goto = 'soap_reload.php';
$back = 'soap_reload.php';

/**
 * Gets informations about the database and, if it is empty, move to the
 * "db_structure.php" script where table can be created
 */
require './libraries/db_info.inc.php';
if ($num_tables == 0 && empty($db_query_force)) {
    $sub_part   = '';
    $is_info    = TRUE;
    require './db_structure.php';
    exit();
}

/**
 * Query box, bookmark, insert data from textfile
 */
if ($_POST == array() ) {
	PMA_soapReloadForm(true, false, isset($_REQUEST['delimiter']) ? latin_htmlspecialchars($_REQUEST['delimiter']) : ';');
}
else
{
	global $client;
//	$client = new SoapClient("./cfg/cs_service.wsdl", 
//								array( 
//								// Stuff for development. 
//								'trace' => 1, 
//								'exceptions' => true, 
//								'cache_wsdl' => WSDL_CACHE_NONE, 
//								'features' => SOAP_SINGLE_ELEMENT_ARRAYS, 
//								'encoding' =>'iso-8859-1',
//								'login' => 'soapuser',
//								'password' => 'soapuser#1234'
//								)
//							);
	$RegionID = 0;
	$WorldID = 0;
	$TimeOut = 30000;
	global $RegionID;
	global $WorldID;

	if (isset($_POST['region_id']))
	{
		$RegionID = $_POST['region_id'];
	}

	if (isset($_POST['world_id']))
	{
		$WorldID = $_POST['world_id'];
	}

//	var_dump($_SESSION);

//	var_dump($RegionID);
//	var_dump($WorldID);
//	var_dump($_POST);
//	exit;

	$xml=$client->ReloadServer(array('TimeOut'=>$TimeOut,'RegionID'=>$RegionID,'WorldID'=>$WorldID));
	$ret = $xml->ErrorCode;
	if ($ret == 0)
	{
		$ret = 1;
	}
	else
	{
		$GLOBALS['soap_err_str'] = $xml->StrError->ErrorText;
//		var_dump($GLOBALS['soap_err_str']);
//		var_dump($xml);
	}
	PMA_soapReloadForm(true, false, isset($_REQUEST['delimiter']) ? latin_htmlspecialchars($_REQUEST['delimiter']) : ';',$ret);
}

/**
 * Displays the footer
 */
require_once './libraries/footer.inc.php';
?>
