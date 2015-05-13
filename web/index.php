<?php 
function get_bootstrap_scripts() {
	return "<!-- Include all compiled plugins (below), or include individual files as needed -->" .
	   "<script src='../lib/bootstrap-3.3.4/dist/js/bootstrap.min.js'></script>" .
	   "<script src='../lib/bootstrap-3.3.4/js/tooltip.js'></script>" .
	   "<script src='../lib/bootstrap-3.3.4/js/popover.js'></script>" .
	   "<!-- Initialisations -->" .
	   "<script type='text/javascript'>" .
	   "$(function () { " .
	   "$(\"[data-toggle='tooltip']\").tooltip();" .
	   " });" .
	   "$(function () { " .
	   "$(\"[data-toggle='popover']\").popover({html : true});" .
	   " });" .
	   "</script>";
}
?> 

<!Doctype html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns='http://www.w3.org/1999/xhtml' xml:lang='fr' lang='fr'>
   <head>
   <meta http-equiv='Content-Type' content='text/html; charset=utf-8' />
   <meta name='viewport' content='width=device-width, initial-scale=1'>
   <title>ProjetPERI</title>
   <link rel='stylesheet' media='all' type='text/css' href='./lib/bootstrap-3.3.4/dist/css/bootstrap.css' />
   <script src='./lib/jquery/jquery-1.11.1.min.js'></script>
   <script src='./ajax.js'></script>
   <script src='./carControl.js'></script>
   </head>

   <body>
   <!-- Car control-->
      <div id='carControlContainer'>
	 <!-- speed up -->
	 <div id='speedUpContainer'>
	    <table>
	       <tr> 
		  <td><span class="glyphicon glyphicon-menu-left" aria-hidden="true"></span></td>
		  <td><br/><div class="progress" style='width: 200px' >
			<div class="wk-progress-bar progress-bar" role='progressbar' aria-valuenow='50' aria-valuemin='0' aria-valuemax='100' style='width: 50%'>50%</div>
		     </div>
		  </td>
		  <td><span class="glyphicon glyphicon-menu-right" aria-hidden="true"></span></td>
	       </tr>
	    </table>
	 </div>
	 
      </div>
   <?php get_bootstrap_scripts() ?>
   </body>
</html>