<?php

define('SPEED_FIFO', "/tmp/speedpipe");

$initSpeed = 50;

//$SPEED_RESET = 0;
//$SPEED_UP   = 2;
//$SPEED_DOWN = 1;

$ret = "";

if( !isset($_POST['arg']) ){
   $ret = "Error 010: no speed";
   goto end_label;
}
$arg = $_POST['arg'];

if( $arg > 100 || $arg < 0 ){
   $ret = "Error 011: Invalid Value";
   goto end_label;
}

if(!file_exists(SPEED_FIFO)){
   $ret = "ERROR 012: Daemon isn't lunched yet";
   goto end_label;
}

$pipe = fopen(SPEED_FIFO, "w");
if( !$pipe ){
   $ret = "ERROR 013: Cannot open SPEED_FIFO";
   goto end_label;
}

fwrite($pipe, chr($arg), 2);

fclose($pipe);

$ret = $arg;

end_label:
   header("Content-type: text/plain;charset=utf-8");
   echo $ret;