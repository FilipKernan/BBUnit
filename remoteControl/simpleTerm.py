import serial
import time
import math
import binascii


if __name__ == '__main__':
    ser = serial.Serial('/dev/rfcomm0')
    print("conntected to " + ser.name)
    commandMethod = 128
    while input("would you like to continue: ") == 'Y':
        power = input("please enter the power at which the robot should run: ");
        power = int(power, 10)
        deltaTime = input("please enter the time in 1/10 of a sec for the robot to run: ")
        deltaTime = int(deltaTime, 10)
        commandList = [chr(commandMethod), chr(0), chr(deltaTime), chr(power), chr(deltaTime), chr(0), chr(deltaTime), chr(power), chr(deltaTime), chr(0)]
        commandStr = ""
        commandStr = commandStr.join(commandList)
        toWrite = bytes(commandStr, encoding='utf8')
        print(commandList)
        print(toWrite[2:])
        ser.flush()
        ser.write(toWrite[1:])
        waitTime = math.floor(deltaTime / 10) + math.floor(time.time())
        print(waitTime)
        while (math.floor(time.time())) < waitTime:
            message = bytes()
            while len(message) < 10:
                message += ser.readline()
            print(message)
            print('|')
            
    ser.close()
    