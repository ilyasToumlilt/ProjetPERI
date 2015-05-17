var speedVal = 50;

function updateSpeedUpProgressBar()
{
   var e = document.getElementById("speedUpProgressBar");
   e.setAttribute("aria-valuenow", speedVal);
   e.style.width = speedVal + "%";
   e.innerHTML = speedVal + "%";
}

function onClickDownHandler()
{
   if( speedVal > 0 ){
      speedVal -= 5;
      updateSpeedUpProgressBar();
   }
}
   
function onClickUpHandler()
{
   if( speedVal < 100 ){
      speedVal += 5;
      updateSpeedUpProgressBar();
   }
}