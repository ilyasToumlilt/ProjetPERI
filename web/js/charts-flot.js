function randNum() {
   return ((Math.floor(Math.random() * (1 + 40 - 20))) + 20) * 1200;
}

function randNum2() {
   return ((Math.floor(Math.random() * (1 + 40 - 20))) + 20) * 500;
}

function randNum3() {
   return ((Math.floor(Math.random() * (1 + 40 - 20))) + 20) * 300;
}

function randNum4() {
   return ((Math.floor(Math.random() * (1 + 40 - 20))) + 20) * 100;
}

$(document).ready(function() {


   var data = [], totalPoints = 300;
   var res = [];

   function initChart() {
      for(var i = 0; i < totalPoints; i++){
	 data.push(0);
	 res.push([i, 0]);
      }
      
      return res;
   }

   function updateChart(newValue) {
      if( data.length > 0)
	 data = data.slice(1);
      
      data.push(newValue);
      
      // zip the generated y values with the x values
      res = [];
      for(var i = 0; i < data.length; i++){
	 res.push([i, data[i]]);
      }
      
   }

   function dataReceiver(xhr, arg) {
      if (xhr.readyState === 4) {
	 if (xhr.status === 200) {
	    if (xhr.responseText) {
	       if (/^Error/.test(xhr.responseText)) {
		  alert(xhr.responseText);
	       } else {
		  /* Si on est là c'est que la réponse est bonne */
		  updateChart(parseInt(xhr.responseText));
	       }
	    } else {
	       alert("ERROR: response is not valid");
	    }
	 }
      }
   }

   // setup control widget
   var updateInterval = 100;
   $("#updateInterval").val(updateInterval).change(function() {
      var v = $(this).val();
      if (v && !isNaN(+v)) {
	 updateInterval = +v;
	 if (updateInterval < 1)
	    updateInterval = 1;
	 if (updateInterval > 2000)
	    updateInterval = 2000;
	 $(this).val("" + updateInterval);
      }
   });


   if ($("#realtimechart").length)
   {
      var options = {
	 series: {shadowSize: 1},
	 lines: {fill: true, fillColor: {colors: [{opacity: 1}, {opacity: 0.1}]}},
	 yaxis: {min: 0, max: 100},
	 xaxis: {show: false},
	 colors: ["#F4A506"],
	 grid: {tickColor: "#dddddd",
	    borderWidth: 0
	 },
      };
      var plot = $.plot($("#realtimechart"), [initChart()], options);
      function update() {
	 ajax_brightness("./ajax/brightnessAjax.php", 0, dataReceiver, 0);
	 //document.getElementById("debugContainer").innerHTML = "-" + res.toString();
	 
	 plot.setData([res]);
	 // since the axes don't change, we don't need to call plot.setupGrid()
	 plot.draw();
	 
	 setTimeout(update, updateInterval);
      }

      update();
   }

});