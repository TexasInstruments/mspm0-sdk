import serial
import serial.tools.list_ports
import struct


class UART_send:
    def __init__(self):
        pass

    def find_MSP_COM(self):
        flag = 0
        port_list = list(serial.tools.list_ports.comports())
        if len(port_list) == 0:
            flag = 0
        else:
            for i in range(0, len(port_list)):
                #        print(port_list[i])
                buffer = list(port_list[i])
                buffer1 = buffer[1]
                #        print(buffer1)
                #        print(type(buffer1))
                if buffer1.find("XDS110 Class Application/User UART") >= 0:
                    flag = 1
                    break
        if flag:
            #            print("Find the COM port: " + buffer[0])
            return buffer[0]
        else:
            return ""

    #            sys.exit(0)
    def config_uart(self, com):
        ser = serial.Serial(
            com,
            baudrate=9600,
            bytesize=serial.EIGHTBITS,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            timeout=5,
            xonxoff=False,
            rtscts=False,
            dsrdtr=False,
        )
        return ser

    def send_data(self, serr, data):
        serr.write(data)

    def read_data(self, serr, num):
        bsl_pack_ack = serr.read(num).hex()
        return bsl_pack_ack
