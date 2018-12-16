import ev3dev.ev3 as ev3
from time import sleep

mL = ev3.LargeMotor('outD')
mR = ev3.LargeMotor('outA')

runSpeed = 900  # 600 works
turnSpeed = 600 # 400 works
turnDistanceWheel = 360


prev_error = [0, 0]
integrator = [0, 0]
integratorMinMax = [0, 10]


# TUNING ADVICE
# https://robotics.stackexchange.com/questions/9786/how-do-the-pid-parameters-kp-ki-and-kd-affect-the-heading-of-a-differential

def PID(errorL, errorR):

    #change it to take the raw value and not the normalized

    Kp = 1 #4
    Ki = 0
    Kd = 0.5 #0.5

    ### LEFT MOTOR

    pL = Kp * errorL
    dL = Kd * (1-abs(errorL - prev_error[0]))
    prev_error[0] = errorL

    integrator[0] = integrator[0] + errorL

    # clamp
    if integrator[0] < integratorMinMax[0]:
        integrator[0] = integratorMinMax[0]
    elif integrator[0] > integratorMinMax[1]:
        integrator[0] = integratorMinMax[1]

    iL = Ki * integrator[0]

    gainL = pL+dL+iL
    normGainL = gainL / (Kp + integratorMinMax[1] * Ki + Kd)

    ### Right MOTOR

    pR = Kp * errorR
    dR = Kd * (1 - abs(errorR - prev_error[1]))
    prev_error[1] = errorR

    integrator[1] = integrator[1] + errorR

    # clamp
    if integrator[1] < integratorMinMax[0]:
        integrator[1] = integratorMinMax[0]
    elif integrator[1] > integratorMinMax[1]:
        integrator[1] = integratorMinMax[1]

    iR = Ki * integrator[1]

    gainR = pR + dR + iR
    normGainR = gainR / (Kp + integratorMinMax[1] * Ki + Kd)

    return normGainL, normGainR



#def moveStraigth(direction, errorL, errorR):
#    if direction == 'backward':
#        mL.run_forever(speed_sp=-int(errorL*runSpeed))
#        mR.run_forever(speed_sp=-int(errorR*runSpeed))
#    else:
#        mL.run_forever(speed_sp=int(errorL*runSpeed))
#        mR.run_forever(speed_sp=int(errorR*runSpeed))

def moveStraigth(direction, errorL, errorR):
    gainL, gainR = PID(errorL, errorR)
    if direction == 'backward':
        mL.run_forever(speed_sp=-int(gainL*runSpeed))
        mR.run_forever(speed_sp=-int(gainR*runSpeed))
    else:
        mL.run_forever(speed_sp=int(gainL*runSpeed))
        mR.run_forever(speed_sp=int(gainR*runSpeed))


#def moveStraigth(direction, sensorLeft, sensorRight, TH):
#    if direction == 'backward':
#        mL.run_forever(speed_sp=-int(errorL*runSpeed))
#        mR.run_forever(speed_sp=-int(errorR*runSpeed))
#    else:
#        diff = 1-(abs(sensorLeft-sensorRight)/TH)
#        if sensorLeft > sensorRight:
#            mL.run_forever(speed_sp=int(runSpeed))
#            mR.run_forever(speed_sp=int(runSpeed) * diff)
#        elif sensorLeft < sensorRight:
#            mL.run_forever(speed_sp=int(runSpeed) * diff)
#            mR.run_forever(speed_sp=int(runSpeed))


def moveReal(direction, distance):
    if direction == "backward":
        mL.run_to_rel_pos(position_sp=-distance, speed_sp=runSpeed, stop_action="hold")
        mR.run_to_rel_pos(position_sp=-distance, speed_sp=runSpeed, stop_action="hold")
        mL.wait_while('running')
        mR.wait_while('running')
    else:
        mL.run_to_rel_pos(position_sp=distance, speed_sp=runSpeed, stop_action="hold")
        mR.run_to_rel_pos(position_sp=distance, speed_sp=runSpeed, stop_action="hold")
        mL.wait_while('running')
        mR.wait_while('running')


def stopMovement():
    mL.stop(stop_action='brake')
    mR.stop(stop_action='brake')
    mL.run_forever(speed_sp=0)
    mR.run_forever(speed_sp=0)


def turnRel(direction, turnDistance):
    if direction == "right":
        mL.run_to_rel_pos(position_sp=turnDistance, speed_sp=(turnSpeed), stop_action="hold")
        mR.run_to_rel_pos(position_sp=-turnDistance, speed_sp=(turnSpeed), stop_action="hold")
        mL.wait_while('running')
        mR.wait_while('running')
    elif direction == "left":
        mR.run_to_rel_pos(position_sp=turnDistance, speed_sp=(turnSpeed), stop_action="hold")
        mL.run_to_rel_pos(position_sp=-turnDistance, speed_sp=(turnSpeed), stop_action="hold")
        mR.wait_while('running')
        mL.wait_while('running')

def turn(direction, speed=turnSpeed):
    if direction == "right":
        mL.run_forever(speed_sp=speed)
        mR.run_forever(speed_sp=-speed)
    elif direction == "left":
        mR.run_forever(speed_sp=speed)
        mL.run_forever(speed_sp=-speed)


def adjustLeft(direction, error):
    if direction == 'backward':
        mL.run_forever(speed_sp=-error*runSpeed)
        mR.run_forever(speed_sp=-runSpeed)
    else:
        mL.run_forever(speed_sp=error*runSpeed)
        mR.run_forever(speed_sp=runSpeed)


def adjustRight(direction, error):
    if direction == 'backward':
        mL.run_forever(speed_sp=-runSpeed)
        mR.run_forever(speed_sp=-error*runSpeed)
    else:
        mL.run_forever(speed_sp=runSpeed)
        mR.run_forever(speed_sp=error*runSpeed)
