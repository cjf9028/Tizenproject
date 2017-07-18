<?php


$x=$_POST['gyrox'];
$y=$_POST['gyroy'];
$z=$_POST['gyroz'];
$hr=$_POST['hrm'];
$servername="localhost";
echo $_SERVER['REQUEST_METHOD'];

if($_SERVER['REQUEST_METHOD'] != "POST") {
    header("HTTP/1.0 403 Forbidden");
    echo "disable";
}
else
	echo "able";
$user="root";

$password = "wise";

$database="user";

$conn=mysqli_connect($servername,$user,$password,"test2");
$insert="insert into watchdata values ($x,$y,$z,$hr)";
$row=mysqli_query($conn,$insert);
if(!row)
	echo "fail";
else
	echo "success";

?>