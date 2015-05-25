var speedVal = 50;

function updateSpeedUpProgressBar()
{
   var e = document.getElementById("speedUpProgressBar");
   e.setAttribute("aria-valuenow", speedVal);
   e.style.width = speedVal + "%";
   e.innerHTML = speedVal + "%";
}

function speedUpAjaxResponseHandler(xhr, arg)
{
   if (xhr.readyState === 4) {
      if (xhr.status === 200) {
	 if (xhr.responseText) {
	    if (/^ERROR/.test(xhr.responseText)) {
	       alert(xhr.responseText);
	    } else {
	       speedVal = parseInt(xhr.responseText);
	       updateSpeedUpProgressBar();
	    }
	 } else {
	    alert("ERROR: response is not text");
	 }
      }
   }
}

function onClickDownHandler()
{
   if (speedVal > 0) {
      var params = "arg=" + (speedVal - 5);
      ajax_car("./ajax/speedUpAjax.php", params, speedUpAjaxResponseHandler, 0);
   }
}

function onClickUpHandler()
{
   if (speedVal < 100) {
      var params = "arg=" + (speedVal + 5);
      ajax_car("./ajax/speedUpAjax.php", params, speedUpAjaxResponseHandler, 0);
   }
}