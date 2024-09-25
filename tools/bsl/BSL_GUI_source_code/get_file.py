
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

class Get_files():
    def __init__(self):
        self.password1 = b''
    def get_password(self, password_file):
        flag1 = 0
        flag2 = 0
        self.password1 = b''
        with open(password_file) as file_object:
            lines = file_object.readlines()
            #    print(lines)
        for line in lines:
            line = line.rstrip()
            #        print(line[0:5])
            if line[0:9] == '@password' or line[0:9] == '@00000000' or line[0:9] == '@41c00110':
                flag1 = 1
                continue
            if flag1 == 1:
                flag2 += 1
                #            print(bytes(line, encoding="utf8"))
                #            print(line)
                self.password1 += bytes.fromhex(line)
            if flag2 == 2:
                #            print(password1)
                break
        if len(self.password1)!=32:
            self.password1 = b''
        return self.password1

    def get_firmware(self, firmware_file):
        buff_flag = 0
        buff_flag2 = 0
        buff_line = ''
        data_array1 = []

        with open(firmware_file) as file_object:
            lines = file_object.readlines()
        for line in lines:
            if line[0] == '@':
                buff_line2 = "0x" + line[1:]
                buff_line2 = buff_line2.rstrip()
                if int(buff_line2, 16) == 0x0:
                    buff_flag = 1
                    buff_flag2 = 1
                    data_array1.append('@0010\n')
                else:
                    data_array1.append(line)
            else:
                if line[0] != 'q':
                    if buff_flag == 1:
                        buff_line = line
                        buff_flag = 0
                    else:
                        data_array1.append(line)
                else:
                    if buff_flag2 == 1:
                        data_array1.append('@0000\n')
                        data_array1.append(buff_line)
                    data_array1.append(line)
        # with open('test1.txt', 'w+') as file_write:
        #     for line in data_array1:
        #         file_write.write(line)

        return data_array1