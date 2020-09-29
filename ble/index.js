const bleno = require("bleno");
const DeviceInfoService = require("./utils/services/device-info-service");
const DataHandlerService = require("./utils/services/data-handler-service");
const deviceInfoService = new DeviceInfoService();
const DataHandlerService = new DataHandlerService();

//JSON STUFF
const fs = require('fs')
const path = '/data/data.json'

var data = {
  "speed" : 1,
  "scale" : 1,
  "red" : 100,
  "blue" : 100,
  "green" : 100,
  "brightness" : 50
};

fs.access(path, fs.F_OK, (err) => {
  if (err) {
    var json_data = JSON.stringify(data);
    fs.writeFileSync('/data/data.json', json_data)
    console.log("File written");
    return
  }
  //file exists
  data = fs.readFileSync(path);
  data = JSON.parse(data);
  console.log(data);
});

function setJSONvalues() = function(r, g, b) {
  
};

bleno.on("stateChange", state => {
  console.log(`on -> stateChange: ${state}`);

  if (state === "poweredOn") {
    bleno.startAdvertising("balenaBLE", [
      deviceInfoService.uuid,
      DataHandlerService.uuid
    ]);
  } else {
    bleno.stopAdvertising();
  }
});

bleno.on("advertisingStart", error => {
  console.log(
    `on -> advertisingStart: ${error ? "error " + error : "success"}`
  );

  if (!error) {
    bleno.setServices([deviceInfoService, DataHandlerService], error => {
      console.log(`setServices: ${error ? "error " + error : "success"}`);
    });
  }
});

module.exports.setJSONvalues = setJSONValues;
