var set_real_time;

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
function set_switch1_on(submit) {
    server = "/Switch1?Switch1=on";
    document.getElementById('switch_t1').innerHTML = "on";
    send_request(submit, server);
}

function set_switch1_off(submit) {
    server = "/Switch1?Switch1=off";
    document.getElementById('switch_t1').innerHTML = "off";
    send_request(submit, server);
}

function set_switch2_on(submit) {
    server = "/Switch2?Switch2=on";
    document.getElementById('switch_t2').innerHTML = "on";
    send_request(submit, server);
}

function set_switch2_off(submit) {
    server = "/Switch2?Switch2=off";
    document.getElementById('switch_t2').innerHTML = "off";
    send_request(submit, server);
}

function set_switch3_on(submit) {
    server = "/Switch3?Switch3=on";
    document.getElementById('switch_t3').innerHTML = "on";
    send_request(submit, server);
}

function set_switch3_off(submit) {
    server = "/Switch3?Switch3=off";
    document.getElementById('switch_t3').innerHTML = "off";
    send_request(submit, server);
}

function set_switch4_on(submit) {
    server = "/Switch4?Switch4=on";
    document.getElementById('switch_t4').innerHTML = "on";
    send_request(submit, server);
}

function set_switch4_off(submit) {
    server = "/Switch4?Switch4=off";
    document.getElementById('switch_t4').innerHTML = "off";
    send_request(submit, server);
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