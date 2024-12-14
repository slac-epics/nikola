from datetime import datetime
import time
import random
import sys
from os.path import dirname
sys.path.append(dirname(__file__))
import mockNikolaController

cont_tst = mockNikolaController.NikolaController()

print("Controller off")
for iii in range (5):
    print(str(iii) + " - " + str(cont_tst.read_temperature()))
    time.sleep(2)

print()
print("Controller on, setpoint 7 oC")
cont_tst.set_temperature(7)
cont_tst.turn_on()

for iii in range (30):
    print(str(iii) + " - " + str(cont_tst.read_temperature()))
    time.sleep(10)

print()
print("Controller off")
cont_tst.turn_off()

for iii in range (20):
    print(str(iii) + " - " + str(cont_tst.read_temperature()))
    time.sleep(10)
