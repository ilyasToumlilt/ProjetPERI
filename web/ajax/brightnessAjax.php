<?php

$ret = rand(0, 100);

end_label:
   header("Content-type: text/plain;charset=utf-8");
   echo $ret;
