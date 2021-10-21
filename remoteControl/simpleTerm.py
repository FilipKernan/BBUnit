import socket
import time
import math
import PySimpleGUI as sg

Addr = ("192.168.4.1", 80)
messageIndex = 1

def rs232_checksum(the_bytes):
    return b'%02X' % (sum(the_bytes) & 0xFF)

if __name__ == '__main__':
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        sg.Window(title="controler", layout=[[]], margins=(100,50))
        s.connect(Addr)
        print("connected")
        while True:
            messageType = chr(int(input("please enter your message type: "))).encode("utf-8")
            isMessageMultiPart = input("is this message multipart(y/n): ")
            if isMessageMultiPart == 'y':
                isMessageMultiPart = True
            else:
                isMessageMultiPart = False
            messageString = input("please enter your message contents: ")
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
            if 'n' in toContinue:
                exit()
            
    
    