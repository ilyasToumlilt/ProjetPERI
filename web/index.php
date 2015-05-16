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
	 <!-- extended bootstrapCss -->
	 <link href="lib/extendedBootstrap/css/bootstrap.min.css" rel="stylesheet" />
	 <link href="lib/extendedBootstrap/css/font-awesome.min.css" rel="stylesheet" />

	 <link rel="stylesheet" href="lib/extendedBootstrap/css/jquery-ui-1.10.3.custom.min.css" />
	 <link rel="stylesheet" href="lib/extendedBootstrap/css/chosen.css" />
	 <link rel="stylesheet" href="lib/extendedBootstrap/css/datepicker.css" />
	 <link rel="stylesheet" href="lib/extendedBootstrap/css/bootstrap-timepicker.css" />
	 <link rel="stylesheet" href="lib/extendedBootstrap/css/colorpicker.css" />
	 <link href="lib/extendedBootstrap/css/refineslide.css" rel="stylesheet" />
	 <link href="lib/extendedBootstrap/css/bootstro.css" rel="stylesheet" />

	 <link rel="stylesheet" href="lib/extendedBootstrap/css/jquery-slider.css" />
	 <link rel="stylesheet" href="lib/extendedBootstrap/css/checkbox-radio-switch.css" />
	 <link href="lib/extendedBootstrap/css/extend.css" rel="stylesheet" />
	 <!-- extended 2.0 -->
	 <!-- end -->
	 <link href="./indexDesign.css" rel="stylesheet" />
	 <script src='./lib/jquery/jquery-1.11.1.min.js'></script>
	 <script src='./ajax.js'></script>
	 <script src='./charts-flot.js'></script>
	 <script src='./speedUp.js'></script>
	 <script src='./turn.js'></script>
   </head>

   <body data-twttr-rendered="true" cz-shortcut-listen="true" id="top" data-spy="scroll" data-target=".subnav" data-offset="80">
      <!-- Car control-->
      <div id="carControlContainer" class="panel panel-default">
	 <div class="panel-heading">
	    <h3 class="panel-title">
	       <span class="glyphicon glyphicon-road" aria-hidden="true"></span> Car Control</h3>
	 </div>
	 <div class="panel-body">
	    <!-- speed up -->
	    <div id='speedUpContainer'>
	       <table>
		  <tr>
		     <td style="cursor: pointer" onclick="onClickDownHandler()"><span class="glyphicon glyphicon-menu-left" aria-hidden="true" ></span></td>
		     <td><br/><div class="progress" style='width: 200px' >
			   <div id="speedUpProgressBar" class="wk-progress-bar progress-bar" role='progressbar' aria-valuenow='50' aria-valuemin='0' aria-valuemax='100' style='width: 50%'>50%</div>
			</div>
		     </td>
		     <td style="cursor: pointer" onclick="onClickUpHandler()"><span class="glyphicon glyphicon-menu-right" aria-hidden="true"></span></td>
		  </tr>
	       </table>
	    </div>
	    <!-- turn -->
	    <div id='turnContainer'>
	       <table>
		  <tr> 
		     <td style="cursor: pointer" onclick="onClickLeftHandler()"><span class="glyphicon glyphicon-menu-left" aria-hidden="true"></span></td>
		     <td><br/><div class="progress" style='width: 200px' >
			   <div id="turnProgressBar" class="wk-progress-bar progress-bar" role='progressbar' aria-valuenow='50' aria-valuemin='0' aria-valuemax='100' style='width: 50%'>50%</div>
			</div>
		     </td>
		     <td style="cursor: pointer" onclick="onClickRightHandler()"><span class="glyphicon glyphicon-menu-right" aria-hidden="true"></span></td>
		  </tr>
	       </table>
	    </div>
	 </div>
      </div>
      <!-- meteo station -->
      <div id='meteoStationContainer'>
	 <div class="panel panel-default">
	    <div class="panel-heading">
	       <h3 class="panel-title"><span class="glyphicon glyphicon-tint" aria-hidden="true"></span> Meteo Station</h3>
	    </div>
	    <div class="panel-body">
	       <?php
	       echo $temperature
	       ?>
	    </div>
	 </div>
      </div>
      <!-- graph -->
      <div id='graphContainer'>
	 <div class="panel panel-default">
	    <div class="panel-heading">
	       <h3 class="panel-title"><span class="glyphicon glyphicon-adjust" aria-hidden="true"></span> Brightness graph</h3>
	    </div>
	    <div class="panel-body">
	       <div class="box">
		  <div class="box-header">
		     <h2><i class="fa fa-list-alt"></i><span class="break"></span>Realtime</h2>
		     <div class="box-icon">
			<a class="btn-setting" href="charts-flot.html#"><i class="fa fa-wrench"></i></a>
			<a class="btn-minimize" href="charts-flot.html#"><i class="fa fa-chevron-up"></i></a>
			<a class="btn-close" href="charts-flot.html#"><i class="fa fa-times"></i></a>
		     </div>
		  </div>
		  <div class="box-content">
		     <div style="height: 190px; padding: 0px; position: relative;" id="realtimechart"><canvas class="flot-base" style="direction: ltr; position: absolute; left: 0px; top: 0px; width: 1531px; height: 190px;" width="1531" height="190"></canvas><div class="flot-text" style="position: absolute; top: 0px; left: 0px; bottom: 0px; right: 0px; font-size: smaller; color: rgb(84, 84, 84);"><div class="flot-y-axis flot-y1-axis yAxis y1Axis" style="position: absolute; top: 0px; left: 0px; bottom: 0px; right: 0px; display: block;"><div style="position: absolute; top: 174px; left: 13px; text-align: right;" class="flot-tick-label tickLabel">0</div><div style="position: absolute; top: 131px; left: 6px; text-align: right;" class="flot-tick-label tickLabel">25</div><div style="position: absolute; top: 87px; left: 6px; text-align: right;" class="flot-tick-label tickLabel">50</div><div style="position: absolute; top: 44px; left: 6px; text-align: right;" class="flot-tick-label tickLabel">75</div><div style="position: absolute; top: 0px; left: 0px; text-align: right;" class="flot-tick-label tickLabel">100</div></div></div><canvas class="flot-overlay" style="direction: ltr; position: absolute; left: 0px; top: 0px; width: 1531px; height: 190px;" width="1531" height="190"></canvas></div>
		     <p>You can update a chart periodically to get a real-time effect by using a timer to insert the new data in the plot and redraw it.</p>
		     <p>Time between updates: <input type="text" style="text-align: right; width:5em" value="" id="updateInterval"> milliseconds</p>
		  </div>
	       </div>
	    </div>
	 </div>

      </div>
      <!-- /container -->

      <script>
	 (function(i, s, o, g, r, a, m) {
	    i['GoogleAnalyticsObject'] = r;
	    i[r] = i[r] || function() {
	       (i[r].q = i[r].q || []).push(arguments)
	    }, i[r].l = 1 * new Date();
	    a = s.createElement(o),
		    m = s.getElementsByTagName(o)[0];
	    a.async = 1;
	    a.src = g;
	    m.parentNode.insertBefore(a, m)
	 })(window, document, 'script', '//www.google-analytics.com/analytics.js', 'ga');

	 ga('create', 'UA-42861651-1', 'extendbootstrap.com');
	 ga('send', 'pageview');

      </script>
      <!-- enxtendedBootstrap scripts -->

      <script src="lib/extendedBootstrap/js/bootstrap.min.js"></script>
      <script src="lib/extendedBootstrap/js/jquery-ui-1.10.3.custom.min.js"></script>

      <script src="lib/extendedBootstrap/js/flot/jquery.flot.min.js"></script>
      <script src="lib/extendedBootstrap/js/flot/jquery.flot.pie.min.js"></script>
      <script src="lib/extendedBootstrap/js/flot/jquery.flot.resize.min.js"></script>

      <script src="lib/extendedBootstrap/js/date-time/bootstrap-datepicker.min.js"></script>
      <script src="lib/extendedBootstrap/js/date-time/bootstrap-timepicker.min.js"></script>
      <script src="lib/extendedBootstrap/js/bootstrap-colorpicker.min.js"></script>

      <script src="lib/extendedBootstrap/js/jquery.knob.min.js"></script>
      <script src="lib/extendedBootstrap/js/jquery.autosize-min.js"></script>
      <script src="lib/extendedBootstrap/js/jquery.inputlimiter.1.3.1.min.js"></script>
      <script src="lib/extendedBootstrap/js/jquery.maskedinput.min.js"></script>
      <script src="lib/extendedBootstrap/js/jquery.refineslide.js"></script>
      <script src="lib/extendedBootstrap/js/bootbox.js"></script>
      <script src="lib/extendedBootstrap/js/bootstro.min.js"></script>
      <!-- extended Bootstrap 2.0 -->
      <!-- end --> 

      <!-- extended not included -->
      <script type="text/javascript">
	 $(function() {
	    var data = [
	       {label: "Green", data: 30.7, color: "#48CA3B"},
	       {label: "Blue", data: 24.5, color: "#00BCE1"},
	       {label: "Purple", data: 10.2, color: "#4D3A7D"},
	       {label: "Red", data: 18.6, color: "#AD1D28"},
	       {label: "Yellow", data: 16, color: "#DEBB27"}
	    ];

	    var placeholder = $('#piechart-placeholder').css({'width': '70%', 'min-height': '150px'});
	    $.plot(placeholder, data, {
	       series: {
		  pie: {
		     show: true,
		     tilt: 0.8,
		     highlight: {
			opacity: 0.25
		     },
		     stroke: {
			color: '#fff',
			width: 2
		     },
		     startAngle: 2

		  }
	       },
	       legend: {
		  show: true,
		  position: "ne",
		  labelBoxBorderColor: null,
		  margin: [-30, 15]
	       }
	       ,
	       grid: {
		  hoverable: true,
		  clickable: true
	       },
	       tooltip: true, //activate tooltip
	       tooltipOpts: {
		  content: "%s : %y.1",
		  shifts: {
		     x: -30,
		     y: -50
		  }
	       }

	    });


	    var $tooltip = $("<div class='tooltip top in' style='display:none;'><div class='tooltip-inner'></div></div>").appendTo('body');
	    placeholder.data('tooltip', $tooltip);
	    var previousPoint = null;

	    placeholder.on('plothover', function(event, pos, item) {
	       if (item) {
		  if (previousPoint != item.seriesIndex) {
		     previousPoint = item.seriesIndex;
		     var tip = item.series['label'] + " : " + item.series['percent'] + '%';
		     $(this).data('tooltip').show().children(0).text(tip);
		  }
		  $(this).data('tooltip').css({top: pos.pageY + 10, left: pos.pageX + 10});
	       } else {
		  $(this).data('tooltip').hide();
		  previousPoint = null;
	       }

	    });


	    var d1 = [];
	    for (var i = 0; i < Math.PI * 2; i += 0.5) {
	       d1.push([i, Math.sin(i)]);
	    }

	    var d2 = [];
	    for (var i = 0; i < Math.PI * 2; i += 0.5) {
	       d2.push([i, Math.cos(i)]);
	    }

	    var d3 = [];
	    for (var i = 0; i < Math.PI * 2; i += 0.2) {
	       d3.push([i, Math.tan(i)]);
	    }


	    var sales_charts = $('#sales-charts').css({'width': '100%', 'height': '220px'});
	    $.plot("#sales-charts", [
	       {label: "Yellow", data: d1},
	       {label: "Blue", data: d2},
	       {label: "Red", data: d3}
	    ], {
	       hoverable: true,
	       shadowSize: 0,
	       series: {
		  lines: {show: true},
		  points: {show: true}
	       },
	       xaxis: {
		  tickLength: 0
	       },
	       yaxis: {
		  ticks: 10,
		  min: -2,
		  max: 2,
		  tickDecimals: 3
	       },
	       grid: {
		  backgroundColor: {colors: ["#fff", "#fff"]},
		  borderWidth: 1,
		  borderColor: '#555'
	       }
	    });

	    $("#input-size-slider").css('width', '200px').slider({
	       value: 1,
	       range: "min",
	       min: 1,
	       max: 6,
	       step: 1,
	       slide: function(event, ui) {
		  var sizing = ['', 'input-mini', 'input-small', 'input-medium', 'input-large', 'input-xlarge', 'input-xxlarge'];
		  var val = parseInt(ui.value);
		  $('#form-field-4').attr('class', sizing[val]).val('.' + sizing[val]);
	       }
	    });

	    $("#input-span-slider").slider({
	       value: 1,
	       range: "min",
	       min: 1,
	       max: 11,
	       step: 1,
	       slide: function(event, ui) {
		  var val = parseInt(ui.value);
		  $('#form-field-5').attr('class', 'span' + val).val('.span' + val).next().attr('class', 'span' + (12 - val)).val('.span' + (12 - val));
	       }
	    });


	    var $tooltip = $("<div class='tooltip right in' style='display:none;'><div class='tooltip-arrow'></div><div class='tooltip-inner'></div></div>").appendTo('body');
	    $("#slider-range").css('height', '200px').slider({
	       orientation: "vertical",
	       range: true,
	       min: 0,
	       max: 100,
	       values: [17, 67],
	       slide: function(event, ui) {
		  var val = ui.values[$(ui.handle).index() - 1] + "";

		  var pos = $(ui.handle).offset();
		  $tooltip.show().children().eq(1).text(val);
		  $tooltip.css({top: pos.top - 10, left: pos.left + 18});

		  //$(this).find('a').eq(which).attr('data-original-title' , val).tooltip('show');
	       }
	    });
	    $('#slider-range a').tooltip({placement: 'right', trigger: 'manual', animation: false}).blur(function() {
	       $tooltip.hide();
	       //$(this).tooltip('hide');
	    });
	    //$('#slider-range a').tooltip({placement:'right', animation:false});

	    $("#slider-range-max").slider({
	       range: "max",
	       min: 1,
	       max: 10,
	       value: 2,
	       //slide: function( event, ui ) {
	       //  $( "#amount" ).val( ui.value );
	       //}
	    });
	    //$( "#amount" ).val( $( "#slider-range-max" ).slider( "value" ) );

	    $("#eq > span").css({width: '90%', float: 'left', margin: '15px'}).each(function() {
	       // read initial values from markup and remove that
	       var value = parseInt($(this).text(), 10);
	       $(this).empty().slider({
		  value: value,
		  range: "min",
		  animate: true

	       });
	    });


	 })
      </script>


      <script type="text/javascript">
	 $(document).ready(function() {

	    $('.date-picker').datepicker();
	    $('#timepicker1').timepicker({
	       minuteStep: 1,
	       showSeconds: true,
	       showMeridian: false
	    });

	    $('.colorpicker').colorpicker();
	    $('.colorpicker input').click(function() {
	       $(this).parents('.colorpicker').colorpicker('show');
	    })


	    $('.knob').knob();

	    var $upper = $('#upper');

	    $('#images').refineSlide({
	       transition: 'random',
	       onInit: function() {
		  var slider = this.slider,
			  $triggers = $('.translist').find('> li > a');

		  $triggers.parent().find('a[href="#_' + this.slider.settings['transition'] + '"]').addClass('active');

		  $triggers.on('click', function(e) {
		     e.preventDefault();

		     if (!$(this).find('.unsupported').length) {
			$triggers.removeClass('active');
			$(this).addClass('active');
			slider.settings['transition'] = $(this).attr('href').replace('#_', '');
		     }
		  });

		  function support(result, bobble) {
		     var phrase = '';

		     if (!result) {
			phrase = ' not';
			$upper.find('div.bobble-' + bobble).addClass('unsupported');
			$upper.find('div.bobble-js.bobble-css.unsupported').removeClass('bobble-css unsupported').text('JS');
		     }
		  }

		  support(this.slider.cssTransforms3d, '3d');
		  support(this.slider.cssTransitions, 'css');
	       }
	    });

	 });
      </script>

      <script>
	 var Example = (function() {
	    "use strict";

	    var elem,
		    hideHandler,
		    that = {};

	    that.init = function(options) {
	       elem = $(options.selector);
	    };

	    that.show = function(text) {
	       clearTimeout(hideHandler);

	       elem.find("span").html(text);
	       elem.fadeIn();

	       hideHandler = setTimeout(function() {
		  that.hide();
	       }, 4000);
	    };

	    that.hide = function() {
	       elem.fadeOut();
	    };

	    return that;

	 }());

	 $(function() {

	    Example.init({
	       "selector": ".bb-alert"
	    });

	 });
      </script>

      <script type="text/javascript">

	 $(function() {

	    var demos = {};

	    $(document).on("click", "a[data-bb]", function(e) {
	       e.preventDefault();
	       var type = $(this).data("bb");

	       if (typeof demos[type] === 'function') {
		  demos[type]();
	       }
	    });

	    // let's namespace the demo methods; it makes them easier
	    // to invoke
	    demos.alert = function() {
	       bootbox.alert("Hello world!");
	    };

	    demos.alert_callback = function() {
	       bootbox.alert("Hello world!");
	    };

	    demos.confirm = function() {
	       bootbox.confirm("Are you sure?", function(result) {
		  Example.show("Confirm result: " + result);
	       });
	    };

	    demos.prompt = function() {
	       bootbox.prompt("What is your name?", function(result) {
		  if (result === null) {
		     Example.show("Prompt dismissed");
		  } else {
		     Example.show("Hi <b>" + result + "</b>");
		  }
	       });
	    };

	    demos.dialog = function() {
	       bootbox.dialog("I am a custom dialog", [{
		     "label": "Success!",
		     "class": "btn-success",
		     "callback": function() {
			Example.show("great success");
		     }
		  }, {
		     "label": "Danger!",
		     "class": "btn-danger",
		     "callback": function() {
			Example.show("uh oh, look out!");
		     }
		  }, {
		     "label": "Click ME!",
		     "class": "btn-primary",
		     "callback": function() {
			Example.show("Primary button");
		     }
		  }, {
		     "label": "Just a button..."
		  }]);
	    };


	 })
      </script>

      <script type="text/javascript">
	 // $(document).ready(function(){
	 //   bootstro.start(selector, options);
	 // });
      </script>

      <script type="text/javascript">

	 $(document).ready(function() {
	    $("#demo").click(function() {
	       bootstro.start(".bootstro", {
		  onComplete: function(params)
		  {
		     alert("Reached end of introduction with total " + (params.idx + 1) + " slides");
		  },
		  onExit: function(params)
		  {
		     alert("Introduction stopped at slide #" + (params.idx + 1));
		  },
	       });
	    });
	    $(".demo_stopOn").click(function() {
	       alert('Clicking on the backdrop or Esc will NOT stop the show')
	       bootstro.start('.bootstro', {stopOnBackdropClick: false, stopOnEsc: false});
	    });
	    $(".demo_size1").click(function() {
	       bootstro.start('.bootstro_size1');
	    });
	    $(".demo_nonav").click(function() {
	       bootstro.start('.bootstro', {
		  nextButton: '',
		  prevButton: '',
		  finishButton: ''
	       });
	    });
	    $(".demo_ajax").click(function() {
	       bootstro.start('', {
		  url: './bootstro.json',
	       });
	    });
	 });

      </script>
      <!-- end -->
      <?php get_bootstrap_scripts() ?>
   </body>
</html>