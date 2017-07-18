<?php
error_reporting(E_ALL | E_STRICT);
error_log($_POST);
$json_string = json_decode(stripslashes($_POST), true);
// === parse json string ===
$json_string = '{ "programmers": [
        { "firstName": "Brett", "lastName":"McLaughlin", "email": "brett@newInstance.com" },
        { "firstName": "Jason", "lastName":"Hunter", "email": "jason@servlets.com" }
]}';
$data_array = json_decode($json_string, true);
for($k=0;$k<count( $data_array);$k++)
{
	$x=$data_array['gyrox'][$k];
	$y=$data_array['gyroy'][$k];
	$z=$data_array['gyroz'][$k];
	$hrm=$data_array['hrm'][$k];
}
$servername="localhost";

$user="root";

$password = "wise";

$database="user";

$conn=mysqli_connect($servername,$user,$password,"test2");
$insert="insert into watchdata values ($x,$y,$z,$hrm)";
$row=mysqli_query($conn,$insert);
if(!$row)
	echo "fail";
else
	echo "success";
?>