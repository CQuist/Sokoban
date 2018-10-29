#!/usr/bin/python3

import ev3dev.ev3 as ev3
from motor import *
from time import sleep

lightSensorLeft = ev3.ColorSensor('in2')
lightSensorRight = ev3.ColorSensor('in1')
lightSensorFront = ev3.LightSensor('in4')

debugMotor = ev3.MediumMotor('outB')

## 62-69    White
## 4-7      Black

THRESHOLD = 65
STOP_THRESHOLD = 400


# FEJL MELLEM DE TO SENSOR
##======================================================================

def GoForward():
    done = False
    sensorLeft = lightSensorLeft.value()
    sensorRight = lightSensorRight.value()
    sensorFront = lightSensorFront.value()

    errorR = sensorRight/THRESHOLD
    errorL = sensorLeft/THRESHOLD

    if sensorFront < STOP_THRESHOLD:
        stopMovement()
        done = True
    else:
        moveStreigth('forward', errorL, errorR)

    return done


def GoBackward():
    done = False
    sensorLeft = lightSensorLeft.value()
    sensorRight = lightSensorRight.value()

    errorR = sensorRight//THRESHOLD
    errorL = sensorLeft//THRESHOLD

    if sensorLeft < STOP_THRESHOLD and sensorRight < STOP_THRESHOLD:
        stopMovement()
        done = True
    else:
        moveStreigth('backward', errorL, errorR)

    return done


def TurnRight():
    turnRight()
    return True


def TurnLeft():
    turnLeft()
    return True


def StopMovement():
    stopMovement()
    return True

def Error():
    print("Input not valid")
    sleep(10)

##======================================================================

##======================================================================

state = "idle"

listOfMovements = ["driveForward", "driveForward", "driveForward", "stop"]
index = 0

logfile = open("logfile.txt", "w")
logfile.write("SOKOBAN LOG FILE \n")
logfile.close()

while True:

    if state == "idle":
        state = listOfMovements[index]
        index = index + 1
        logfile = open("logfile.txt", "a")
        logfile.write("NEXT STATE \n")
        logfile.close()
        if index%2 == 0:
            debugMotor.run_to_rel_pos(position_sp=350, speed_sp=500, stop_action="hold")
        else:
            debugMotor.run_to_rel_pos(position_sp=-350, speed_sp=500, stop_action="hold")

        sleep(2)

    elif state == "driveForward":
        done = GoForward()
        if done:
            state = "idle"
    elif state == "driveBackwards":
        done = GoBackward()
        if done:
            state = "idle"
    elif state == "stop":
        done = StopMovement()
        if done:
            state = "idle"
    elif state == "turnRight":
        done = TurnRight()
        if done:
            state = "idle"
    elif state == "turnLeft":
        done = TurnLeft()
        if done:
            state = "idle"
    else:
        Error()
        break

    if index == len(listOfMovements):
        break
