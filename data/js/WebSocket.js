
/* ***************** Websocket *************************** */

var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);

connection.onopen = function () {
    connection.send('Connect ' + new Date());
};

connection.onerror = function (error) {
    console.log('WebSocket Error ', error);
};

connection.onmessage = function (evt) {  
    console.log('Server: ', evt.data);
 if (evt.data[0] == "S") {
    console.log("YEAH!!!!!!!!! STOP ALARM!!!!!!");
    window.location.assign("/stop_alarm.html");
  }
};

connection.onclose = function(){
    console.log('WebSocket connection closed');
};

/* ****************** MP3 ************************ */

var pause_mp3 =false;   						// valeur pour pause et resume(restart)

function MP3 (value) { 							// PLAY MP3
   console.log( value +"mp3" );
   connection.send(value + "Mp3");
}

function pauseMP3 () {							// PAUSE MP3
pause_mp3 = ! pause_mp3;
  if (pause_mp3) {
   console.log("Pause Mp3" );
   connection.send('PauseMp3');
    }
   else {
   console.log("Restart Mp3");
   connection.send('StartMp3');
   }
}

function volEffect () {                                                   // VOLUME
  var v = document.getElementById('vol').value;
  console.log('Volume réglé sur ' + v);
  connection.send('*' + v);
  document.getElementById("displayVol").innerHTML = v;
}

function sendVOL () {							  // VOLUME SPAN
  var k = document.getElementById('vol').value;
  console.log('Volume Display: ' + k);
  document.getElementById("displayVol").innerHTML = k;
}

/* ****************** NIXIE ****************************** */

function nixieDown () {
   document.getElementById("nixieOFF").innerHTML = 'Nixie Off' ;
   console.log("Nixie DOWN" );
   connection.send('nixieDOWN');
}

function nixieUp () {
   document.getElementById("nixieOFF").innerHTML = 'Nixie On';
   console.log("Nixie UP" );
   connection.send('nixieUP');
}

function progNIXIE() {
   var checkBox = document.getElementById("checkboxNixie");
  if (checkBox.checked == true){
   var n = document.getElementById("myTimeNixie").value;
   document.getElementById("nixieOFF").innerHTML = 'Nixie Off : ' + n;
   console.log("Extinction des Nixies: " + n );
   connection.send('N' + n);
   } 
  else {
   document.getElementById("nixieOFF").innerHTML = 'Nixie On';
   console.log("Nixie UP" );
   connection.send('nixieUP');
   }
}

/* ********************* ALARM!!!!!!! ******************* */

// => REPEAT ALARM

function repeatALARM(){							 
    console.log("Repétition de la sonnerie dans 9 minutes ");
    connection.send("REPEAT_ALARM");
   // document.getElementById("alarmON").innerHTML = 'Alarme REPEAT';
}

// => STOP ALARM

function stopALARM(){							
    console.log("Alarme stoppée");
    connection.send("STOP_ALARM");
   // document.getElementById("alarmON").innerHTML = 'Alarme OFF';
}

// => INIT ALARME

function initALARME(hourAlarm, minAlarm, idAlarm) {
var checkBox = document.getElementById(idAlarm);
 if (checkBox.checked == true){
  connection.send("Z" + hourAlarm +":" + minAlarm );
  console.log("Alarme ON: réglée à " + hourAlarm + ":" + minAlarm );

var named = "toogleAlarmCheck";
var inputs = document.getElementsByName(named);
        for (var i = 0, max = inputs.length; i < max; i++)  {
         inputs[i].checked = false;
          }
  document.getElementById(idAlarm).checked = true;
  }else
   { 
    console.log("Alarme OFF");
    connection.send('ALARMOFF'); 
   }
}

 /* ________TIMER & CHRONO ______________________________________________

      _____________________ TIMER COMMANDES ____________________________ */


function timerCom(com, chroTim) {
        console.log(com + chroTim);
        connection.send(com +"_" + chroTim);
switch (com) {
     case "INIT" :
        document.getElementById('init'+ chroTim).style.backgroundColor = '#00878F';
        document.getElementById('play' + chroTim).style.backgroundColor = '#999';
        document.getElementById('pause'+ chroTim).style.backgroundColor = '#999';
     break;
     case "PLAY" : 
        document.getElementById('init'+ chroTim).style.backgroundColor = '#999';
        document.getElementById('play'+ chroTim).style.backgroundColor = '#00878F';
        document.getElementById('pause'+ chroTim).style.backgroundColor = '#999';
     break;
     case "PAUSE" :
        document.getElementById('init'+ chroTim).style.backgroundColor = '#999';
        document.getElementById('pause'+ chroTim).style.backgroundColor = '#00878F';
        document.getElementById('play'+ chroTim).style.backgroundColor = '#999';
     break;
     case "STOP" :
        document.getElementById('init'+ chroTim).style.backgroundColor = '#999';
        document.getElementById('play'+ chroTim).style.backgroundColor = '#999';
        document.getElementById('pause'+ chroTim).style.backgroundColor = '#999';
     break;
   }
}

     /* ____________________ INIT TIMER ___________________________________ */

