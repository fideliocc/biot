# BIoT
Tracking temperature, air humidity and soil moisture on cloud through and Arduino Yún design and Ubidots dashboard. This was a final project and is very useful to understand the basics of Internet of Things and Cloud services. Watch a demo here: [https://youtu.be/ICUrWtavMwg](https://youtu.be/ICUrWtavMwg)

**Release:** 1.0.0

**Status:** Done!

## **Built with**
- Arduino Yún
- Ubidots for Education ([http://education.ubidots.com](http://education.ubidots.com))

## **Sensors and devices**
- DHT22 temperature/humidity sensor
- Grove Seeed Moisture sensor
- LDR
- a simple potentiometer
- 16x2 LCD display
- microSD (optional)

**Note:** You can easily upgrade these sensors for accurate readings. I have good experience with:
- SHT31 for temperature/humidity
- TSL2591 for light measurement
- Watermark 200SS is a recognized moisture sensor  that you can choose for their features and cost, but you must consider some other parameters in order to obtain accurate readings. You can read the oficial documentation and take first steps on it.

## **Libraries**
Please download and add these libraries to your local directory. You can search for more info about:
- **UbidotsYUN:** Ubidots library for Yún, very easy ([https://github.com/ubidots/ubidots-arduino-yun](https://github.com/ubidots/ubidots-arduino-yun))
- **DHT:** DHT22 sensor library ([https://github.com/adafruit/DHT-sensor-library](https://github.com/adafruit/DHT-sensor-library))
- **LiquidCrystal:** For LCD display handling ([https://github.com/arduino-libraries/LiquidCrystal](https://github.com/arduino-libraries/LiquidCrystal))
- **FileIO:** for SD Card usage
- **Process:** base class for all Bridge based calls

## **Installation**
Download and install the Arduino IDE to work with Arduino hardware. Visit 
First, you need to build your circuit following the diagram below:

![Image](https://drive.google.com/uc?export=view&id=1FKw46MOeLCnN28WIfQYvlIxZTrtu5kKt)

Then you can edit, compile and upload the *biot.ino* sketch to your Arduino Yún vía USB. Remember that you have to replace the *TOKEN* constant with your unique Ubidots Token. Your Yún has to be correctly configured to work on a near WiFi network. If you need some help with this can visit the oficial documentation, or email me :)

Finally, create your devices, variables and dashboard to visualize your data. Visit the demo to have a better idea to your future dashboard. 

For a better understanding (and fun!) the LCD shows sensors values changing in time beside cool icons. If you are interested on make your own, visit [https://maxpromer.github.io/LCD-Character-Creator/](https://maxpromer.github.io/LCD-Character-Creator/) 




## **Author**
Erick Calle Castillo | ecalle17@gmail.com

If you have any question or suggestion feel free to email me.


