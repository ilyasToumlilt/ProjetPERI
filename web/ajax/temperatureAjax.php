<?php

define('TEMP_FIFO', "/tmp/temppipe");

$ret = "";

/**
 * @todo je ne dois pas créer le pipe, mais tant qu'il n'existe pas la section doit être inactive
 */
if(!file_exists(TEMP_FIFO)){
   if(!posix_mkfifo(TEMP_FIFO, 0766)){
      $ret = "ERROR 030: Cannot make FIFO";
      goto end_label;
   }
}

$pipe = fopen(TEMP_FIFO, "r");
if( !$pipe ){
   $ret = "ERROR 031: Cannot open TEMP_FIFO pipe";
   goto end_label;
}

$ret = ord(fread($pipe, 2));

fclose($pipe);

end_label:
   header("Content-type: text/plain;charset=utf-8");
   echo $ret;