function initTIMER (valM, valS, idTimer) {
var checkBox = document.getElementById(idTimer);
 if (checkBox.checked == true){
   console.log("init timer "+valM + ":" +valS);
   connection.send("W" +valM + ":" +valS);

var named = "toogleTimerCheck";
var inputs = document.getElementsByName(named);
        for (var i = 0, max = inputs.length; i < max; i++)  {
         inputs[i].checked = false
};
  document.getElementById(idTimer).checked = true;
} else { 
        console.log("TIMER OFF");
       }
}

      /* __________________  EDIT TIMER & ALARM ____________________________ */

 var new_open =false;

function editTimerOrAlarm(valueId) {                                       // New Alarm ou Timer => valueId = Timer or Alarm

 new_open = ! new_open;

   if(new_open) {
	document.getElementById('text_'+ valueId).style.display ="block";          // on Affiche "text_Alarm" ou "textTimer"
	document.getElementById('music_choice'+ valueId).style.display = "block";  // on Efface "music_choiceAlarm" ou "music_choiceTimer"
	document.getElementById('cmd_' + valueId).style.display = "none";                  // on Efface commandes alarm (repeat et stop)
	document.getElementById('xhttp' + valueId).innerHTML = "";                 // on Efface "xhttpAlarm" ou "xhttpTimer"

        var yo = document.createElement("strong");
        var newContent = document.createTextNode("Titre"); 
            yo.appendChild(newContent); 
	var element1 = document.getElementById("new_" +valueId);          // fils de ...
	    element1.appendChild(yo);

        var yeah = document.createElement("br");
	var element1 = document.getElementById("new_" +valueId);          // fils de ...
	    element1.appendChild(yeah);
        
        var y = document.createElement("INPUT");                           // creation d'un input type text
            y.setAttribute("type", "text");
            y.setAttribute("id", "set_txt_"+valueId);
            y.setAttribute("class", "text_new");
            y.setAttribute("align", "center");
            y.setAttribute("placeholder", "default");

        var element1 = document.getElementById("new_" +valueId);          // fils de ...
            element1.appendChild(y);

	var y = document.createElement("INPUT");                          // creation d'un input type time
	    y.setAttribute("type", "time");
	    y.setAttribute("id", "set_"+valueId);
	    y.setAttribute("style", "width:240px;margin-bottom:0px");
	    y.setAttribute("value", "00:00");

	var element1 = document.getElementById("new_" +valueId);          // fils de ...
	    element1.appendChild(y);


        var yeah = document.createElement("br");
	var element1 = document.getElementById("new_" +valueId);          // fils de ...
	    element1.appendChild(yeah);
	var x = document.createElement("INPUT");                          // creation d'un input type button
	    x.setAttribute("type", "button");
	    x.setAttribute("id", "ok");
	    x.setAttribute("value", "OK");
	    x.setAttribute("class", "inputChrono");
	    x.setAttribute("onclick", "creatTimerOrAlarm('" +valueId + "');");   // Appel function creatTimerOrAlarm()

	var element = document.getElementById("new_" +valueId);           // fils de ... 
	    element.appendChild(x);

   } else{
	document.getElementById('new_' +valueId).innerHTML = "";
	document.getElementById('text_' + valueId).style.display ="none";
	document.getElementById('music_choice' + valueId).style.display = "none";
//	document.getElementById('cmd_' + valueId).style.display = "block"; 
  }
}
      /* __________________  CREAT TIMER & ALARM ____________________________ */


function creatTimerOrAlarm(valueId) {  // valueId = Timer Or Alarm

 var hourTimerOrAlarm = document.getElementById("set_"+valueId).value;
 var txtTimerOrAlarm = document.getElementById("set_txt_"+valueId).value;
 
  if (txtTimerOrAlarm == null || txtTimerOrAlarm == "")
    {
    alert("Libellé obligatoire!!");
    return false;
    }
  else {
    console.log("Création " + valueId +" réglé sur: " + hourTimerOrAlarm + txtTimerOrAlarm);
     if ( valueId == "Alarm") {
       connection.send("Y" + hourTimerOrAlarm + "V" + txtTimerOrAlarm);
      }else {
       connection.send("W" + hourTimerOrAlarm + "X" + txtTimerOrAlarm);
      }
 console.log("Enregistrement " +valueId);
 connection.send("Rcd"+valueId);

 setTimeout(function(){                         // delay 1 second

    document.getElementById('text_' + valueId).style.display ="none";
    document.getElementById('new_' + valueId).innerHTML = "";
    document.getElementById('music_choice' + valueId).style.display = "none";
    document.getElementById('cmd_' + valueId).style.display = "block"; 

   if (valueId == "Alarm") {
      includeHTML('/alarm.html', 'xhttpAlarm');
 
     }
   if (valueId == "Timer") {
      includeHTML('/timer.html', 'xhttpTimer');
    }
    }, 1000); // fin delay seTimeout() 
  }
}

     /* ________________  REMOVE TIMER & ALARM ________________________________ */

