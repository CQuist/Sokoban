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
    valueL = 0
    valueR = 0

    for i in range(INIT_STEPS):
        sensorLeft = lightSensorLeft.value()
        sensorRight = lightSensorRight.value()
        sensorFront = lightSensorFront.value()

        valueL = valueL + sensorLeft
        valueR = valueR +  + sensorRight
        frontValue = frontValue + sensorFront


    valueL = (valueL / INIT_STEPS)
    valueR = (valueR / INIT_STEPS)
    frontValue = (frontValue / INIT_STEPS) * 0.9
    return (valueL, valueR, frontValue)


def GoForward():
    done = False
    sensorLeft = lightSensorLeft.value()
    sensorRight = lightSensorRight.value()
    sensorFront = lightSensorFront.value()

    errorR = sensorRight/THRESHOLD_R
    errorL = sensorLeft/THRESHOLD_L

    if sensorFront < STOP_THRESHOLD:
        moveReal('forward', 250)
        #stopMovement()
        done = True
    else:
        moveStraigth('forward', errorL, errorR)

    return done


def GoBackward():
    done = False
    sensorLeft = lightSensorLeft.value()
    sensorRight = lightSensorRight.value()

    errorR = sensorRight//THRESHOLD_R
    errorL = sensorLeft//THRESHOLD_L

    if sensorLeft < STOP_THRESHOLD and sensorRight < STOP_THRESHOLD:
        moveReal('forward', 115)
        stopMovement()
        done = True
    else:
        moveStraigth('backward', errorL, errorR)

    return done

def TurnRel(direction):
    turnRel(direction, 220)
    return True

def CenterLine():
    done = False
    sensorLeft = lightSensorLeft.value()
    sensorRight = lightSensorRight.value()

    if abs(sensorRight - sensorLeft) < 15:
        stopMovement()
        done = True
    elif sensorLeft > sensorRight:
        turn("right", 100)              # 200 worked
    elif sensorRight > sensorLeft:
        turn("left", 100)

    return done

def Turn(direction):
    done = False
    sensorRight = lightSensorRight.value()
    sensorLeft = lightSensorLeft.value()

    if direction == "right":
        turn(direction)
        if sensorLeft - sensorRight > THRESHOLD_L*0.75:
            #stopMovement()
            done = True
    elif direction == "left":
        turn(direction)
        if sensorRight - sensorLeft > THRESHOLD_R*0.75:
            #stopMovement()
            done = True

    return done

def deliverCan():
    done = False
    sensorLeft = lightSensorLeft.value()
    sensorRight = lightSensorRight.value()
    sensorFront = lightSensorFront.value()

    errorR = sensorRight/THRESHOLD_R
    errorL = sensorLeft/THRESHOLD_L

    if sensorFront < STOP_THRESHOLD:
        moveReal('backward', 330)
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
    ev3.Sound.beep().wait()
    ev3.Sound.beep().wait()
    sleep(10)

##======================================================================

##======================================================================


runForever = False

THRESHOLD_L, THRESHOLD_R, STOP_THRESHOLD = initializeSensors()

ev3.Sound.beep().wait()

while not touchSensor.value():
    pass

sleep(1)
listofmoves = "MRMMMMRMMMMRMMCRMLMLMMMMCLLMMMRMMMLMMLMLMCLMRMRMMCRMLMLMMMCRMLMLMMMCRMLMLMCLMMMRMMMRMMMLMMMMLMLMMMCLMRMRMMCRMLMLMMMCRMLMLMCLMMMRMMLMMMMLMMMLMLMMCLMRMRMMMCLMRMRMMCRMLMLMMMCRMLMMLMMMCRRMRMCLMCLMRMRMC"
#listofmoves = "MRMMMMRMMMMRMMCRMLMLMMMMCLLMMMRMMMLMMLMLMCLMRMRMMCRMLMLMMMCRMLMLMMMCRMLMLMCLMMMRMMMRMMMLMMMMLMLMMMCLMRMRMMCRMLMLMMMCRMLMLMCLMMMRMMLMMMLMRMLMMLMLMMCLMRMRMMMCLMRMRMMCRMLMLMMMCRMLMMLMMMCRRMRMCLMCLMRMRMC"
listOfMovements = []

for n in listofmoves:
    if n == "M":
        listOfMovements.append("driveForward")
    elif n == "R":
        listOfMovements.append("turnRight")
    elif n == "L":
        listOfMovements.append("turnLeft")
    elif n == "C":
        listOfMovements.append("deliverCan")
    elif n == "W":
        listOfMovements.append("wait")
    elif n == "S":
        listOfMovements.append("stop")
    else:
        listOfMovements.append("error")

state = "idle"
index = -1
turnState = "initial"
counter = 0


logfile = open("logfile.txt", "w")
logfile.write("SOKOBAN LOG FILE \n")
logfile.write("Number of Moves: " + str(len(listOfMovements)) + "\n")
logfile.write("Threshold L: " + str(THRESHOLD_L) + "Threshold R: " + str(THRESHOLD_R) + " Stop Threshold: " + str(STOP_THRESHOLD) + "\n")
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
            turnDone = TurnRel("right")
            #turnDone = Turn("right")
            if turnDone:
                turnState = "primary"
        elif turnState == "primary":
            #done = Turn("right")
            done = CenterLine()
            if done:
                state = "idle"
                turnState = "initial"
    elif state == "turnLeft":
        if turnState == "initial":
            turnDone = TurnRel("left")
            #turnDone = Turn("left")
            if turnDone:
                turnState = "primary"
        elif turnState == "primary":
            #done = Turn("left")
            done = CenterLine()
            if done:
                state = "idle"
                turnState = "initial"
    elif state == "deliverCan":
        done = deliverCan()
        if done:
            state = "idle"
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