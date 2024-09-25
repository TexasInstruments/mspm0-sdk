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
import datetime

class TXT_to_h():
    def __init__(self):
        pass
    def conver_fun(self, file430, fileH):
        buff_flag = 0
        buff_flag2 = 0
        buff_line = ''
        data_array1 = []

        addr_count = 0
        address_array = []
        app_count_array = []
        sizecount = 0
        data_array = []
        app_sizecount = 0
        buff_count5 = 0
        output_array = 'App1'
        addr_buff = 0
        addr_buff_str =  ''

        with open(file430) as file_object:
            lines = file_object.readlines()
        for line in lines:
            if line[0] == '@':
                buff_line2 = "0x" + line[1:]
                buff_line2 = buff_line2.rstrip()
                # if int(buff_line2, 16) == 0x0:
                #     buff_flag = 1
                #     buff_flag2 = 1
                #     data_array1.append('@0010\n')
                # else:
                #     data_array1.append(line)
                if buff_count5 == 0:
                    buff_count5 = 1
                    buff_flag = 1
                    buff_flag2 = 1
                    addr_buff_str = line
                    addr_buff = int(buff_line2, 16) + 0x10
                    print(str(hex(addr_buff)))
                    data_array1.append('@'+str(hex(addr_buff))[2:]+'\n')
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
                        data_array1.append(addr_buff_str)
                        # data_array1.append('@0000\n')
                        data_array1.append(buff_line)
                    data_array1.append(line)

        # print(data_array1)
        # filee = open("example.txt", "w")
        # for liness in data_array1:
        #     filee.write(liness)
        # filee.close()


        for line in data_array1:
            if line[0] == '@':
                addr_count += 1
                line = "0x" + line[1:]
                line = line.rstrip()
                address_array.append(''.join(line))
                app_count_array.append(app_sizecount)
                #		data_array.pop()
                app_sizecount = 0
            else:
                if line[0] != 'q':
                    line = "0x" + line
                    #			print(isinstance(line, str))
                    line = line.replace(' \n', '\n')
                    line = line.replace(' ', ' 0x')
                    #			print(line)
                    sizecount += line.count('0x')
                    app_sizecount += line.count('0x')
                    data_array2 = line.split()
                    #			data_array2.pop()
                    data_array += data_array2

        app_count_array.append(app_sizecount)
        app_count_array.pop(0)

        # Write .c file
        curr_time = datetime.datetime.now()
        time_str = curr_time.strftime("%Y-%m-%d")
        current_number = 0
        with open(fileH , 'w+') as file_write:
            file_write.write("// " + time_str + "\n\n")
            file_write.write("#include <stdint.h>\n\n")
            file_write.write('#define ' + output_array + '_SIZE   ' + str(sizecount) + '\n\n')
            file_write.write('const uint32_t ' + output_array + '_Addr[] = {\n')
            while current_number < addr_count:
                file_write.write('    ' + str(address_array[current_number]) + ',\n')
                current_number += 1
            file_write.write('};\n\n')
            file_write.write('const uint32_t ' + output_array + '_Size[] = {\n')
            current_number = 0
            while current_number < addr_count:
                file_write.write('    ' + str(app_count_array[current_number]) + ',\n')
                current_number += 1
            file_write.write('};\n\n')
            current_number = 0
            i = 0
            while current_number < addr_count:
                file_write.write('const uint8_t ' + output_array + '_' + str(current_number) + '[] = {\n')
                current_number2 = 0
                while current_number2 < app_count_array[current_number]:
                    file_write.write(str(data_array[i]) + ',')
                    current_number2 += 1
                    i += 1
                    if current_number2 % 16 == 0:
                        file_write.write('\n')
                file_write.write('};\n\n')
                current_number += 1
            file_write.write('const uint8_t *' + output_array + '_Ptr[' + str(addr_count) + '] = {\n')
            current_number = 0
            while current_number < addr_count:
                file_write.write('    ' + output_array + '_' + str(current_number) + ',\n')
                current_number += 1
            file_write.write('};\n\n')
