var util = require("util")

var bleno = require("bleno")
var BlenoPrimaryService = bleno.PrimaryService;

var sliders = require("../characteristics/colour-characteristics");

function DataHandlerService() {
    DataHandlerService.super_.call(this, {
        uuid: "fff0",
        characteristics: [new colour-characteristics()]
    });
}

utils.inherits(DataHandlerService, BlenoPrimaryService);

module.exports = DataHandlerService;