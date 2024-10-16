<?php
session_start();
if(isset($_POST['hex']))
{
	$_SESSION['hex'] = $_POST['hex'];
	return 1;
}
if(isset($_GET['file']))
{
	header("Content-type:application/vnd.ms-xml");
	header("Content-Disposition:attachment;filename=rolefile.xml");
	$hex = $_SESSION['hex'];
	$_SESSION['hex'] = "";
	require_once('soap.php');
	$result = ParseBlob($hex);
	/*if($result->ErrorCode==0)
	{
		$str = $result->StrXML;
		$data=simplexml_load_string($str);
		
	}
	if($result->ErrorCode!=0)
	{
		$err = 1;
		$arr['ErrorCode'] = $result->ErrorCode;
		$arr['ErrorText'] = $result->StrError->ErrorText;
		$data  = $arr;
	}*/
//	$str = $result->StrXML;
//	$data=simplexml_load_string($str);
	print_r($result);
	exit;
}
?>