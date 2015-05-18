<?php

$initSpeed = 50;

$ret = "";

if( !isset($_POST['arg']) ){
   $ret = "Error 403: no speed";
   goto end_label;
}
$newSpeed = $_POST['arg'];

/*
 * @todo ecriture dans le fichier
 */

end_label:
   header("Content-type: text/plain;charset=utf-8");
   echo $ret;