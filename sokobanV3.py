#!/usr/bin/python3

import ev3dev.ev3 as ev3
from motor import *
from time import sleep

lightSensorLeft = ev3.ColorSensor('in2')
lightSensorRight = ev3.ColorSensor('in1')
lightSensorFront = ev3.LightSensor('in4')
touchSensor = ev3.TouchSensor('in3')

## 62-69    White
## 4-7      Black



##======================================================================
# GLOBAL VARIABLES

INIT_STEPS = 20

##======================================================================

def initializeSensors():

    frontValue = 0
    value = 0

    for i in range(INIT_STEPS):
        sensorLeft = lightSensorLeft.value()
        sensorRight = lightSensorRight.value()
        sensorFront = lightSensorFront.value()

        value = value + sensorLeft + sensorRight
        frontValue = frontValue + sensorFront


    value = (value / (INIT_STEPS * 2))
    frontValue = (frontValue / INIT_STEPS) * 0.9
    return (value, frontValue)


def GoForward():
    done = False
    sensorLeft = lightSensorLeft.value()
    sensorRight = lightSensorRight.value()
    sensorFront = lightSensorFront.value()

    errorR = sensorRight/THRESHOLD
    errorL = sensorLeft/THRESHOLD

    if sensorFront < STOP_THRESHOLD:
        moveReal('forward', 250)
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

def TurnRel(direction):
    turnRel(direction, 70)

def CenterLine():
    done = False
    sensorLeft = lightSensorLeft.value()
    sensorRight = lightSensorRight.value()

    if abs(sensorLeft - sensorRight) < 5:
        stopMovement()
        done = True
    elif sensorLeft > sensorRight:
        turn("right")
    elif sensorRight > sensorLeft:
        turn("left")

    return done

def Turn(direction):
    done = False
    sensorLeft = lightSensorLeft.value()
    sensorRight = lightSensorRight.value()

    if direction == "right":
        turn(direction)
        if sensorLeft - sensorRight > THRESHOLD*0.75:
            stopMovement()
            done = True
    elif direction == "left":
        turn(direction)
        if sensorRight - sensorLeft > THRESHOLD*0.75:
            stopMovement()
            done = True

    return done

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


runForever = True

THRESHOLD, STOP_THRESHOLD = initializeSensors()

#listOfMovements = ["driveForward"]
#listOfMovements = ["driveForward", "turnLeft", "wait", "turnLeft", "driveForward"]
#listOfMovements = ["turnLeft", "wait", "turnLeft", "wait", "turnLeft", "wait", "turnLeft", "wait"]
#listOfMovements = ["driveForward", "driveForward", "driveForward", "driveForward", "turnLeft", "turnLeft", "driveForward", "driveForward", "driveForward", "driveForward", "turnRight", "turnRight"]
#listOfMovements = ["driveForward", "turnLeft", "driveForward", "turnRight", "driveForward", "turnRight", "driveForward", "turnRight", "driveForward", "turnRight","driveForward", "turnLeft", "driveForward", "turnLeft", "driveForward", "turnLeft"]
listOfMovements = ["driveForward", "driveForward", "driveForward", "driveForward", "turnLeft"]

state = "idle"
index = -1
turnState = "initial"
counter = 0


logfile = open("logfile.txt", "w")
logfile.write("SOKOBAN LOG FILE \n")
logfile.write("Number of Moves: " + str(len(listOfMovements)) + "\n")
logfile.write("Threshold: " + str(THRESHOLD) + " Stop Threshold: " + str(STOP_THRESHOLD) + "\n")
logfile.close()

while len(listOfMovements) >= index and not touchSensor.value():

    if index >= 0:
        pass
        #logfile = open("logfile.txt", "a")
        #logfile.write("index: " + str(index) + " movement: " + listOfMovements[index] + "\n")
        #logfile.close()


    if state == "idle":

        #logfile = open("logfile.txt", "a")
        #logfile.write("Number of iterations used: " + str(counter) + " for, index: " + str(index) + " movement: " + listOfMovements[index] + "\n")

        index = index + 1
        if len(listOfMovements) > index:
            state = listOfMovements[index]

        #logfile.write("Length: " + str(len(listOfMovements)) + "   index: " + str(index) + " movement: " + listOfMovements[index] + " TAKEN!!" + "\n")
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
        if turnState == "initial":
            turnDone = Turn("right")
            if turnDone:
                turnState = "primary"
        elif turnState == "primary":
            done = CenterLine()
            if done:
                state = "idle"
                turnState = "initial"
    elif state == "turnLeft":
        if turnState == "initial":
            turnDone = Turn("left")
            if turnDone:
                turnState = "primary"
        elif turnState == "primary":
            done = CenterLine()
            if done:
                state = "idle"
                turnState = "initial"
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


    if runForever == True:
        if index == len(listOfMovements)-1:
            index = -1

    counter += 1