var turnVal  = 50;

function updateTurnProgressBar()
{
   var e = document.getElementById("turnProgressBar");
   e.setAttribute("aria-valuenow", turnVal);
   e.style.width = turnVal + "%";
   e.innerHTML = turnVal + "%";
}

function turnAjaxResponseHandler(xhr, arg)
{
   if (xhr.readyState === 4) {
      if (xhr.status === 200) {
	 if (xhr.responseText) {
	    if (/^Error/.test(xhr.responseText)) {
	       alert(xhr.responseText);
	    } else {
	       turnVal = parseInt(xhr.responseText);
	       updateTurnProgressBar();
	    }
	 } else {
	    alert("ERROR: response is not text");
	 }
      }
   }
}

function onClickLeftHandler()
{
   if (turnVal > 0) {
      var params = "arg=" + (turnVal - 5);
      ajax_car("./ajax/turnAjax.php", params, turnAjaxResponseHandler, 0);
   }
}

function onClickRightHandler()
{
   if (turnVal < 100) {
      var params = "arg=" + (turnVal + 5);
      ajax_car("./ajax/turnAjax.php", params, turnAjaxResponseHandler, 0);
   }
}