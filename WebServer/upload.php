<?
	//Set the default timezone (IST)
	date_default_timezone_set("Asia/Calcutta");
	
	//Connect to the MySQL database on the server
	$con = mysqli_connect("mysql9.000webhost.com","a7984291_un","pesgif118","a7984291_DB");
	if (mysqli_connect_errno($con))
	{
		echo "Failed to connect to MySQL: " . mysqli_connect_error();
	}
	
	//Get the various GET parameters from the URL (that were passed by the Python script)
	$id = $_GET['id'];
	$dt = date("Y-m-d H:i:s"); //Generate timestamp
	$crop = $_GET['crop'];
	$sen = $_GET['sen'];
	$read = $_GET['read'];
	$perc = $_GET['perc'];
	$city = $_GET['city'];
	$region = $_GET['region'];
	$lat = $_GET['lat'];
	$lon = $_GET['lon'];
	$zipc = $_GET['zipc'];
	
	//Insert the row into the table 'Readings'
	mysqli_query($con,"INSERT INTO Readings (`ID`, `DateTime`, `Crop`, `Sensor`, `Reading`, `Percentage`, `City`,
	`Region`, `Latitude`, `Longitude`, `Zip Code`) VALUES
	('".$id."','".$dt."','".$crop."','".$sen."','".$read."','".$perc."','".$city."','".$region."','".$lat."','".$lon."',
	'".$zipc."')");
?>