import ev3dev.ev3 as ev3
from time import sleep

mL = ev3.LargeMotor('outD')
mR = ev3.LargeMotor('outA')

runSpeed = 300
turnSpeed = 200
turnDistanceWheel = 130


#def moveStraigth(direction, errorL, errorR):
#    if direction == 'backward':
#        mL.run_forever(speed_sp=int(errorL*runSpeed))
#        mR.run_forever(speed_sp=int(errorR*runSpeed))
#    else:
#        mL.run_forever(speed_sp=-int(errorL*runSpeed))
#        mR.run_forever(speed_sp=-int(errorR*runSpeed))


def moveStraigth(direction, sensorLeft, sensorRight, TH):
    if direction == 'backward':
        mL.run_forever(speed_sp=int(errorL*runSpeed))
        mR.run_forever(speed_sp=int(errorR*runSpeed))
    else:
        diff = 1-(abs(sensorLeft-sensorRight)/TH)
        if sensorLeft > sensorRight:
            mL.run_forever(speed_sp=-int(runSpeed))
            mR.run_forever(speed_sp=-int(runSpeed) * diff)
        elif sensorLeft < sensorRight:
            mL.run_forever(speed_sp=-int(runSpeed) * diff)
            mR.run_forever(speed_sp=-int(runSpeed))


def moveReal(direction, distance):
    if direction == "backward":
        mL.run_to_rel_pos(position_sp=distance, speed_sp=runSpeed, stop_action="hold")
        mR.run_to_rel_pos(position_sp=distance, speed_sp=runSpeed, stop_action="hold")
        mL.wait_while('running')
    else:
        mL.run_to_rel_pos(position_sp=-distance, speed_sp=runSpeed, stop_action="hold")
        mR.run_to_rel_pos(position_sp=-distance, speed_sp=runSpeed, stop_action="hold")
        mL.wait_while('running')
    #while mL.state == 'running' or mR.state == 'running':
    #    sleep(0.1)


def stopMovement():
    mL.stop(stop_action='brake')
    mR.stop(stop_action='brake')
    mL.run_forever(speed_sp=0)
    mR.run_forever(speed_sp=0)


def turnRel(direction, turnDistance):
    if direction == "right":
        mL.run_to_rel_pos(position_sp=-turnDistance, speed_sp=(turnSpeed), stop_action="hold")
        mR.run_to_rel_pos(position_sp=turnDistance, speed_sp=(turnSpeed), stop_action="hold")
        mL.wait_while('running')
        mR.wait_while('running')
    elif direction == "left":
        mL.run_to_rel_pos(position_sp=turnDistance, speed_sp=(turnSpeed), stop_action="hold")
        mR.run_to_rel_pos(position_sp=-turnDistance, speed_sp=(turnSpeed), stop_action="hold")
        mL.wait_while('running')
        mR.wait_while('running')

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
