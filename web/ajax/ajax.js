function ajax_grid(url, flux, rappel, arg, method) {
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

/*
 * Scripts cible :
 * - speedUpAjax.php
 * - turnAjax.php
 */