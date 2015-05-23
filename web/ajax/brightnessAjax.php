<?php

define('LIGHT_FIFO', "/tmp/lightpipe");

$ret = "";

/*
if(!file_exists(LIGHT_FIFO)){
   if(!posix_mkfifo(LIGHT_FIFO, 0766)){
      $ret = "ERROR 020: Cannot make FIFO";
      goto end_label;
   }
}

$pipe = fopen(LIGHT_FIFO, "r");
if( !$pipe ){
   $ret = "ERROR 021: Cannot open LIGHT_FIFO pipe";
   goto end_label;
}

$ret = ord(fread($pipe, 2));

fclose($pipe);
*/

$ret = 50;

end_label:
   header("Content-type: text/plain;charset=utf-8");
   echo $ret;
