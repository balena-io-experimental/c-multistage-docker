const util = require("util");
const bleno = require("bleno");

const Descriptor = bleno.Descriptor;
const Characteristic = bleno.Characteristic;

class ColourCharacteristic {
  constructor() {
    Slider.super_.call(this, {
      uuid: "d7e84cb2-ff37-4afc-9ed8-5577aeb8454c",
      properties: ["write"],
      descriptors: [
        new Descriptor({
          uuid: "2901",
          value: "Assign values to json"
        })
      ]
    });
    this.red = 0;
    this.green = 0;
    this.blue = 0;
}
  onSubscribe(maxValueSize, updateValueCallback) {
    console.log(`Subscribed, max value size is ${maxValueSize}`);
    this.updateValueCallback = updateValueCallback;
  }

  onUnsubscribe() {
    console.log("Unsubscribed");
    this.updateValueCallback = null;
  }

  onWriteRequest(data, offset, withoutResponse, callback) {
    if(offset) {
      callback(this.RESULT_ATTR_NOT_LONG);
    } else if(data.length !== 3) {
      callback(this.RESULT_INVALID_ATTRIBUTE_LENGTH);
    } else {
      this.red = data.readUInt8(0);
      this.green = data.readUInt8(1);
      this.blue = data.readUInt8(2);
      callback(this.RESULT_SUCCESS);
    } 
    // let payload = data.readUInt8();
    // console.log(payload);
    // callback(this.RESULT_SUCCESS);
  }
};

util.inherits(ColourCharacteristic, Characteristic);

module.exports = ColourCharacteristic;
