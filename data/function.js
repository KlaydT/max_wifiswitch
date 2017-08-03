function load() {
    $.ajax("configs.json")
        .done(function(data){
            initSwitches(data['Switches']);
            initForm(data);
            $('#SSDPNameTop').text(data['SSDPName']);
        })

    //Switched handler
    $("#reset").click(function(){
        if (confirm('Вы уверены?')) {
            window.location = "reset";
        } else {
        }
    });

    //Switched handler
    $(".switch_cl").click(function(){
        var swID = $(this).attr("id").charAt(3);
        var switchObj = this;
        console.log("click: " + swID);
        $.ajax({
            method: "post",
            url: "switch",
            data: {sw: swID}
        }).done(function(data){
            console.log(data);
            if (data == "OK") {
                $(switchObj).toggleClass("btn-success btn-danger");
            }
        })
    });

}

function initSwitches(switches){
    for(sw in switches){
        if (switches[sw] == "1"){
            $("#sw-"+sw).addClass("btn-success");
        } else {
            $("#sw-"+sw).addClass("btn-danger");
        }
    }

}

function initForm(data){
    for(setting in data){
        $("input[name='"+setting+"']").val(data[setting]);
    }
}