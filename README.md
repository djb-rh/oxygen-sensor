# oxygen-sensor

This is a Particle-specific project to make an O2 percentage sensor.  It also does some math to convert the oxygen percentage to an estimate of altitude. Note that no temperature or barometric pressure changes due to weather are taken into account, so this is a ROUGH ESTIMATE of altitude. It's close enough for relative measurements needed for hypoxic training, which is what I will be using it for.

Docs for the LCD I used are here:

http://wiki.sunfounder.cc/index.php?title=I2C_LCD2004
and I bought it on Amazon.  Search for Sunfounder LCD. It's a basic 4x20 but has an I2C module added.  The library for it is in the Particle ecosystem already, so you just add the LiquidCrystal_I2C_Spark library for this piece.

I also use the Adafruit ADS1015 12 bit ADC, which is also I2C. Again, add the Adafruit_ADS1015 library, already in the Particle ecosystem. The sensor I used is this one:

https://www.oxycheq.com/analyzers-sensors/r17s-oxygen-sensor.html

I also use the clickButton library (again, already in the Particle ecosystem) for my button debouncing.

I used the code and wiring from this project as my starting point:

https://www.instructables.com/id/Easy-To-Build-Oxygen-Analyzer-Using-An-Arduino-Com/

That one is fine, but this one is one I found later that looks a little better for you scuba divers out there:

https://ejlabs.net/arduino-oled-nitrox-analyzer/

(Better display for one, and more complete code. Pretty sure he started with the code from the previous guy, too.)

Now, why didn't I just use one of those two?  For starters, I already had the display that I'm using. But the main reason I didn't just port one of those to my display is I'm not using this for scuba diving, and both these seem to assume you're starting at sea level.  I wanted to understand the code a little better, so I used that first one as a guide and tried to re-write it as I went.  Also the Particle is a tad different than the Arduino.  And unless folks want to submit changes, this code probably won't ever been clean and complete.  I built and am testing this code as a standalone unit, but I have no need for a standalone unit ultimately, so once this code is close, I'll be yanking the important bits out and stuffing them into my pid-controller code.  

My ultimate goal is to have an air conditioner unit in my hypoxic training tent that controls the temperature but also tells me the effective altitude based on the oxygen content inside the tent. My AC controller uses this display already, too, so I'm used to coding for it.

As for wiring, I don't have a diagram. Put a momentary switch connected to D2 on one side and GND on the other.  Put the ADC and the display on the I2C bus.  Then connect the end of the 1/8" mono jack to GND on the ADC and the inboard connection of the 1/8" jack to the A0 port on the ADC. That should be all you need to make this particular sensor work with this ADC and display. I also used the NCD.io Particle breakout board, available here:

https://store.ncd.io/product/screw-terminal-breakout-board-for-particle-photon/

You don't need the above as pretty much any Particle breakout board will do. These are well made, easy, and cheap though. Also note that this board has a specific style of I2C connector used by NCD devices. Their I2C sensors are good, but often pretty expensive compared to similar products from Adafruit.  For example, NCD sells an SHT-31 temp/hum sensor for $33 and Adafruit has a similar product for $13.  The NCD one has both an in and out I2C connector that matches the ones on their boards, which is super nice and convenient, but if you don't care or need that, go with the cheaper option. Also note that NCD sells those connectors they use for I2C way cheaper than even Digikey does, so if you plan to use that standard but want to make your own connections, buy your connectors from NCD!  They also have a fairly decent price on premade cables in different lengths for the I2C bus, too.


## Welcome to your project!

Every new Particle project is composed of 3 important elements that you'll see have been created in your project directory for oxygen-sensor.

#### ```/src``` folder:  
This is the source folder that contains the firmware files for your project. It should *not* be renamed. 
Anything that is in this folder when you compile your project will be sent to our compile service and compiled into a firmware binary for the Particle device that you have targeted.

If your application contains multiple files, they should all be included in the `src` folder. If your firmware depends on Particle libraries, those dependencies are specified in the `project.properties` file referenced below.

#### ```.ino``` file:
This file is the firmware that will run as the primary application on your Particle device. It contains a `setup()` and `loop()` function, and can be written in Wiring or C/C++. For more information about using the Particle firmware API to create firmware for your Particle device, refer to the [Firmware Reference](https://docs.particle.io/reference/firmware/) section of the Particle documentation.

#### ```project.properties``` file:  
This is the file that specifies the name and version number of the libraries that your project depends on. Dependencies are added automatically to your `project.properties` file when you add a library to a project using the `particle library add` command in the CLI or add a library in the Desktop IDE.

## Adding additional files to your project

#### Projects with multiple sources
If you would like add additional files to your application, they should be added to the `/src` folder. All files in the `/src` folder will be sent to the Particle Cloud to produce a compiled binary.

#### Projects with external libraries
If your project includes a library that has not been registered in the Particle libraries system, you should create a new folder named `/lib/<libraryname>/src` under `/<project dir>` and add the `.h`, `.cpp` & `library.properties` files for your library there. Read the [Firmware Libraries guide](https://docs.particle.io/guide/tools-and-features/libraries/) for more details on how to develop libraries. Note that all contents of the `/lib` folder and subfolders will also be sent to the Cloud for compilation.

## Compiling your project

When you're ready to compile your project, make sure you have the correct Particle device target selected and run `particle compile <platform>` in the CLI or click the Compile button in the Desktop IDE. The following files in your project folder will be sent to the compile service:

- Everything in the `/src` folder, including your `.ino` application file
- The `project.properties` file for your project
- Any libraries stored under `lib/<libraryname>/src`
