# SSD1306-OLED-emulator-with-SDL2

I2C protocol and some OLED details in protocol.txt

Compile in root directory

```
make
```

How to send I2C bitstream:

Sample (u8 = uint8_t)
```cpp
vector<u8> bitstream = {1,0,0,1,0,....0,1,0,0}
run(plc, oled1, bitstream);
```

How to convert from readible hex to binary bitstream:

Sample
```cpp
vector<u8> stream1 = convert(0x3C, 0, 0x80, 0xB1);
```

See ```sample2()``` function in main.cpp for sample series of commands to draw a certain image
