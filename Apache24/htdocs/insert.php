<?php
$x=$_POST['gyrox'];
$y=$_POST['gyroy'];
$z=$_POST['gyroz'];
$hr=$_POST['hrm'];
$servername="localhost";

$user="root";

$password = "wise";

$database="watch";
$date =  date("Y-m-d H:i:s",time());

$conn=mysqli_connect($servername,$user,$password,$database);
$insert="insert into watchdata values ($x,$y,$z,$hr,$time)";
$row=mysqli_query($conn,$insert);
if(!$row)
	echo "fail";
else
	echo "success";
?>