<?php

define('TEMP_FIFO', "/tmp/temppipe");

$ret = "";

$ret = rand(15, 25);

end_label:
   header("Content-type: text/plain;charset=utf-8");
   echo $ret;
