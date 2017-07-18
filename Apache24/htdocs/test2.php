<?php
$x=$_POST['gyrox'];
$y=$_POST['gyroy'];
$z=$_POST['gyroz'];
$hr=$_POST['hrm'];
$servername="localhost";

$user="root";

$password = "wise";

$database="test2";
$date =  date("Y-m-d H:i:s",time());

$conn=mysqli_connect($servername,$user,$password,"test2");
$insert="insert into watchdata values ($x,$y,$z,$hr)";
$row=mysqli_query($conn,$insert);
?>