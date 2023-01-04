# RainSavor
The goal of this system is to sample the electrical conductivity of rainfall. Precipitation is collected in a cup with 2 wires for measuring electrical conductivity at the bottom and two wires for measuring water height at the top. The cup is emptied automatically from the bottom once the water reaches a set height by a siphon. The maximum measurable conductivity value and resolution are set with an I2C AD5246 variable resistor. Temperature and relative humidity are measured with an SHT31 sensor. All data is timestamped and logged to a microSD card at a set time interval with the device on standy between measurements.