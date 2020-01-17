<?php
// globals for test use
$GLOBALS['$servername'] = "127.0.0.1";
$GLOBALS['$username'] = "listuser";
$GLOBALS['$password'] = "listuser";
$GLOBALS['$dbname'] = "test";
//live server globals
//$GLOBALS['$servername'] = "localhost";
//$GLOBALS['$username'] = "root";
//$GLOBALS['$password'] = "password";
//$GLOBALS['$dbname'] = "Temperature";

//test input to see which function gets used
    $action = $_GET['Action'];
    switch($action) {
        case 'insert' : insert();break;
        case 'get' : get();break;
}

function insert(){
  $servername = $GLOBALS['$servername'];
  $username = $GLOBALS['$username'];
  $password = $GLOBALS['$password'];
  $dbname = $GLOBALS['$dbname'];
  // Create connection
  $conn = new mysqli($servername, $username, $password, $dbname, 0, '/path/to/mysql/socket');
  // Check connection
  if ($conn->connect_error) {
      die("Connection failed: " . $conn->connect_error);
  }
  $Temperature=$_GET["Temperature"];
  $Humidity=$_GET["Humidity"];
  $TimeStamp=$_GET["TimeStamp"];
  $sql = "INSERT INTO Readings (Temperature, Humidity, TimeStamp)
  VALUES ('$Temperature','$Humidity','$TimeStamp')";
  if ($conn->query($sql) === TRUE) {
      echo "New record created successfully";
  } else {
      echo "Error: " . $sql . "<br>" . $conn->error;
  }
  $conn->close();
}

function get(){
  $servername = $GLOBALS['$servername'];
  $username = $GLOBALS['$username'];
  $password = $GLOBALS['$password'];
  $dbname = $GLOBALS['$dbname'];
  $output=array();
  // Create connection
  $conn = new mysqli($servername, $username, $password, $dbname, 0,'/path/to/mysql/socket');
  //$conn = new mysqli($servername, $username, $password, $dbname, 3306);
  // Check connection
  if ($conn->connect_error) {
      die("Connection failed: " . $conn->connect_error);
  }
  $sql = "SELECT Temperature, Humidity, TimeStamp FROM `Readings` WHERE TimeStamp > date_add(now(), INTERVAL -7 DAY) ORDER by TimeStamp ASC";
  $result = $conn->query($sql);
  if ($result->num_rows > 0) {
     //stuff into array
     while($row = $result->fetch_assoc()) {
     //make a class object
     $line = (object) [
         'Temperature' => $row['Temperature'],
         'Humidity' => $row['Humidity'],
         'TimeStamp' => $row['TimeStamp']
     ];
     //push into array
     array_push($output,$line);
     }
  } else echo '';//need blank array for datatable if empty
  //return as json
  echo json_encode($output);
  $conn->close();
}
?>
