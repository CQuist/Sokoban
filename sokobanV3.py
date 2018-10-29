#!/usr/bin/python3

import ev3dev.ev3 as ev3
from motor import *
from time import sleep

lightSensorLeft = ev3.ColorSensor('in2')
lightSensorRight = ev3.ColorSensor('in1')
lightSensorFront = ev3.LightSensor('in4')

## 62-69    White
## 4-7      Black

THRESHOLD = 65
STOP_THRESHOLD = 300

##======================================================================

def GoForward():
    done = False
    sensorLeft = lightSensorLeft.value()
    sensorRight = lightSensorRight.value()
    sensorFront = lightSensorFront.value()

    errorR = sensorRight/THRESHOLD
    errorL = sensorLeft/THRESHOLD

    if sensorFront < STOP_THRESHOLD:
        moveReal('forward', 100)
        stopMovement()
        done = True
    else:
        moveStraigth('forward', errorL, errorR)

    return done


def GoBackward():
    done = False
    sensorLeft = lightSensorLeft.value()
    sensorRight = lightSensorRight.value()

    errorR = sensorRight//THRESHOLD
    errorL = sensorLeft//THRESHOLD

    if sensorLeft < STOP_THRESHOLD and sensorRight < STOP_THRESHOLD:
        moveReal('forward', 115)
        stopMovement()
        done = True
    else:
        moveStraigth('backward', errorL, errorR)

    return done

def Turn(direction):
    done = turnRel(direction)
    if done == True:
        stopMovement()
        return done
    else:
        return done

#def Turn(direction, step):
#    done = False
#    sensorLeft = lightSensorLeft.value()
#    sensorRight = lightSensorRight.value()
#
#    errorR = sensorRight/THRESHOLD
#    errorL = sensorLeft/THRESHOLD
#    error_thresh = 0.6
#
#    if direction == "right":
#        if step == 1:
#            turnRel(direction)
#            step = 2
#        elif step == 2:
#            if errorL < error_thresh:
#                stopMovement()
#                done = True
#            else:
#                turn(direction)
#    elif direction == "left":
#        if step == 1:
#            turnRel(direction)
#            step = 2
#        elif step == 2:
#            if errorR < error_thresh:
#                stopMovement()
#                done = True
#            else:
#                turn(direction)
#
#    return (done, step)

def deliverCan():
    done = False
    sensorLeft = lightSensorLeft.value()
    sensorRight = lightSensorRight.value()
    sensorFront = lightSensorFront.value()

    errorR = sensorRight/THRESHOLD
    errorL = sensorLeft/THRESHOLD

    if sensorFront < STOP_THRESHOLD:
        moveReal('backward', 360)
        stopMovement()
        done = True
    else:
        moveStraigth('forward', errorL, errorR)

    return done


def StopMovement():
    stopMovement()
    return True

def Error():
    print("Input not valid")
    sleep(10)

##======================================================================

##======================================================================

state = "idle"

#listOfMovements = ["driveForward"]
#listOfMovements = ["turnLeft"]
#listOfMovements = ["driveForward", "wait", "turnLeft", "wait", "driveForward", "wait", "turnLeft", "wait", "driveForward"]
listOfMovements = ["driveForward", "turnLeft", "driveForward", "turnLeft", "driveForward"]
#listOfMovements = ["driveForward", "driveForward", "driveForward", "stop"]
#listOfMovements = ["turnLeft", "wait", "turnRight", "wait", "turnLeft", "wait", "turnRight"]
#listOfMovements = ["driveForward", "turnRight", "driveForward", "turnLeft", "driveForward", "stop"]
#listOfMovements = ["driveForward", "driveForward", "turnLeft", "driveForward", "driveForward", "turnLeft", "driveForward", "driveForward", "turnLeft", "driveForward", "driveForward", "turnLeft"]
#listOfMovements = ["turnRight", "deliverCan", "turnLeft", "driveForward"]
index = -1
turn_step = 1


logfile = open("logfile.txt", "w")
logfile.write("SOKOBAN LOG FILE \n")
logfile.write("Number of Moves: " + str(len(listOfMovements)) + "\n")
logfile.close()

while len(listOfMovements) >= index:

    if index >= 0:
        logfile = open("logfile.txt", "a")
        logfile.write("index: " + str(index) + " movement: " + listOfMovements[index] + "\n")
        logfile.close()


    if state == "idle":
        index = index + 1
        if len(listOfMovements) > index:
            state = listOfMovements[index]

        logfile = open("logfile.txt", "a")
        logfile.write("index: " + str(index) + " movement: " + listOfMovements[index] + " TAKEN!!" + "\n")
        logfile.close()
        #logfile = open("logfile.txt", "a")
        #logfile.write("NEXT STATE \n")
        #logfile.close()
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
        done = Turn('right')
        if done:
            state = "idle"
    elif state == "turnLeft":
        done = Turn("left")
        if done:
            state = "idle"
    elif state == "deliverCan":
        done = deliverCan()
        if done:
            state = "driveBackwards"
    elif state == "wait":
        sleep(3)
        state = "idle"
    else:
        Error()
        break
