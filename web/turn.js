var turnVal  = 50;

function updateTurnProgressBar()
{
   var e = document.getElementById("turnProgressBar");
   e.setAttribute("aria-valuenow", turnVal);
   e.style.width = turnVal + "%";
   e.innerHTML = turnVal + "%";
}

function onClickLeftHandler()
{
   if( turnVal > 0 ){
      turnVal -= 5;
      updateTurnProgressBar();
   }
}

function onClickRightHandler()
{
   if( turnVal < 100 ){
      turnVal += 5;
      updateTurnProgressBar();
   }
}