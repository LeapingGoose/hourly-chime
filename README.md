# ESP32 Hourly Random Chime with LED Indicator

This project is an ESP32-based hourly random chime with an LED indicator. The chime sounds at random intervals within each hour, and the LED blinks every second to indicate the device is running.

## Features

- Hourly chime with random intervals. Chimes will not sound with 15 minutes of eachother and the random interval is weighted.
- LED indicator that blinks every second
- Configurable debug mode for testing

## Hardware Requirements

- ESP32 development board
- Buzzer connected to pin 21

## Software Requirements

- PlatformIO
- Arduino framework

## Installation

1. Clone this repository:
   ```sh
   git clone https://github.com/yourusername/esp32-hourly-random-chime.git
   ```
2. Open the project with PlatformIO in Visual Studio Code.

3. Build and upload the project to your ESP32 board.

## Configuration

You can configure the project by modifying the `platformio.ini` file and the `src/main.cpp` file.

### Debug Mode

To enable debug mode, uncomment the `#define DEBUG` line in `src/main.cpp`. This will set the interval to 60 seconds and the minimum distance to 15 seconds for testing purposes.

## Usage

Once the project is uploaded to the ESP32 board, it will start running automatically. The LED will blink every second, and the buzzer will chime at random intervals within each hour.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgements

- [PlatformIO](https://platformio.org/)
- [Arduino](https://www.arduino.cc/)
