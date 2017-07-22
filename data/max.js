var set_real_time;

//Init load
$(load);

//Switched handler
$(".switch_cl").click(function(){
    console.log("click");
    $(this).toggleClass("btn-success btn-danger");
});

function handleServerResponse() {
    clearTimeout(set_real_time);
    var res = jsonResponse.time.split(":");
    real_time(hours = res[0], min = res[1], sec = res[2]);
    document.body.style.backgroundColor = "rgb(" + jsonResponse.rgb + ")";
}

function real_time(hours, min, sec) {
    sec = Number(sec) + 1;
    if (sec >= 60) {
        min = Number(min) + 1;
        sec = 0;
    }
    if (min >= 60) {
        hours = Number(hours) + 1;
        min = 0;
    }
    if (hours >= 24) {
        hours = 0
    };
    document.getElementById("time").innerHTML = hours + ":" + min + ":" + sec;
    set_real_time = setTimeout("real_time(" + hours + "," + min + "," + sec + ");", 1000);
}

function load_time(submit) {
    server = "/Time";
    send_request(submit, server);
    load();
}

function time_zone(submit) {
    server = "/TimeZone?timezone=" + val('timezone');
    send_request(submit, server);
}

function set_time_zone(submit) {
    var set_date = new Date();
    var gmtHours = -set_date.getTimezoneOffset() / 60;
    document.getElementById('timezone').value = gmtHours;
    server = "/TimeZone?timezone=" + gmtHours;
    send_request(submit, server);
}

function set_ssid(submit) {
    server = "/ssid?ssid=" + val('ssid') + "&password=" + encodeURIComponent(val('password'));
    send_request(submit, server);
    alert("Измененя вступят в силу после перезагрузки. Пожалуйта перезагрузите устройство.");
}

function set_ssid_ap(submit) {
    server = "/ssidap?ssidAP=" + val('ssidap') + "&passwordAP=" + encodeURIComponent(val('passwordAp'));
    send_request(submit, server);
    alert("Измененя вступят в силу после перезагрузки. Пожалуйта перезагрузите устройство.");
}

function set_ssdp(submit) {
    server = "/ssdp?ssdp=" + val('ssdp');
    send_request(submit, server);
    document.getElementById('ssdp_t').innerHTML = val('ssdp');
}

// -----------------------------------------------------------------------------------------------
function set_switch(submit, sw, cmd) {
    server = "/Switch?Switch=" + sw + "&cmd=" + cmd;
    document.getElementById('switch_t' + sw).innerHTML = cmd;
    send_request(submit, server);
    return false;
}

// ------------------------------------------------------------------------------------------------
function restart(submit, texts) {
    if (confirm(texts)) {
        server = "/restart?device=ok";
        send_request(submit, server);
        return true;
    } else {
        return false;
    }
}