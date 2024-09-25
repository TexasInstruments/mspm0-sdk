'''
 * Copyright (c) 2024, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
'''
import serial
import serial.tools.list_ports
import struct

class UART_send():
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
            return ''
#            sys.exit(0)
    def config_uart(self, com):
        ser = serial.Serial(com,
                            baudrate=9600,
                            bytesize=serial.EIGHTBITS,
                            parity=serial.PARITY_NONE,
                            stopbits=serial.STOPBITS_ONE,
                            timeout=5,
                            xonxoff=False,
                            rtscts=False,
                            dsrdtr=False)
        return ser
    def send_data(self, serr,data):
        serr.write(data)
    def read_data(self, serr ,num):
        bsl_pack_ack = serr.read(num).hex()
        return bsl_pack_ack
