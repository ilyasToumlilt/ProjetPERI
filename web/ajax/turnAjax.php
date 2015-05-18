<?php

$initRotation = 50;

$ret = "";
if( !isset($_POST['arg'])){
   $ret = "Error 403: incompatible argument";
   goto end_label;
}

$newRot = $_POST['arg'];
if( $newRot < 0 || $newRot > 100){
   $ret = "Error 070: invalid rotation value";
   goto end_label;
}

/*
 * @todo copy_to_c.
 */

end_label:
   header("Content-type: text/plain;charset=utf-8");
   echo $ret;