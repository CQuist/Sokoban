import ev3dev.ev3 as ev3
from time import sleep

mL = ev3.LargeMotor('outD')
mR = ev3.LargeMotor('outA')

#runSpeed = 300
turnSpeed = 100
turnDistanceWheel = 130


def moveStraigth(direction, errorL, errorR, runSpeed=300):
    if direction == 'backward':
        mL.run_forever(speed_sp=int(errorL*runSpeed))
        mR.run_forever(speed_sp=int(errorR*runSpeed))
    else:
        mL.run_forever(speed_sp=-int(errorL*runSpeed))
        mR.run_forever(speed_sp=-int(errorR*runSpeed))


def moveReal(direction, distance):
    if direction == "backward":
        mL.run_to_rel_pos(position_sp=distance, speed_sp=300, stop_action="hold")
        mR.run_to_rel_pos(position_sp=distance, speed_sp=300, stop_action="hold")
        mL.wait_while('running')
    else:
        mL.run_to_rel_pos(position_sp=-distance, speed_sp=300, stop_action="hold")
        mR.run_to_rel_pos(position_sp=-distance, speed_sp=300, stop_action="hold")
        mL.wait_while('running')
    #while mL.state == 'running' or mR.state == 'running':
    #    sleep(0.1)


def stopMovement():
    mL.stop(stop_action='brake')
    mR.stop(stop_action='brake')
    mL.run_forever(speed_sp=0)
    mR.run_forever(speed_sp=0)


def turnRel(direction):
    if direction == "right":
        mL.run_to_rel_pos(position_sp=-turnDistanceWheel, speed_sp=(turnSpeed), stop_action="hold")
        mR.run_to_rel_pos(position_sp=turnDistanceWheel, speed_sp=(turnSpeed), stop_action="hold")
        if mL.state == 'running':
            return False
        else:
            return True
    elif direction == "left":
        mL.run_to_rel_pos(position_sp=turnDistanceWheel, speed_sp=(turnSpeed), stop_action="hold")
        mR.run_to_rel_pos(position_sp=-turnDistanceWheel, speed_sp=(turnSpeed), stop_action="hold")
        if mL.state == 'running':
            return False
        else:
            return True

def turn(direction):
    if direction == "right":
        mL.run_forever(speed_sp=-turnSpeed)
        mR.run_forever(speed_sp=turnSpeed)
    elif direction == "left":
        mL.run_forever(speed_sp=turnSpeed)
        mR.run_forever(speed_sp=-turnSpeed)


def adjustLeft(direction, error):
    if direction == 'backward':
        mL.run_forever(speed_sp=error*runSpeed)
        mR.run_forever(speed_sp=runSpeed)
    else:
        mL.run_forever(speed_sp=-error*runSpeed)
        mR.run_forever(speed_sp=-runSpeed)


def adjustRight(direction, error):
    if direction == 'backward':
        mL.run_forever(speed_sp=runSpeed)
        mR.run_forever(speed_sp=error*runSpeed)
    else:
        mL.run_forever(speed_sp=-runSpeed)
        mR.run_forever(speed_sp=-error*runSpeed)
