# Demos
Demos are a way to test the CanSat-container code for different devices. They are not meant to be used in the actual CanSat control system. If you want to create a new demo, you can use the template in the `demo-template` folder and copy the src folder and platformio.ini file to a new folder. Then you can start editing the code.


## Running and Uploading Demos
To run the compile the demos run the following while in the desired demo folder:
```bash
platformio run
```

To upload the code to the device run the following:
```bash
platformio run --target upload
```