function removeTimerOrAlarm(valueId) {
     console.log("Remove " + valueId);
     connection.send("Remove" + valueId);
     window.location.assign("/");
}
      /* ________________ AFFICHE MUSIC || RADIO ____________________________ */

function affMusic(valueId) {
document.getElementById('affMusic'+ valueId ).style.display = "block";
document.getElementById('affRadio' + valueId ).style.display = "none";
}

function affRadio(valueId) {
document.getElementById('affRadio' + valueId ).style.display = "block";
document.getElementById('affMusic' + valueId ).style.display = "none";
}

/* ******** SET PRESET RADIO & TRACK MUSIC ******************** */

function help(helpValue) {
 if ( helpValue == "radio") {
   alert("Tip: Selecet Preset radio");
  }
}

function setMusic(valueId) {
    var x = document.getElementById("mySelectMusic" +valueId).value;
    console.log("Set Music"+ valueId + x);
connection.send("M" + x);
}

function setRadio(valueId) {
    var x = document.getElementById("mySelectRadio" + valueId).value;
    console.log("Set Radio" + valueId + x);
connection.send("B" + x);
}

/* -------------------- ANIMATIONS -------------------------------------------- */

function Anim(value) {
  // Get the checkbox
  var checkBox = document.getElementById(value);
  // If the checkbox is checked, display the output text
  if (checkBox.checked == true){
    console.log("Animation :" + value + " ON");
    connection.send(value + "_on");
  } else {
    console.log("Animation :" + value + " OFF");
    connection.send( value + "_off");
  }
}

/* --------------- xhttp AJAX Timer -------------------------------------------- */

function includeHTML(url, iD) {
  var xhttp;
  xhttp=new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      callback(this, iD);
    }
  };
  xhttp.open("GET", url, true);
  xhttp.send();
}

function callback(xhttp,iD) {
  document.getElementById(iD).innerHTML =
  xhttp.responseText;

    if( iD == "nixieOFF" ) {
      document.getElementById(iD).innerHTML = '<br> ';
     }
    if( iD == "xhttpAlarm" || iD == "xhttpTimer" ){
     }	
}

/* ********************** NTP **************************** */

function setNtp(){
console.log("Set NTP");
connection.send("SETNTP");
}

/* ********************************************************** */
/*
function includeHTML(address, iD) {
  var z, i, elmnt, file, xhttp;
  //loop through a collection of all HTML elements:
  z = document.getElementsByTagName("div");
  for (i = 0; i < z.length; i++) {
    elmnt = z[i];
    //search for elements with a certain atrribute:
    file = elmnt.getAttribute(address);
    if (file) {
      //make an HTTP request using the attribute value as the file name:
      xhttp = new XMLHttpRequest();
     
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4) {
          if (this.status == 200) {elmnt.innerHTML = this.responseText;}
          elmnt.removeAttribute(address);
          includeHTML();
        }
      }      
      xhttp.open("GET", file, true);
      xhttp.send();

   if (iD == "xhttpTimer" || iD == "xhttpAlarm") {
      return;} else {
      document.getElementById(iD).innerHTML = '<br> ';
      return;} 
      }
  }
};
*/

/* ********************************************************** */
  // INIT TIME ALARM
/*
function initTIME () {                                                 
var checkBox = document.getElementById("checkboxAlarm");
if (checkBox.checked == true){
   var z = document.getElementById("myTime").value;
   document.getElementById("alarmON").innerHTML = 'Alarme ON : ' + z;
   console.log("Alarm ON: réglée à " + z );
   connection.send('Z' + z);
 } else
  { 
  console.log("Alarm OFF");
  connection.send('ALARMOFF');
  document.getElementById("alarmON").innerHTML = 'Alarme OFF';   
  }
}
*/

/* ********************************************************** */

/*
function affAlarm() {

var xhr = new XMLHttpRequest();
var url = "/alarm.json";

xhr.onreadystatechange = function() {
  if (xhr.readyState == 4 && xhr.status == 200) { 
    var jsonData = JSON.parse(xhr.responseText);
    showAlarm(jsonData);
  }
};

xhr.open("GET", url, true);
xhr.send();

function showAlarm(data) {
    var output = "<div>";
    var i;

    for (var i in data.alarme) {
 output += "<div class='toogleL'><input id=\"" + data.alarme[i].named + "\" type='checkbox' onclick=\"initALARME('" + data.alarme[i].heure + "','" + data.alarme[i].minutes + "','" + data.alarme[i].named + "')\" " + data.alarme[i].checked +  ">";
 output += "<strong id='set_txt_Alarm' style='margin-left:20px'>" + data.alarme[i].named + "</strong></div>";
 output += "<div class='toogleR'><strong>" + data.alarme[i].heure + ":" + data.alarme[i].minutes + "</strong></div><br>";
    }
 output += "</div>"; // Close list

 document.getElementById("alarmList").innerHTML = output;
 }
}
*/
