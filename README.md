# LVGL Hello World

A basic LVGL example for (Waveshares ESP32-s3-touch-lcd-1.46 that displays) a button on screen, and when pressed, opens a window with a "Hello World!" message.

## Description

This application demonstrates the use of the LVGL (Light and Versatile Graphics Library) with an SPD2010 display and touch panel. The project implements a simple interface consisting of:

- A central button with the text "Click here"
- A modal window that appears when the button is pressed, showing the "Hello World!" message
- A "Close" button to dismiss the modal window

## Required Hardware

- ESP32 (ESP-IDF compatible)
- SPD2010 display with capacitive touch panel
- TCA9554PWR I/O controller

## Software Requirements

- Arduino IDE
- LVGL library (version 8 or higher)
- Specific drivers included in this repository:
  - SPD2010 (display and touch)
  - TCA9554PWR (I/O expander)
  - I2C_Driver

## Project Structure

- `HolaMundo_LVGL.ino`: Main file with the application code
- `LVGL_Driver.cpp/h`: Interface between LVGL and the specific hardware
- `Display_SPD2010.cpp/h`: LCD display driver
- `Touch_SPD2010.cpp/h`: Touch panel driver
- `TCA9554PWR.cpp/h`: I/O expander driver
- `I2C_Driver.cpp/h`: Driver for I2C communication

## Connections

The project uses the following pins:
- SPI (display): Pins 40, 41, 42, 45, 46, 21
- I2C (touch and I/O expander): Pins 10, 11
- Touch interrupt: Pin 4
- Backlight: Pin 5

## Usage

1. Upload the code to your ESP32
2. The screen will display a button with the text "Click here"
3. When pressing the button, a modal window will appear with the "Hello World!" message
4. Press "Close" to dismiss the window

## Customization

You can customize this example by modifying:
- The button text in the `setup()` function: `lv_label_set_text(label, "Click here");`
- The modal window message in the `btn_event_cb()` function: `msgbox = lv_msgbox_create(NULL, "Message", "Hello World!", btns, false);`
- The screen brightness: `Set_Backlight(50);` (range 0-100)
