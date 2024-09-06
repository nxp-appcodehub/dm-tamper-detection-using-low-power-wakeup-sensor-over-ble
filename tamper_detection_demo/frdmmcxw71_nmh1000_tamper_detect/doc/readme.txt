Overview
========
This example demo describes how to configure low-power motion wake-up accelerometer for tamper/theft detection and transmit ALERT to connected end user via BLE wireless UART.
This example use FRDM-MCXW71 and on-board FXLS8974CF accelerometer to autonomously detect tampering/theft/abuse on device using low-power motion wakeup feature and trasmit
ALERT message via BLE wireless UART on NXP IoT Toolbox mobile App.

Toolchain supported
===================
- IAR embedded Workbench (ide version details are in the Release Notes)
- MCUXpresso IDE (ide version details are in the Release Notes)

Hardware requirements
=====================
- Mini/micro USB cable
- frdmmcxw71 board

Software requirements
=====================
- NXP IoT ToolBox App - Android Store or NXP IoT ToolBox App - Apple Store

Board settings
==============
- Flash your FRDM-MCXW71 with its corresponding NBU for wireless examples.
- The NBU file is available as part of downloaded FRDM-MCXW71 SDK under "SDK_2_16_000_FRDM-MCXW71\middleware\wireless\ble_controller\bin" folder.
- Information regarding flashing NBU can be found on the getting started page for FRDM-MCXW71.

Prepare the Demo
================
- Connect a USB-C cable between the host PC and the MCU-Link USB port on the target FRDM-MCXW71 board.
- Right click on the project and select "Debug As". Select MCUXpresso IDE LinkServer (inc. CMSIS DAP) probes.
- Debugger will start downloading the program to device. MCU-LINK probes will be identified. Click “OK” to continue.
- Click on “Resume” button or press “F8” key on your keyboard to continue running the downloaded program on device.
- Click on “Terminate” button or press “CNTR + F2” to terminate the debug session.
- Disconnect the USB-C cable connected to FRDM-MCXW71 and reconnect.
- To enable your board to start its BLE advertising, press and release button SW3.

Running the demo
================
- Open the installed NXP IoT Toolbox (available from Play Store and App Store) on your mobile phone. Make sure bluetooth connection is ON.
- Inside the application, click on the Wireless UART widget. Scan should start automatically.
- When a device called NXP_WU appears, click on it. Your phone should now be connecting to the board.
- After connecting with the FRDM-MCXW71 device, the serial UART terminal will show this message:

 Wireless UART starting as GAP Peripheral.
 Advertising...
 Connected to device 0 as peripheral.

- With no tampering/motion shown on the FRDM-MCXW71 board, the Wireless UART application on IoT ToolBox mobile app will show status as: "Your Asset is Safe"
- When you show tampering/movement on the FRDM-MCXW71, the on-board FXLS8974CF accelerometer detects the motion and wakeup.
- At that point, you will see Wireless UART app showing ALERT message as
  
  Tampering Detected on your Asset.

- FRDM-MCXW71 board will also show "RED" LED status. The "RED" LED status will continously remain ON till tampering/motion detected.
- If there is no further tampering/motion detected for continous ~5 sec, the on-board FXLS8974CF accelerometer will detect no-motion and update the status message on wirless UART app. The "RED" LED status on FRDM-MCXW71 board will also go OFF.



