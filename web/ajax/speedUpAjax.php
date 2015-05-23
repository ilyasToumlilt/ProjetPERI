<?php

$initSpeed = 50;

//$SPEED_RESET = 0;
//$SPEED_UP   = 2;
//$SPEED_DOWN = 1;

$ret = "";

if( !isset($_POST['arg']) ){
   $ret = "Error 403: no speed";
   goto end_label;
}
$arg = $_POST['arg'];

if( $arg > 100 || $arg < 0 ){
   $ret = "Error 102: Invalid Value";
   goto end_label;
}

$ret = $arg;

end_label:
   header("Content-type: text/plain;charset=utf-8");
   echo $ret;