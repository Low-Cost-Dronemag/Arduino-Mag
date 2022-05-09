# Intro
This GitHub page details a project done by Brett Bernstein and Cameron Modisett as part of the completion of GPGN590 at the Colorado School of Mines. Herein we discuss the creation of a low-cost, drone-based magnetometer using Arduino microcontrollers. May this page be a reference to those who come after.


# Why dronemag?
Drones are becoming increasingly prevalent in the geophysics industry. Equipment can be mounted on a drone to collect data similar to those of a ground survey, but the drone is more efficient than a human operator, and drones can fly surveys in areas where human traversal is impossible. Magnetometers are a prime candidate to mount on a drone because they record the Earth's passive magnetic field without the need to create a source field. From locating buried orebodies to underground utilities and pipelines, magnetics can help pinpoint the location and depth of objects that are important to everyday life. As is common in the geophysics industry, equipment to perform these surveys can cost up to tens-of-thousands of dollars. Two common industry-grade magnetometers include the Geometrics MagArrow (https://baam.tech/design/product/magarrow-uav-enabled-magnetometer/) and the QuSpin total-field magnetometer (https://quspin.com/qtfm/). The MagArrow is a fully-formed magnetic sensor with GPS and IMU onboard, clocking in at $36,000. The QuSpin magnetometer is just the magnetic sensor and still costs $10,000 for a single unit. These prices may be affordable for larger companies and organizations, but are beyond the range of possibility for many smaller organizations that, for example, need to survey a battlefield for unexploded ordnaces (UXOs). We explore a low-cost magnetomer unit using Arduino microcontrollers and compatible sensors, and compare data collected with our design to data collected by the MagArrow.

# Sensor Components 
There are a few primary motivations for choosing Arduino as the brain of our magnetometer. First is the price, an Arduino MEGA 2560 costs only $38.94 but can easily interface with a large number of sensors. Those sensors are typically easily to interface with, code-wise, often requiring little documentation. 

Aside from the Arduino, the magnetometer is the most important component. We use the DRV425EVM magnetometer (https://www.ti.com/tool/DRV425EVM), approximately $24.00. This component outputs a voltage, which can be transformed to true magnetic reading using V_out / (4 * 12.2 * R1), where V_out is the voltage reading and R1 is the resistance of one of the resistors on the sensor. We found R1 to be 200 Ohms, confirmed not by voltmeter but by comparing the output with the known value of the magnetic field in the location. R1 may vary for different versions of this sensor. The output is in mT and can be converted to nT (the standard magnetic field unit) by multiplying by 10^6.

We want our magnetometer to record not just temporally-, but spatially-varying data, so it is important that we have an accurate clock and GPS. Both of these are achieved using the NEO-6M GPS module (https://www.digikey.com/htmldatasheets/production/2757457/0/0/1/neo-6.html), $2.75. From this we acquire the latitude, longitude, and time at which each data point is recorded. 

In order to store the recorded data we need some sort of microSD card interface. We use the HiLetgo W5100 ethernet shield (https://www.amazon.com/Ethernet-Shield-Network-Expansion-Arduino/dp/B00AXVX5D0), $20.99, which has a built-in microSD slot. The provided code stores magnetic and GPS location/date/time values to a microSD card, which can be transferred to a computer for further use using an SD reader.

A power supply is necessary to run the Arduino. We use an 11.1v battery. The battery alone was too strong and caused issues with the Arduino. Tyical resistors would burn up when set between the battery and the Arduino, so we use a 10 Ohm resistor rated for 100 W to ensure proper power is delivered to the Arduino.

# Sensor Wiring

The figure below depicts the wiring diagram. The ethernet shield is plugged into the top of the Arduino, and we use the following connections.

DRV425EVM (with the bottom pin as pin 1) <br />
1 -> 5v <br />
2 -> gnd <br />
3 -> A0 <br />
4 -> not used

NEO-6M <br />
Vcc -> 3.3v <br />
Rx  -> pin 18 <br />
Tx  -> pin 19 <br />
Gnd -> gnd <br />
Note that Tx on the sensor should go to Rx on the Arduino, and Rx on the sensor should go to Tx on the Arduino. Tx<->Rx

Battery <br />
Positive -> Vin <br />
Negative -> gnd

![Circuit diagram](figs/circuit.png?raw=true "Circuit diagram")

# Mounting device on drone

A 3D-printed housing, the first figure below, was created to hold the Arduino and the sensors. The housing was mounted on a drone created by students at the Colorado School of Mines, the second figure below. We used this drone/housing combo to fly a survey and test the sensor against the MagArrow with a commericial drone.

![Arduino and sensor housing](figs/housing.png?raw=true "Arduino and sensor housing")

![Homemade drone](figs/drone.png?raw=true "Homemade drone donated to the Colorado School of Mines Geophysics department")

# Test survey flight

We contucted a survey over an empty field using both our homenmade Arduino magnetometer with the homemade drone, and the MagArrow using a DJI Matrice 300 RTK drone. The first figure below is the arduino-mag data, and the second figure is the production equipment data. Drone flight lines are indicated by the black lines, and two UXO piles are indicated with black dots. We expect to see magnetic highs, or a strong red color near the black dots. The UXO locations were determined using a phone GPS with a error of up to 3 m, so in both cases we are able to see magnetic highs in the expected areas. Our design is not perfect by any means, but these plots begin to show evidence that our low-cost design can hold up against a system that costs $36,000, which opens door for future potential work.

![Arduino Mag](figs/arduinomag.png?raw=true "Data collected with our Arduino magnetometer")
![MagArrow](figs/magarrow.png?raw=true "Data collected with the MagArrow")

# Potential improvements

There are a few aspects that could be greaetly improved with our design. First, the DRV425EVM only measures a single component of the magnetic field vector. It would be possible to add more sensors such that x, y, and z magnetic components are all recorded. Second is an issue with housing. The magnetometer is located in the same box as all the other components used. This is an issue in that most of them have metal, and therefore are sources of magnetic noise. The magnetometer(s) could be removed further using longer jumper cables in order to mitigate the contributions to noise. Third is the addition of an inertial measurement unit (IMU) to the system. The magnetometer reading will change when it is rotated out of alignment with the Earth's magnetic field. An IMU will be able to supply information about the drone's pitch, yaw, and roll--the orientation of the drone--and in turn allow for any alignment deviations to be corrected.
