import socket
import time
import math
import PySimpleGUI as sg
import binascii

Addr = ("192.168.4.1", 80)
messageIndex = 1
messageType = 0
isMessageMultiPart = False
messageString = ""
window_layout = [
    [
        # place buttons here 
        sg.Button("Reboot", key="REBOOT"),
        sg.Button("Report if Alive", key="REPORT_ALIVE"),
        sg.Button("Listen for OTAs", key="OTA"),
        sg.Button("Report Motor", key="REPORT_MOTOR"),
        sg.Button("Report Gyro", key="REPORT_GYR0"),
        sg.Button("Report Goal", key="REPORT_GOAL"),
        sg.Button("Set Motor Power", key="SET_MOTOR_POWER"),
        sg.Button("Set Encoder Goal", key="SET_ENCODER_GOAL;"),
        sg.Button("Set gyro goal", key="REPORT_GYRO_GOAL"),
        sg.Button("Set PID value", key="SET_PID"),
        sg.Button("Get PID value", key="REPORT_PID"),
        sg.Button("Plain Text", key="PLAIN_TEXT"),
        sg.Button("Play Sound", key="PLAY_SOUND")
    ],
    [
        sg.Text(text="Motor Index"),
        sg.Input(size=(10, 1), key="-MOTOR_INDEX-"),
        sg.Text(text="Motor Power"),
        sg.Input(size=(10, 1), key="-MOTOR_POWER-"),
        sg.Text(text="Encoder Goal"),
        sg.Input(size=(10, 1), key="-ENCODER_GOAL-"),
        sg.Text("Gyro Index"),
        sg.Input(size=(10, 1), key="-GYRO_INDEX-"),
        sg.Text(text="Gyro Goal"),
        sg.Input(size=(10, 1), key="-GYRO_GOAL-"),

        sg.Text("PID constant"),
        sg.Input(size=(10, 1), key="-PID_CONSTANT-"),
        sg.Text("PID value"),
        sg.Input(size=(10, 1), key="-PID_VALUE-"),
        sg.Text("Sound"),
        sg.Input(size=(10, 1), key="-SOUND_VALUE-")
        # place text boxes here
    ],
    [
        sg.Output(size=(200, 15), echo_stdout_stderr=True)
        # place console output here 
    ]
]

sendMessage = True


def bitwise_and_bytes(a, b):
    result_int = int.from_bytes(a, byteorder="big") & int.from_bytes(b, byteorder="big")
    return result_int.to_bytes(max(len(a), len(b)), byteorder="big")


def bitwise_or_bytes(a, b):
    result_int = int.from_bytes(a, byteorder="big") | int.from_bytes(b, byteorder="big")
    return result_int.to_bytes(max(len(a), len(b)), byteorder="big")


def sendMessage(messageType, isMessageMultiPart, messageString):
    msg = messageString.encode("utf-8")
    msg += ('\0' * (12 - len(msg))).encode()
    if isMessageMultiPart:
        pass
    else: 
        mutlipartMessage = "\0".encode()
    print(msg.decode())
    msg = chr(messageIndex).encode("utf-8") + messageType + mutlipartMessage + msg 
    print(msg.decode())
    checksum = rs232_checksum(msg)
    msg = msg + checksum
    print(msg.decode())
    s.send(msg)
    print("sending message")
    data = s.recv(32)
    print("return message")
    print(data.decode())
    toContinue = input("do you want to continue(y/n): ")
    messageIndex += 1

def handleEvent(event, values):
    print(event)
    print(values)
    isMessageMultiPart = False

    # pass
    if event == "REBOOT":
        messageType = 1
        messageString = ""
    elif event == "REPORT_ALIVE":
        messageType = 2
        messageString = ""
    elif event == "OTA":
        messageType = 3
        messageString = ""
    elif event == "REPORT_MOTOR":
        messageType = 4
        messageString = chr(int(values.get('-MOTOR_INDEX-')) + 1) 
        # adding 1 to motor index to prevent byte from being '/0'
        # change this once you have created the byte masks for everything
    elif event == "REPORT_GYR0":
        messageType = 5
        messageString = chr(int(values.get('-GYRO_INDEX-')) + 1)
    elif event == "REPORT_GOAL":
        messageType = 8
        messageString = ""
    elif event == "SET_MOTOR_POWER":
        messageType = 9
        messageString =  chr(int(values.get('-MOTOR_INDEX-')) + 1) + chr(int(values.get('-MOTOR_POWER-')))
    elif event == "SET_ENCODER_GOAL":
        messageType = 10
        messageString = chr(int(values.get('-MOTOR_INDEX-')) + 1) + chr(int(values.get('-ENCODER_GOAL-')))
    elif event == "SET_GYRO_GOAL":
        messageType = 11
        messageString = chr(int(values.get('-GYRO_INDEX-')) + 1) + chr(int(values.get('-GYRO_GOAL-')))
    elif event == "SET_PID":
        messageType = 12
        if values.get('-MOTOR_INDEX-') is not '':
            pidIndex = chr(int(values.get('-MOTOR_INDEX-')) + 64)
        else:
            pidIndex = chr(int(values.get('-GYRO_INDEX-')) + 1)
        messageString = pidIndex + values.get('-PID_CONSTANT-') + "v" + values.get('-PID_VALUE-')
    elif event == "REPORT_PID":
        messageType = 13
        if values.get('-MOTOR_INDEX-') is not '':
            pidIndex = chr(int(values.get('-MOTOR_INDEX-')) + 64)
        else:
            pidIndex = chr(int(values.get('-GYRO_INDEX-')) + 1)
        messageString = pidIndex + values.get('-PID_CONSTANT-')
    elif event == "PLAIN_TEXT":
        messageType = 15
        messageString = ""
    elif event == "PLAY_SOUND":
        messageType = 17
        messageString = chr(int(values.get('-SOUND_VALUE-')) + 1)
    return (messageType, messageString, isMessageMultiPart)




def rs232_checksum(the_bytes):
    return b'%02X' % (sum(the_bytes) & 0xFF)

if __name__ == '__main__':
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        sg.theme("Dark")
        window = sg.Window(title="controler", layout=window_layout, margins=(10,10))
        s.connect(Addr)
        print("connected")
        while True:
            event, values = window.read()
            if event == sg.WIN_CLOSED:
                break
            messageType, messageString, isMessageMultiPart = handleEvent(event, values)
            print(messageType)
            if sendMessage:
                msg = messageString.encode("utf-8")
                msg = ('\0' * (12 - len(msg))).encode() + msg
                if isMessageMultiPart:
                    pass
                else: 
                    mutlipartMessage = "\0".encode()
                msg = chr(messageIndex).encode("utf-8") + chr(messageType).encode('utf-8') + mutlipartMessage + msg 
                print(msg.decode())
                checksum = rs232_checksum(msg)
                msg = msg + checksum
                print(msg)
                s.send(msg)
                print("sending message")
                data = s.recv(32)
                print("return message")
                print(data.decode())
                messageIndex += 1
    window.close()

            
    
    