

$(document).ready(function() {
   var tempVal = 0;
   var tempLock = 0;

   function updateTemperatureField()
   {
      document.getElementById("temperatureValue").innerHTML = tempVal;
   }
   
   function receiveTemperature(xhr, arg){
      if (xhr.readyState === 4) {
	 if (xhr.status === 200) {
	    if (xhr.responseText) {
	       if (/^Error/.test(xhr.responseText)) {
		  alert(xhr.responseText);
	       } else {
		  /* Si on est là c'est que la réponse est bonne */
		  tempVal = parseInt(xhr.responseText);
		  updateTemperatureField();
		  tempLock = 0;
		  document.getElementById("temperatureLoading").style.display = "none";
	       }
	    } else {
	       alert("ERROR: response is not valid");
	    }
	 }
      }
   }  
   
   var temperatureUpdateInterval = 1000;
   function updateTemperature() {
      if( !tempLock ){
	 tempLock = 1;
	 document.getElementById("temperatureLoading").style.display = "initial";
	 ajax_temperature("./ajax/temperatureAjax.php", 0, receiveTemperature, 0);
      }
      setTimeout(updateTemperature, temperatureUpdateInterval);
   }
   
   updateTemperatureField();
   updateTemperature();
});