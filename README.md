# Firebase IoT Demo with ESP8266

This project uses ESP8266 to read sensors, connect to Firebase and push data to Realtime Database. Temperature and Humidity data is read from DHT11 sensor, Gas leak detection is read from MQ5 Gas sensor finally device heartbeat is sent every 2 secondes to Firebase to ensure device cinnection.
Note that you can simulate the sensors with random values by uncommenting 'DONT_HAVE_SENSORS' define.

## Getting Started

These instructions will get you a copy of the project up and running on your ESP8266 for development and testing purposes.

### Prerequisites

This project uses the Arduino framework and PlatformIO IDE for building and flashing the app into ESP8266. The data sent from the device is stored in Firebase Realtime Database.

* Setup [PlatformIO](https://platformio.org/install/ide?install=vscode) extension with VSCode
* Setup a new [Firebase](https://console.firebase.google.com/) project in Firebase console

### Installing

Once installed PlatformIO and created firebase project, download this project

```bash
git clone https://github.com/kaizoku-619/firebase-iot-demo.git
```

#### Setup credentials

```bash
cd firebase-iot-demo/include
```

Inside 'include' directory create a new file named 'Creds.h' and replace with the values of your WiFi SSID, Password and Firebase host of your Firebase project and Firebase Auth which you can find them in this link by replacing [###########] with your Firebase project name. https://console.firebase.google.com/project/[###########]/settings/serviceaccounts/databasesecrets

```C
#define SSID "XXXXXXXXXXX"
#define PASSWORD "XXXXXXXXXXXXXXX"

#define FIREBASE_HOST "XXXXXXXXXXXXX.firebaseio.com"
#define FIREBASE_AUTH "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
```

#### Simulate Sensors

If you don't have sensors attached to your ESP8266 or you want to quickly test the application and consult data in Firebase database without the need of attaching sensors, you can simply uncomment DONT_HAVE_SENSORS define.

```C
#define DONT_HAVE_SENSORS
```

#### Update Firebase Fingerprint

Build the project using PlatformIO, Once project built you'll see .piolibdeps folder created with the 3 new libraries installed:

* DHT sensor library
* Adafruit Unified Sensor
* FirebaseArduino

Open FirebaseArduino/src/FirebaseHttpClient.h and scroll to the end of the page, you'll find FirebaseFingerprint[] which its value need to be changed.

To generate a fingerprint, go to this [website](https://www.grc.com/fingerprints.htm) and copy your Firebase host link without the https part (your might look something like this your-prject-name.firebaseio.com) and click on Fingerprint Site.
Copy the newly generated fingerprint in the place of the FirebaseFingerprint[] value and replace the columns with spaces.

Upload the code and you're good to go.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## References

* [Fingerprints](https://www.grc.com/fingerprints.htm)
* [PlatformIO](https://platformio.org/platformio-ide)
