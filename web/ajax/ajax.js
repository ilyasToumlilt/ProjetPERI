function ajax_car(url, flux, rappel, arg, method) {
  var r = window.XMLHttpRequest ? new XMLHttpRequest() :
    (window.ActiveXObject ?  new ActiveXObject("Microsoft.XMLHTTP") : '');
  if (!r) return false;
  r.onreadystatechange = function () {rappel(r, arg);};
  r.open(method ? method : 'POST', url, true);
  if (flux)
      r.setRequestHeader("Content-Type", 
                         "application/x-www-form-urlencoded; ");
  r.send(flux);
  return true;
}

function ajax_brightness(url, flux, rappel, arg, method) {
  var r = window.XMLHttpRequest ? new XMLHttpRequest() :
    (window.ActiveXObject ?  new ActiveXObject("Microsoft.XMLHTTP") : '');
  if (!r) return false;
  r.onreadystatechange = function () {rappel(r, arg);};
  r.open(method ? method : 'POST', url, true);
  if (flux)
      r.setRequestHeader("Content-Type", 
                         "application/x-www-form-urlencoded; ");
  r.send(flux);
  return true;
}

function ajax_temperature(url, flux, rappel, arg, method) {
  var r = window.XMLHttpRequest ? new XMLHttpRequest() :
    (window.ActiveXObject ?  new ActiveXObject("Microsoft.XMLHTTP") : '');
  if (!r) return false;
  r.onreadystatechange = function () {rappel(r, arg);};
  r.open(method ? method : 'POST', url, true);
  if (flux)
      r.setRequestHeader("Content-Type", 
                         "application/x-www-form-urlencoded; ");
  r.send(flux);
  return true;
}