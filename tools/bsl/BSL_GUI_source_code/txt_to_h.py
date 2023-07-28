import datetime


class TXT_to_h:
    def __init__(self):
        pass

    def conver_fun(self, file430, fileH):
        buff_count = 0
        buff_address = 0
        buff_count3 = 0
        buff_count4 = 0
        buff_address2 = 0
        buff_count2 = 0
        data_array3 = []
        buff_data_array = ""
        buff_data_array2 = ""
        addr_count = 0
        address_array = []
        app_count_array = []
        sizecount = 0
        data_array = []
        app_sizecount = 0
        buff_count5 = 0
        output_array = "App1"
        with open(file430) as file_object:
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
        print(buff_count2)
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
                addr_count += 1
                line = "0x" + line[1:]
                line = line.rstrip()
                address_array.append("".join(line))
                app_count_array.append(app_sizecount)
                # 		data_array.pop()
                app_sizecount = 0
            else:
                if line[0] != "q":
                    line = "0x" + line
                    # 			print(isinstance(line, str))
                    line = line.replace(" \n", "\n")
                    line = line.replace(" ", " 0x")
                    # 			print(line)
                    sizecount += line.count("0x")
                    app_sizecount += line.count("0x")
                    data_array2 = line.split()
                    # 			data_array2.pop()
                    data_array += data_array2

        app_count_array.append(app_sizecount)
        app_count_array.pop(0)
        i = 0
        for element in address_array:
            if 0xFFA2 <= int(element, 16) <= 0xFFFF:
                address_array[i] = str(hex(int(element, 16) - 0xC2))
            i += 1
        # Write .c file
        curr_time = datetime.datetime.now()
        time_str = curr_time.strftime("%Y-%m-%d")
        current_number = 0
        with open(fileH, "w+") as file_write:
            file_write.write("// " + time_str + "\n\n")
            file_write.write("#include <stdint.h>\n\n")
            file_write.write(
                "#define " + output_array + "_SIZE   " + str(sizecount) + "\n\n"
            )
            file_write.write(
                "const uint32_t "
                + output_array
                + "_Addr["
                + str(addr_count)
                + "] = {\n"
            )
            while current_number < addr_count:
                file_write.write("    " + str(address_array[current_number]) + ",\n")
                current_number += 1
            file_write.write("};\n\n")
            file_write.write(
                "const uint32_t "
                + output_array
                + "_Size["
                + str(addr_count)
                + "] = {\n"
            )
            current_number = 0
            while current_number < addr_count:
                file_write.write("    " + str(app_count_array[current_number]) + ",\n")
                current_number += 1
            file_write.write("};\n\n")
            current_number = 0
            i = 0
            while current_number < addr_count:
                file_write.write(
                    "const uint8_t "
                    + output_array
                    + "_"
                    + str(current_number)
                    + "[] = {\n"
                )
                current_number2 = 0
                while current_number2 < app_count_array[current_number]:
                    file_write.write(str(data_array[i]) + ",")
                    current_number2 += 1
                    i += 1
                    if current_number2 % 16 == 0:
                        file_write.write("\n")
                file_write.write("};\n\n")
                current_number += 1
            file_write.write(
                "const uint8_t *" + output_array + "_Ptr[" + str(addr_count) + "] = {\n"
            )
            current_number = 0
            while current_number < addr_count:
                file_write.write(
                    "    " + output_array + "_" + str(current_number) + ",\n"
                )
                current_number += 1
            file_write.write("};\n\n")
