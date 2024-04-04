
$(document).ready(function(){
    startDHTSensorInterval();
});

//Function to change RGB color from webpage
function updateColor(){
    var color = document.getElementById("color_picker").value;
    var red = parseInt(color.substring(1, 3), 16);
    var green = parseInt(color.substring(3, 5), 16);
    var blue = parseInt(color.substring(5, 7), 16);
    var msg = red + "," + green + "," + blue
    var request = new XMLHttpRequest(); 
    request.open("POST","/RGBchange");
    request.responseType = "text";
    request.send(msg);
    console.log("Color changed to: " + msg)
}

//Function to get the DHT22 sensor temperature and humidity values
function getDHTSensorValues(){
    $.getJSON('/dhtSensor.json', function(data){
        $("#temperature_reading").text(data["temp"]);
        $("#humidity_reading").text(data["humidity"]);
    })
}

//Sets the interval for getting the update DHT22 sensor values
function startDHTSensorInterval(){
    setInterval(getDHTSensorValues, 5000);
}