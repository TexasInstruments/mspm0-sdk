class Get_files:
    def __init__(self):
        self.password1 = b""

    def get_password(self, password_file):
        flag1 = 0
        flag2 = 0
        self.password1 = b""
        with open(password_file) as file_object:
            lines = file_object.readlines()
            #    print(lines)
        for line in lines:
            line = line.rstrip()
            #        print(line[0:5])
            if line[0:9] == "@password":
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
        if len(self.password1) != 32:
            self.password1 = b""
        return self.password1

    def get_firmware(self, firmware_file):
        buff_address = 0
        buff_count = 0
        buff_count2 = 0
        buff_count3 = 0
        buff_count4 = 0
        buff_count5 = 0
        buff_address2 = 0
        data_array3 = []
        data_array1 = []
        with open(firmware_file) as file_object:
            lines = file_object.readlines()
        for line in lines:
            if line[0] == "@":
                line = "0x" + line[1:]
                line = line.rstrip()
                if int(line, 16) <= 0xFFFF:
                    buff_count += 1
                    buff_address = int(line, 16)
                else:
                    break
        for line in lines:
            if line[0] == "@":
                buff_count2 += 1
                if buff_count3 > 0:
                    break
                if buff_count2 == buff_count:
                    buff_count3 = 1
            else:
                if line[0] != "q":
                    if buff_count3 > 0:
                        buff_count4 += line.count(" ")
                else:
                    break
        if buff_address + buff_count4 > 0x10000:
            buff_address2 = 1
        buff_count2 = 0
        buff_data_array = ""
        buff_data_array2 = ""
        #        print(buff_address2)
        if buff_address2 > 0:
            for line in lines:
                if line[0] == "@":
                    buff_count2 += 1
                    data_array3.append(line)
                    if buff_count2 == buff_count:
                        linel = "0x" + line[1:]
                        linel = linel.rstrip()
                        buff_count3 = 0x10000 - int(linel, 16)
                        buff_count5 = 1
                # 					print(buff_count3)
                else:
                    if buff_count5 > 0:
                        # 					print(line)
                        if buff_count3 > 16:
                            buff_count3 -= 16
                            data_array3.append(line)
                        # 						print(buff_count3)
                        else:
                            buff_count5 = 0
                            data_array2 = line.split()
                            # 						print(data_array2)
                            # 						print(buff_count3)
                            # 						data_array2.insert(buff_count3, '/n@10000/n')
                            # 						print(data_array2)
                            for item in data_array2:
                                if buff_count3 > 0:
                                    buff_data_array += item + " "
                                    buff_count3 -= 1
                                else:
                                    buff_data_array2 += item + " "
                            buff_data_array += "\n"
                            buff_data_array2 += "\n"
                            data_array3.append(buff_data_array)
                            data_array3.append("@10000\n")
                            data_array3.append(buff_data_array2)
                            # 						print(buff_data_array)
                            # 						print(buff_data_array2)
                            line = data_array2
                    else:
                        data_array3.append(line)
        else:
            for line in lines:
                data_array3.append(line)
        for line in data_array3:
            if line[0] == "@":
                linel = line[1:]
                linel = linel.rstrip()
                if 0xFFA2 <= int(linel, 16) <= 0xFFFF:
                    #                print(hex(int(linel,16)))
                    linel = str(hex(int(linel, 16) - 0xC2))
                    linel = linel[2:]
                #               print(hex(int(linel,16)))
                line = "@" + linel + "\n"
            data_array1.append(line)
        # with open('test0.txt', 'w+') as file_write:
        #     for line in data_array1:
        #         file_write.write(line)
        return data_array1
