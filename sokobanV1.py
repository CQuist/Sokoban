#!/usr/bin/python3

import ev3dev.ev3 as ev3
from motor import *
from time import sleep

lightSensorLeft = ev3.ColorSensor('in4')
lightSensorRight = ev3.ColorSensor('in1')

THRESHOLD = 10

##======================================================================
State = type("State", (object,), {})


class GoForward(State):
    def __init__(self, FSM):
        super()

    def execute(self):
        sensorLeft = lightSensorLeft.value()
        sensorRight = lightSensorRight.value()
        if sensorLeft > THRESHOLD and sensorRight > THRESHOLD:
            moveStreigth('forward')
        elif sensorLeft < THRESHOLD and sensorRight > THRESHOLD:
            adjustLeft('forward')
        elif sensorLeft > THRESHOLD and sensorRight < THRESHOLD:
            adjustRight('forward')
        elif sensorLeft < THRESHOLD and sensorRight < THRESHOLD:
            stopMovement()
            self.FSM.transtion("stateIdle")


class GoBackward(State):
    def __init__(self, FSM):
        super(GoBackward, self).__init__(FSM)

    def execute(self):
        sensorLeft = lightSensorLeft.value()
        sensorRight = lightSensorRight.value()
        if sensorLeft > THRESHOLD and sensorRight > THRESHOLD:
            moveStreigth('backward')
        elif sensorLeft < THRESHOLD and sensorRight > THRESHOLD:
            adjustLeft('backward')
        elif sensorLeft > THRESHOLD and sensorRight < THRESHOLD:
            adjustRight('backward')
        elif sensorLeft < THRESHOLD and sensorRight < THRESHOLD:
            stopMovement()
            self.FSM.transtion("stateIdle")


class TurnRight(State):
    def __init__(self, FSM):
        super(TurnRight, self).__init__(FSM)

    def execute(self):
        turnRight()


class TurnLeft(State):
    def __init__(self, FSM):
        super(TurnLeft, self).__init__(FSM)

    def execute(self):
        turnLeft()


class StopMovement(State):
    def __init__(self, FSM):
        super(StopMovement, self).__init__(FSM)

    def execute(self):
        stopMovement()


class Idle(State):
    def __init__(self, FSM):
        super(Idle, self).__init__(FSM)

    def execute(self):
        pass


##======================================================================

class Transition(object):
    def __init__(self, toState):
        self.toState = toState

    def execute(self):
        # code here if action of transition
        pass


##======================================================================

class StateMachine(object):
    def __init__(self, char):
        self.char = char
        self.states = {}
        self.transitions = {}
        self.currentState = None
        self.previousState = None
        self.trans = None

    def addState(self, stateName, state):
        self.states[stateName] = state

    def addTransition(self, transName, transition):
        self.transitions[transName] = transition

    def setState(self, stateName):
        self.previousState = self.currentState
        self.currentState = self.states[stateName]

    def transition(self, transName):
        self.trans = self.transitions[transName]

    def execute(self):
        if (self.trans):
            self.trans.execute()
            self.setState(self.trans.toState)
            self.trans = None
        self.currentState.execute()


##======================================================================
Char = type("Char", (object,), {})


class RobotController(Char):
    def __init__(self):
        self.FSM = StateMachine(self)

        ## STATES
        self.FSM.addState("Forward", GoForward(self.FSM))
        self.FSM.addState("Backward", GoBackward(self.FSM))
        self.FSM.addState("Right", TurnRight(self.FSM))
        self.FSM.addState("Left", TurnLeft(self.FSM))
        self.FSM.addState("Stop", StopMovement(self.FSM))
        self.FSM.addState("Idle", Idle(self.FSM))

        ## TRANSITIONS
        self.FSM.addTransition("stateForward", Transition("Forward"))
        self.FSM.addTransition("stateBackward", Transition("Backward"))
        self.FSM.addTransition("stateRight", Transition("Right"))
        self.FSM.addTransition("stateLeft", Transition("Left"))
        self.FSM.addTransition("stateStop", Transition("Stop"))
        self.FSM.addTransition("stateIdle", Transition("Idle"))

        self.FSM.setState("Idle")

    def execute(self):
        self.FSM.execute()


##======================================================================

#if __name__ == '__main__':
drive = RobotController()

listOfMovements = {"Forward", "toLeft", "toLeft", "toForward", "toStop"}
index = 0

#drive.FSM.states["Forward"] = GoForward()
#drive.FSM.states["Backward"] = GoForward()
#drive.FSM.states["Right"] = GoForward()
#drive.FSM.states["Left"] = GoForward()
#drive.FSM.states["Stop"] = GoForward()
#drive.FSM.states["Idle"] = GoForward()
#drive.FSM.transitions["stateForward"] = Transition("Forward")
#drive.FSM.transitions["stateBackward"] = Transition("Backward")
#drive.FSM.transitions["stateRight"] = Transition("Right")
#drive.FSM.transitions["stateLeft"] = Transition("Left")
#drive.FSM.transitions["stateStop"] = Transition("Stop")
#drive.FSM.transitions["stateIdle"] = Transition("Idle")

#drive.FSM.setState("Idle")


while True:

    if drive.FSM.currentState == Idle():
        drive.FSM.transition(listOfMovements[index])
        index = index + 1

    drive.execute()

    if index == len(listOfMovements):
        break

# https://www.youtube.com/watch?v=E45v2dD3IQU
