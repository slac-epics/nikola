from datetime import datetime
import time
import random

ambient_temp = 29

# in oC
temp_precision_when_on   = 0.2
temp_precision_when_off  = 1

# in seconds
time_for_restart         = 10

# in oc/min
temp_rate_from_off_to_on = -5
temp_rate_from_on_to_off = 8

# For this test server we are considering that the chiller will never get to an
# alarm or warning status, always in cooling mode, and remote. This is
# represented with all bits zero, except for the second one, representing
# remote control and the third one representing chiller ready. Bit B0 is the only
# one we will control.
# Fault bits will always return 0 because this test server will never fail.
STAT_BASE_WORD   = 0x6
STAT_RUNNING_BIT = 0x1

class NikolaController:
    def __init__(self):
        # Chiller starts at ambient temperature, and off
        self.last_temp = ambient_temp
        # Setpoint set by user
        self.temp_setpoint = 0
        # Last time a temperature was read
        self.last_time_reading = datetime.now()
        self.running = False
        # Last time when the system was turned on/off
        self.last_time_on = 0
        self.last_time_off = datetime.now()

    def read_temperature(self):
        # Seconds since last temperature reading
        time_diff_reading = datetime.now() - self.last_time_reading
        time_diff_reading = time_diff_reading.total_seconds()

        if self.running:
            temp_rate = temp_rate_from_off_to_on
            temp_target = self.temp_setpoint
        else:
            temp_rate = temp_rate_from_on_to_off
            temp_target = ambient_temp

        new_temp = self.last_temp + (temp_rate/60) * time_diff_reading

        if self.running:
            # Check if the new calculated temperature overshoot the allowed margin.
            # If not, we assume the temperature is still slowly reaching the setpoint.
            # If yes, this means we are already at the setpoint and need just a random
            # adjustment to make the output more interesting.
            if new_temp < self.temp_setpoint - temp_precision_when_on:
               new_temp = random.gauss(self.temp_setpoint, temp_precision_when_on)
        else:
            # Likewise, if we overshoot the ambient temperature, we were already at it.
            # Time for a random adjustment to make the output vary in a cool way.
            if new_temp > ambient_temp + temp_precision_when_off:
               new_temp = random.gauss(ambient_temp, temp_precision_when_off)

        self.last_temp = new_temp
        self.last_time_reading = datetime.now()
        return new_temp

    def set_temperature(self, target):
        self.temp_setpoint = target

    def turn_on(self):
        self.running = True
        self.last_time_on = datetime.now()

    def turn_off(self):
        self.running = False
        self.last_time_off = datetime.now()
