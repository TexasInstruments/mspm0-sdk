"""
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
"""
import struct


class BSL_Pack:
    def __init__(self):
        self.BSL_HEADER = b"\x80"
        self.CMD_CONNECTION = b"\x12"
        self.CMD_GET_ID = b"\x19"
        self.CMD_PASSWORD = b"\x21"
        self.CMD_MASS_ERASE = b"\x15"
        self.CMD_PROGRAM = b"\x20"
        self.CMD_START_APP = b"\x40"

    def connection_pack(self):
        txdata_c = self.BSL_HEADER + b"\x01\x00" + self.CMD_CONNECTION
        txdata_crc_c = self.CMD_CONNECTION
        checksum_c = 0
        checksum_c = self.crc32(txdata_crc_c)
        # print("%x"%checksum_c)
        checksum_c_buff = struct.pack("I", checksum_c)
        txdata_c = txdata_c + checksum_c_buff
        # print(txdata_c)
        return txdata_c

    def get_ID_pack(self):
        txdata_c = self.BSL_HEADER + b"\x01\x00" + self.CMD_GET_ID
        txdata_crc_c = self.CMD_GET_ID
        checksum_c = 0
        checksum_c = self.crc32(txdata_crc_c)
        # print("%x"%checksum_c)
        checksum_c_buff = struct.pack("I", checksum_c)
        txdata_c = txdata_c + checksum_c_buff
        # print(txdata_c)
        return txdata_c

    def password_pack(self, password):
        bsl_err = 0
        values = bytearray(password)
        #       print(values)
        DATA_Length = len(values) + 1
        DATA_Length_B = bytearray([DATA_Length])
        # print(DATA_Length_B)
        txdata = self.BSL_HEADER + DATA_Length_B + b"\x00" + self.CMD_PASSWORD + values
        txdata_crc = self.CMD_PASSWORD + values
        checksum = 0
        checksum = self.crc32(txdata_crc)
        # print("%x"%checksum)
        checksum_B = struct.pack("I", checksum)
        #    print(checksum_B)
        txdata = txdata + checksum_B
        # print(txdata)
        return txdata

    def mass_erase_pack(self):
        txdata_c = self.BSL_HEADER + b"\x01\x00" + self.CMD_MASS_ERASE
        txdata_crc_c = self.CMD_MASS_ERASE
        checksum_c = 0
        checksum_c = self.crc32(txdata_crc_c)
        # print("%x"%checksum_c)
        checksum_c_buff = struct.pack("I", checksum_c)
        txdata_c = txdata_c + checksum_c_buff
        # print(txdata_c)
        return txdata_c

    def firmware_pack(self, firmware):
        bsl_err = 0
        send_count = 0
        address_count = 0
        bytes_buf = b""
        addr_buf = b""
        addr_bufarray = b""
        data_array = []
        data_b_array = []
        #       with open('test0.txt', 'w+') as file_write:
        #           for line in firmware:
        #               file_write.write(line)
        # print(firmware)
        for line in firmware:
            if line[0] != "q":
                if line[0] == "@":
                    #                data_array.append(line)
                    linel = line[1:]
                    linel = linel.rstrip()
                    address_count = int(linel, 16)
                    # print(type(address_count))
                    # print(address_count)
                    if send_count != 0:
                        data_array.append("&\n")
                    send_count = 0
                #                print(address)
                else:
                    #                print(line)
                    linel = line.rstrip()
                    bytes_buf = bytes.fromhex(linel)
                    send_count += len(bytes_buf)
                    address_count += len(bytes_buf)
                    if send_count > 128:
                        address_count1 = address_count - len(bytes_buf)
                        #                    print(hex(address_count1))
                        #                    print(line)
                        linel = hex(address_count1)
                        linel = linel[2:]
                        data_array.append("&\n")
                        linel = "@" + linel + "\n"
                        data_array.append(linel)
                        send_count = len(bytes_buf)
            #               data_array.append(line)
            else:
                data_array.append("&\n")
            data_array.append(line)
        #    print(data_array)
        #     with open('test1.txt', 'w+') as file_write:
        #         for line in data_array:
        #             file_write.write(line)
        bytes_buf = b""
        #       print(data_array)
        for line in data_array:
            if line[0] != "q":
                if line[0] == "@":
                    send_count = 0
                    #                data_array.append(line)
                    linel = line[1:]
                    linel = linel.rstrip()
                    while len(linel) < 8:
                        linel = "0" + linel
                    # print(linel)
                    addr_buf = bytes.fromhex(linel)
                    addr_bufarray = bytearray(addr_buf)
                    addr_bufarray.reverse()
                    addr_bufarray = self.CMD_PROGRAM + addr_bufarray
                    #                print(addr_bufarray)
                    #                address_count_b=struct.pack('<I',address_count)
                    #                print(address_count_b)
                    # print(addr_bufarray)
                    bytes_buf = b""
                    DATA_Length1 = 0
                else:
                    if line[0] != "&":
                        linel = line.rstrip()
                        #                    print(linel)
                        bytes_buf += bytes.fromhex(linel)
                    else:
                        addr_bufarray += bytes_buf
                        checksum = 0
                        checksum = self.crc32(addr_bufarray)
                        # print("%x"%checksum)
                        checksum_B = struct.pack("I", checksum)
                        #                    print(checksum_B)
                        DATA_Length1 = len(bytes_buf) + 5
                        # print(DATA_Length1)
                        DATA_Length1_b = struct.pack("<H", DATA_Length1)
                        #                    print(DATA_Length1_b)
                        DATA_Length1_b += addr_bufarray
                        DATA_Length1_b = b"\x80" + DATA_Length1_b + checksum_B
                        data_b_array.append(DATA_Length1_b)
        return data_b_array

    def crc32(self, data_B):
        crc = 0xFFFFFFFF
        crc32_POLY = 0xEDB88320
        for b in data_B:
            crc = crc ^ b
            ii = 1
            while ii <= 8:
                mask = -(crc & 1)
                crc = (crc >> 1) ^ (crc32_POLY & mask)
                ii += 1
        return crc

    def start_app_pack(self):
        txdata_d = self.BSL_HEADER + b"\x01\x00" + self.CMD_START_APP
        txdata_crc_d = self.CMD_START_APP
        checksum_d = 0
        checksum_d = self.crc32(txdata_crc_d)
        # print("%x"%checksum_c)
        checksum_d_buff = struct.pack("I", checksum_d)
        txdata_d = txdata_d + checksum_d_buff
        # print(txdata_c)
        return txdata_d
