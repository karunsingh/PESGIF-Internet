<?
	//Create an associative array - the only key, "ipad", has the value of the IP Address
	$arr = array("ipad"=>$_SERVER['REMOTE_ADDR']);
	echo json_encode($arr); //The array is encoded as a JSON Object and echo'd/printed
?>