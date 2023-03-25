# Demo Creation and Execution
Demos are a way to test the CanSat-container code for different devices. They are not meant to be used in the actual CanSat control system. If you want to create a new demo, you can use the template in the `demo-template` folder and copy the src folder and platformio.ini file to a new folder. Then you can start editing the code.

## Pre-requisites
To run and flash the demos you need to install platformio. You can install it by running the following command:
```bash
pip install -U platformio
```

## Running and Uploading Demos
To run the compile the demos run the following while in the desired demo folder:
```bash
platformio run
```

To upload the code to the device run the following:
```bash
platformio run --target upload
```