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
import tkinter
from tkinter import *
from tkinter.filedialog import *
import platform
from get_file import *
from BSL_pack import *
from UART_send import *
import time
import os
from txt_to_h import *
import threading
import subprocess
from subprocess import run

class Tkinter_app:
    def __init__(self, master):
        self.passwordfile = b""
        self.count = 0
        self.firmwaredfile = ""
        self.xds_v = tkinter.StringVar(None, "a")
        # self.xds_r = tkinter.StringVar(None, '1')
        menubar = Menu(master, tearoff=0)
        #        menubar.add_command(label='MoreOption')
        menufile = Menu(menubar, tearoff=0)
        menubar.add_cascade(label="MoreOption", menu=menufile)
        #        menufile.add_command(label='Create Linker Files', command=self.create_linker)
        menufile.add_command(label="TXT_to_H", command=self.txt_h)
        menufile.add_command(label="Update XDS110 firmware", command=self.update_xds110)
        menufile.add_command(label="Get_CRC", command=self.CRC_h)
        master["menu"] = menubar

        # menubar.config(bg='red')
        #   menubar.pack()

        # variable = StringVar(master)
        # variable.set("one")  # default value
        # w = OptionMenu(master, variable, "one", "two", "three")
        # w.config(bg="gray")  # Set background color to green
        # w.pack()

        frame0 = Frame(master)
        frame0.pack(padx=50, pady=20, anchor=E)
        frame1 = Frame(master)
        frame1.pack(padx=50, anchor=E)
        frame3 = Frame(master)
        frame3.pack(pady=10, fill=X)
        frame4 = Frame(master)
        frame4.pack()
        frame5 = Frame(master)
        frame5.pack()
        # frame6 = Frame(master)
        # frame6.pack(padx=1, pady=20, anchor=E)
        frame2 = Frame(master)
        frame2.pack(side="bottom")

        self.label0 = Label(frame0, text="Application firmware file:")
        self.label0.pack(side="left")
        global input_name
        input_name = StringVar()
        self.entry = Entry(frame0, width=50, textvariable=input_name)
        self.entry.pack(side="left")
        self.button = Button(frame0, text="Choose .txt file", command=self.choosefile)
        self.button.pack(side="left")

        self.label1 = Label(frame1, text="Password file:")
        self.label1.pack(side="left")
        global input_name1
        input_name1 = StringVar()
        self.entry1 = Entry(frame1, width=50, textvariable=input_name1)
        self.entry1.pack(side="left")
        self.button1 = Button(frame1, text="Choose .txt file", command=self.choosefile1)
        self.button1.pack(side="left")

        global photo
        #        photo = PhotoImage(file=SETUP_DIR + "\imag\oi.GIF")
        photo = PhotoImage(file="imag\oi.GIF")
        self.label2 = Label(frame2, image=photo)
        self.label2.pack()

        self.button3 = Button(frame3, text="Download", command=self.downlaod_thread)
        self.button3.pack()

        self.label3 = Label(frame3, text="(Download: Just support UART with XDS110)")
        self.label3.pack()

        self.rad_button = Radiobutton(
            frame3,
            text="XDS110 on Launchpad",
            variable=self.xds_v,
            value="a",
            command=self.xds110_LP,
        )
        self.rad_button.place(relx=0.7, rely=0)
        self.rad_button2 = Radiobutton(
            frame3,
            text="Standalone XDS110",
            variable=self.xds_v,
            value="b",
            command=self.xds110_S,
        )
        self.rad_button2.place(relx=0.7, rely=0.5)

        # self.rad_button3 = Radiobutton(frame3, text='BOOTRST', variable=self.xds_r, value='1', command=self.xds110_BR)
        # self.rad_button3.place(relx=0.1,rely=0)
        # self.rad_button4 = Radiobutton(frame3, text='POR', variable=self.xds_r, value='2', command=self.xds110_PR)
        # self.rad_button4.place(relx=0.1,rely=0.5)

        self.s1 = Scrollbar(frame4)
        self.s1.pack(side=RIGHT, fill=Y)
        self.textlog = Text(
            frame4, yscrollcommand=self.s1.set, width=70, height=15, bg="white"
        )
        self.s1.config(command=self.textlog.yview)
        self.textlog.pack()

        self.textlog.insert(INSERT, "Default hardware is XDS110 on Launchpad.\n")
        # self.textlog.insert(INSERT, 'Default reset type is boot reset.\n')
        #        self.textlog.insert(INSERT, "Python version: "+platform.python_version() + '\n')
        self.textlog.tag_config("error", foreground="red")
        self.textlog.tag_config("pass", foreground="green")
        self.textlog.tag_config("normal", foreground="black")
        self.textlog.config(state=DISABLED)

        self.button_c = Button(frame5, text="Clear", command=self.clear_text)
        self.button_c.pack()

        self.connection_pack = BSL_pack.connection_pack()
        self.get_ID_pack = BSL_pack.get_ID_pack()
        self.password_pack = b""
        self.mass_erase_pack = BSL_pack.mass_erase_pack()
        self.firmware_pack = b""
        self.start_app_pack = BSL_pack.start_app_pack()
        self.path = os.getcwd()

    def xds110_LP(self):
        self.textlog.config(state=NORMAL)
        self.textlog.insert(
            INSERT, "Changed the hardware bridge to XDS110 on Launchpad.\n", "normal"
        )
        self.textlog.config(state=DISABLED)

    def xds110_S(self):
        self.textlog.config(state=NORMAL)
        self.textlog.insert(
            INSERT, "Changed the hardware bridge to standalone XDS110.\n", "normal"
        )
        self.textlog.config(state=DISABLED)

    def xds110_BR(self):
        self.textlog.config(state=NORMAL)
        self.textlog.insert(INSERT, "Changed reset type to boot reset.\n", "normal")
        self.textlog.config(state=DISABLED)

    def xds110_PR(self):
        self.textlog.config(state=NORMAL)
        self.textlog.insert(INSERT, "Changed reset type to power on reset.\n", "normal")
        self.textlog.config(state=DISABLED)

    def choosefile(self):
        f = askopenfilename(
            title="Choose a firmware file",
            initialdir="c:",
            filetypes=[("textfile", ".txt")],
        )
        input_name.set(f)
        self.textlog.config(state=NORMAL)
        if f:
            self.textlog.insert(
                INSERT, "Choose a firmware file at:" + f + "\n", "normal"
            )
            self.firmwaredfile = file_d.get_firmware(f)
            self.firmware_pack = BSL_pack.firmware_pack(self.firmwaredfile)
        else:
            self.textlog.insert(
                INSERT, "Error: Please choose a firmware file.\n", "error"
            )
            self.firmwaredfile = ""
            self.firmware_pack = b""
        self.textlog.see(END)
        self.textlog.config(state=DISABLED)

    def choosefile1(self):
        f1 = askopenfilename(
            title="Choose a password file",
            initialdir="c:",
            filetypes=[("textfile", ".txt")],
        )
        input_name1.set(f1)
        self.textlog.config(state=NORMAL)
        if f1:
            self.textlog.insert(
                INSERT, "Choose a password file at:" + f1 + "\n", "normal"
            )
            self.passwordfile = b""
            self.passwordfile = file_d.get_password(f1)
            if self.passwordfile == b"":
                self.textlog.insert(
                    INSERT, "Error: Password format is not correct!\n", "error"
                )
            else:
                self.password_pack = BSL_pack.password_pack(self.passwordfile)
        else:
            self.passwordfile = b""
            self.textlog.insert(
                INSERT, "Error: Please choose a password file.\n", "error"
            )

        # else:
        #     print(self.passwordfile)
        self.textlog.see(END)
        self.textlog.config(state=DISABLED)
    def downlaod_thread(self):
        T = threading.Thread(target=self.download, args=())
        T.start()
    def download(self):
        self.textlog.config(state=NORMAL)
        self.button3.config(state='disabled')
        if self.passwordfile != b"" and self.firmwaredfile != "":
            if self.xds_v.get() == "a":
                try:
                    subprocess.run(
                        self.path
                        + "/common/uscif/dbgjtag.exe  -f @xds110 -Y gpiopins, config=0x1, write=0x1",
                        shell=True,
                        capture_output=True,
                        encoding='utf-8')
                    subprocess.run(
                        self.path
                        + "/common/uscif/xds110/xds110reset.exe -d 1400",
                        shell=True,
                        capture_output=True,
                        encoding='utf-8')
                except:
                    self.textlog.insert(
                        INSERT,
                        "Error: please make sure the folder path not include !\n",
                        "error",
                    )
            else:
                if self.xds_v.get() == "b":
                    subprocess.run(
                        self.path
                        + "/common/uscif/dbgjtag.exe -f @xds110 -Y power,supply=on,voltage=3.2",
                        shell=True,
                        capture_output=True,
                        encoding='utf-8')
                    subprocess.run(
                        self.path
                        + "/common/uscif/dbgjtag.exe -f @xds110 -Y gpiopins, config=0x3, write=0x02",
                        shell=True,
                        capture_output=True,
                        encoding='utf-8')
                    time.sleep(1.4)
                    subprocess.run(
                        self.path
                        + "/common/uscif/dbgjtag.exe -f @xds110 -Y gpiopins, config=0x3, write=0x03",
                        shell=True,
                        capture_output=True,
                        encoding='utf-8')
                else:
                    # print(self.xds_v.get())
                    self.textlog.insert(
                        INSERT, "No correct hardware bridge selected.\n", "error"
                    )
            find_flag = UART_S.find_MSP_COM()
            if find_flag:
                self.textlog.insert(
                    INSERT, "Find MSP COM port:" + find_flag + "\n", "normal"
                )
                self.textlog.see(END)
                ser_port = UART_S.config_uart(find_flag)
                self.textlog.insert(
                    INSERT,
                    "Configure UART: 9600 baudrate, 8 data bits (LSB first), no parity, and 1 stop bit.\n",
                    "normal",
                )
                self.textlog.see(END)
                UART_S.send_data(ser_port, self.connection_pack)
                response_ = UART_S.read_data(ser_port, 1)
                if self.xds_v.get() == "a":
                    subprocess.run(
                        self.path
                        + "/common/uscif/dbgjtag.exe  -f @xds110 -Y gpiopins, config=0x1, write=0x0",
                        shell=True,
                        capture_output=True,
                        encoding='utf-8')
                else:
                    if self.xds_v.get() == "b":
                        subprocess.run(
                            self.path
                            + "/common/uscif/dbgjtag.exe -f @xds110 -Y gpiopins, config=0x3, write=0x01",
                            shell=True,
                            capture_output=True,
                            encoding='utf-8')
                # print(type(response))
                # print(response)
                UART_S.send_data(ser_port, b"\xbb")
                response01 = UART_S.read_data(ser_port, 1)
                #                print(response)
                if response01 == "51":
                    self.textlog.insert(
                        INSERT, "MSPM0 is in BSL mode.\nGet device ID...\n", "normal"
                    )
                    self.textlog.see(END)
                    UART_S.send_data(ser_port, self.get_ID_pack)
                    response1 = UART_S.read_data(ser_port, 33)
                    self.textlog.insert(INSERT, "Send the password...\n", "normal")
                    self.textlog.see(END)
                    UART_S.send_data(ser_port, self.password_pack)
                    response2 = UART_S.read_data(ser_port, 1)
                    check = self.check_pack(response2)
                    if check:
                        response2 = UART_S.read_data(ser_port, 9)
                        check2 = self.check_reponse(response2[8:10])
                        # print(response2[8:10])
                        if check2:
                            self.textlog.insert(INSERT, "Mass erase...\n", "normal")
                            self.textlog.see(END)
                            UART_S.send_data(ser_port, self.mass_erase_pack)
                            response2 = UART_S.read_data(ser_port, 1)
                            response2 = UART_S.read_data(ser_port, 9)
                            self.textlog.insert(
                                INSERT, "Send the firmware...\n", "normal"
                            )
                            self.textlog.see(END)
                            # print(type(firmware_pack))
                            # print(firmware_pack)
                            for list_code in self.firmware_pack:
                                UART_S.send_data(ser_port, list_code)
                                response3 = UART_S.read_data(ser_port, 1)
                                self.count = self.count + 1
                                check = self.check_pack(response3)
                                if check:
                                    response3 = UART_S.read_data(ser_port, 9)
                                    check3 = self.check_reponse(response3[8:10])
                                    if check3:
                                        pass
                                    else:
                                        break
                                else:
                                    break
                            if check:
                                self.textlog.insert(
                                    INSERT, "Send firmware successfully!\n", "normal"
                                )
                                self.textlog.insert(
                                    INSERT,
                                    "Boot reset the device to start application ...\n",
                                    "normal",
                                )
                                self.textlog.insert(
                                    INSERT,
                                    "-----------Download finished!----------\n",
                                    "pass",
                                )
                                self.textlog.see(END)
                                UART_S.send_data(ser_port, self.start_app_pack)
                                response3 = UART_S.read_data(ser_port, 1)
                    else:
                        self.textlog.insert(INSERT, "Error: No response！\n", "error")
                        self.textlog.see(END)
                else:
                    self.textlog.insert(INSERT, "Error: No response！\n", "error")
                    self.textlog.see(END)
            else:
                self.textlog.insert(
                    INSERT, "Error: Can not find MSP COM port!\n", "error"
                )
                self.textlog.see(END)
        else:
            self.textlog.insert(
                INSERT, "Error: please choose all files above!\n", "error"
            )
        self.textlog.see(END)
        self.textlog.config(state=DISABLED)
        self.button3.config(state='normal')

    def clear_text(self):
        self.textlog.config(state=NORMAL)
        self.textlog.delete("2.0", "end")
        self.textlog.insert(INSERT, "\n")
        self.textlog.config(state=DISABLED)

    def check_pack(self, pack_ack):
        flagg = 0
        #        self.textlog.config(state=NORMAL)
        if pack_ack == "00":
            flagg = 1
            self.textlog.insert(INSERT, '[Firmware update on going...] Send firmware package ' + str(self.count) + ' successfully!\n', "normal")
        elif pack_ack == "51":
            self.textlog.insert(INSERT, "Error: Header incorrect!\n", "error")
        elif pack_ack == "52":
            self.textlog.insert(INSERT, "Error: Checksum incorrect!\n", "error")
        elif pack_ack == "53":
            self.textlog.insert(INSERT, "Error: Packet size zero!\n", "error")
        elif pack_ack == "54":
            self.textlog.insert(INSERT, "Error: Packet size too big!\n", "error")
        elif pack_ack == "55":
            self.textlog.insert(INSERT, "Error: Unknown error!\n", "error")
        elif pack_ack == "56":
            self.textlog.insert(INSERT, "Error: Unknown baud rate!\n", "error")
        else:
            self.textlog.insert(INSERT, "Error: Unknow else error!\n", "error")
        #       self.textlog.config(state=DISABLED)
        self.textlog.see(END)
        return flagg

    def check_reponse(self, pack_res):
        flagg = 0
        #       self.textlog.config(state=NORMAL)
        if pack_res == "00":
            flagg = 1
            self.textlog.insert(INSERT, "Operation success!\n", "normal")
        elif pack_res == "01":
            self.textlog.insert(INSERT, "Error: flash program failed!\n", "error")
        elif pack_res == "02":
            self.textlog.insert(INSERT, "Error: Mass Erase failed!\n", "error")
        elif pack_res == "04":
            self.textlog.insert(INSERT, "Error: BSL locked!!\n", "error")
        elif pack_res == "05":
            self.textlog.insert(INSERT, "Error: BSL password error!\n", "error")
        elif pack_res == "06":
            self.textlog.insert(
                INSERT, "Error: Multiple BSL password error!\n", "error"
            )
        elif pack_res == "07":
            self.textlog.insert(INSERT, "Error: Unknown Command!\n", "error")
        elif pack_res == "08":
            self.textlog.insert(INSERT, "Error: Invalid memory range!\n", "error")
        elif pack_res == "0B":
            self.textlog.insert(INSERT, "Error: Factory reset disabled!\n", "error")
        elif pack_res == "0C":
            self.textlog.insert(
                INSERT, "Error: Factory reset password error!\n", "error"
            )
        else:
            self.textlog.insert(INSERT, "Error: Unknow else error!\n", "error")
        self.textlog.see(END)
        return flagg

    #        self.textlog.config(state=DISABLED)

    def txt_h(self):
        sub_win1 = Toplevel(root)
        sub_win1.title("TXT to H")
        #        sub_win1.attributes("-topmost", True)
        sub_win1.geometry("700x350+300+200")
        sub_win1.grab_set()
        frames_0 = Frame(sub_win1)
        frames_0.pack(padx=50, pady=20, anchor=W)
        frames_1 = Frame(sub_win1)
        frames_1.pack(padx=50, anchor=W)
        frames_3 = Frame(sub_win1)
        frames_3.pack(pady=10)
        frames_4 = Frame(sub_win1)
        frames_4.pack()

        self.labelss0 = Label(frames_0, text="Choose a firmware .txt file")
        self.labelss0.pack(side="left")
        global input_name_ss
        input_name_ss = StringVar()
        self.entryss = Entry(frames_0, width=50, textvariable=input_name_ss)
        self.entryss.pack(side="left")
        self.buttonss = Button(
            frames_0, text="Choose .txt file", command=self.choosetxtfile
        )
        self.buttonss.pack(side="left")

        self.labelss1 = Label(frames_1, text="Choose a ouput folder:")
        self.labelss1.pack(side="left")
        global out_name_ss
        out_name_ss = StringVar()
        self.entryss1 = Entry(frames_1, width=50, textvariable=out_name_ss)
        self.entryss1.pack(side="left")
        self.buttonss1 = Button(frames_1, text="Scan", command=self.choosefile_out)
        self.buttonss1.pack(side="left")

        self.buttonss2 = Button(frames_3, text="Convert", command=self.convert_)
        self.buttonss2.pack()

        self.s3 = Scrollbar(frames_4)
        self.s3.pack(side=RIGHT, fill=Y)
        self.textlogsubs = Text(
            frames_4, yscrollcommand=self.s3.set, width=70, height=10, bg="white"
        )
        self.s3.config(command=self.textlogsubs.yview)
        self.textlogsubs.pack()
        self.textlogsubs.tag_config("errors_", foreground="red")
        self.textlogsubs.tag_config("pass_s_", foreground="green")
        self.textlogsubs.insert(
            INSERT, "This function is used for the situation that using MCU as host.\n"
        )
        self.textlogsubs.insert(
            INSERT, "The output header file is used for host MCU.\n"
        )
        self.textlogsubs.config(state=DISABLED)

    def choosetxtfile(self):
        fs = askopenfilename(
            title="Choose a firmware file",
            initialdir="c:",
            filetypes=[("textfile", ".txt")],
        )
        input_name_ss.set(fs)
        self.textlogsubs.config(state=NORMAL)
        if fs:
            self.textlogsubs.insert(
                INSERT, "Choose a firmware file at:" + fs + "\n", "normal"
            )
        else:
            self.textlogsubs.insert(
                INSERT, "Error: Please choose a firmware file.\n", "errors_"
            )
        self.textlogsubs.see(END)
        self.textlogsubs.config(state=DISABLED)

    def choosefile_out(self):
        f3 = askdirectory()
        out_name_ss.set(f3)
        self.textlogsubs.config(state=NORMAL)
        if f3:
            self.textlogsubs.insert(INSERT, "Choose a output folder:" + f3 + "\n")
        else:
            self.textlogsubs.insert(
                INSERT, "Error: Please choose a output folder.\n", "errors_"
            )
        self.textlogsubs.see(END)
        self.textlogsubs.config(state=DISABLED)

    def convert_(self):
        self.textlogsubs.config(state=NORMAL)
        input_names = input_name_ss.get()
        output_paths = out_name_ss.get()
        if input_names:
            if output_paths:
                self.textlogsubs.insert(INSERT, "Converting...\n")
                name_file = input_names.split("/")[-1]
                name_file2 = name_file.split(".")[0]
                output_paths_n = output_paths + "/" + name_file2 + ".h"
                Conver_F.conver_fun(input_names, output_paths_n)
                self.textlogsubs.insert(
                    INSERT,
                    "-----Convert the firmware to header file named "
                    + name_file2
                    + ".h!----\n ",
                    "pass_s_",
                )
            else:
                self.textlogsubs.insert(
                    INSERT, "Error: Please choose a output folder.\n", "errors_"
                )
        else:
            self.textlogsubs.insert(
                INSERT, "Error: Please choose a .txt firmware.\n", "errors_"
            )
        self.textlogsubs.see(END)
        self.textlogsubs.config(state=DISABLED)

    def update_xds110(self):
        self.textlog.config(state=NORMAL)
        self.textlog.insert(
            INSERT, "Update the XDS110 firmware to version firmware_3.0.0.28...\n"
        )
        path = os.getcwd()
        print(path)
        # os.system(path + "/common/uscif/xds110/xdsdfu.exe -m")
        subprocess.run(
            path
            + "/common/uscif/xds110/xdsdfu.exe -m",
            shell=True,
            capture_output=True,
            encoding='utf-8')
        time.sleep(0.5)
        # print(time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(time.time())))
        # os.system(
        #     path
        #     + "/common/uscif/xds110/xdsdfu.exe -f "
        #     + path
        #     + "/common/uscif/xds110/firmware_3.0.0.28.bin -r"
        # )
        subprocess.run(
            path
            + "/common/uscif/xds110/xdsdfu.exe -f "
            + path
            + "/common/uscif/xds110/firmware_3.0.0.28.bin -r",
            shell=True,
            capture_output=True,
            encoding='utf-8')
        # print(time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(time.time())))
        time.sleep(2)
        self.textlog.insert(INSERT, "XDS110 firmware update finished.\n", "pass")
        self.textlog.see(END)
        self.textlog.config(state=DISABLED)

    def CRC_h(self):
        sub_win2 = Toplevel(root)
        sub_win2.title("Get CRC")
        #        sub_win1.attributes("-topmost", True)
        sub_win2.geometry("700x350+300+200")
        sub_win2.grab_set()
        frames_CRC_0 = Frame(sub_win2)
        frames_CRC_0.pack(padx=50, pady=20, anchor=W)
        frames_CRC_1 = Frame(sub_win2)
        frames_CRC_1.pack(padx=50, anchor=W)
        frames_CRC_3 = Frame(sub_win2)
        frames_CRC_3.pack(pady=10)
        frames_CRC_4 = Frame(sub_win2)
        frames_CRC_4.pack()

        self.labelss_CRC_0 = Label(frames_CRC_0, text="Choose a firmware .txt file")
        self.labelss_CRC_0.pack(side="left")
        global input_name_ss_CRC
        input_name_ss_CRC = StringVar()
        self.entryss_CRC = Entry(frames_CRC_0, width=50, textvariable=input_name_ss_CRC)
        self.entryss_CRC.pack(side="left")
        self.buttonss_CRC = Button(
            frames_CRC_0, text="Choose .txt file", command=self.choosetxtfile_crc
        )
        self.buttonss_CRC.pack(side="left")

        self.labelss3_CRC = Label(frames_CRC_1, text="CRC Start Address:")
        self.labelss3_CRC.pack(side="left")
        global out_crc_star
        out_crc_star = StringVar()
        out_crc_star = ""
        self.entryss3_crc = Entry(frames_CRC_1, width=20)
        self.entryss3_crc.pack(side="left")
        self.entryss3_crc.delete(0, "end")
        self.entryss3_crc.insert(INSERT, out_crc_star)
        self.entryss3_crc["state"] = "readonly"

        self.labelss1_CRC = Label(frames_CRC_1, text="CRC Length:")
        self.labelss1_CRC.pack(side="left")
        global out_crc_len
        out_crc_len = StringVar()
        out_crc_len = ""
        self.entryss1_crc = Entry(frames_CRC_1, width=20)
        self.entryss1_crc.pack(side="left")
        self.entryss1_crc.delete(0, "end")
        self.entryss1_crc.insert(INSERT, out_crc_len)
        self.entryss1_crc["state"] = "readonly"

        self.labelss2_CRC = Label(frames_CRC_1, text="CRC Result:")
        self.labelss2_CRC.pack(side="left")
        global out_crc_result
        out_crc_result = StringVar()
        out_crc_result = ""
        self.entryss2_crc = Entry(frames_CRC_1, width=20)
        self.entryss2_crc.pack(side="left")
        self.entryss2_crc.delete(0, "end")
        self.entryss2_crc.insert(INSERT, out_crc_result)
        self.entryss2_crc["state"] = "readonly"

        self.buttonss2_c = Button(frames_CRC_3, text="Generate", command=self.gen_crc)
        self.buttonss2_c.pack()

        self.s3_crc = Scrollbar(frames_CRC_4)
        self.s3_crc.pack(side=RIGHT, fill=Y)
        self.textlogsubs_crc = Text(
            frames_CRC_4,
            yscrollcommand=self.s3_crc.set,
            width=70,
            height=10,
            bg="white",
        )
        self.s3_crc.config(command=self.textlogsubs_crc.yview)
        self.textlogsubs_crc.pack()
        self.textlogsubs_crc.tag_config("errors_", foreground="red")
        self.textlogsubs_crc.tag_config("pass_s_", foreground="green")
        self.textlogsubs_crc.insert(
            INSERT, "This function is used for generate the CRC results.\n"
        )
        self.textlogsubs_crc.insert(
            INSERT,
            "Note: it just can calculate the CRC at first section that the contents under first @address\n",
        )
        self.textlogsubs_crc.config(state=DISABLED)

    def gen_crc(self):
        self.textlogsubs_crc.config(state=NORMAL)
        input_names_crc = input_name_ss_CRC.get()
        if input_names_crc:
            self.textlogsubs_crc.insert(INSERT, "Generating...\n")
            self.gen_crc_fun(input_names_crc)
            self.textlogsubs_crc.insert(
                INSERT, "-----Generate the CRC -----\n ", "pass_s_"
            )
        else:
            self.textlogsubs_crc.insert(
                INSERT, "Error: Please choose a .txt firmware.\n", "errors_"
            )
        self.textlogsubs_crc.see(END)
        self.textlogsubs_crc.config(state=DISABLED)

    def choosetxtfile_crc(self):
        fs_c = askopenfilename(
            title="Choose a firmware file",
            initialdir="c:",
            filetypes=[("textfile", ".txt")],
        )
        input_name_ss_CRC.set(fs_c)
        self.textlogsubs_crc.config(state=NORMAL)
        if fs_c:
            self.textlogsubs_crc.insert(
                INSERT, "Choose a firmware file at:" + fs_c + "\n", "normal"
            )
        else:
            self.textlogsubs.insert(
                INSERT, "Error: Please choose a firmware file.\n", "errors_"
            )
        self.textlogsubs_crc.see(END)
        self.textlogsubs_crc.config(state=DISABLED)

    def gen_crc_fun(self, file430):
        data_array = []
        flag = 0
        sizecount = 0
        bytes_buf = b""
        bytes_buf1 = ""
        with open(file430) as file_object:
            lines = file_object.readlines()
        for line in lines:
            if line[0] == "@":
                flag += 1
                if flag == 1:
                    line = line.rstrip()
                    bytes_buf1 = "0x" + line[1:]
                    # print(bytes_buf1)
            else:
                if flag == 1 and line[0] != "q":
                    line2 = "0x" + line
                    # 			print(isinstance(line, str))
                    line2 = line.replace(" \n", "\n")
                    line2 = line.replace(" ", " 0x")
                    # 			print(line)
                    sizecount += line2.count("0x")
                    data_array2 = line2.split()
                    # 			data_array2.pop()
                    data_array += data_array2
                    bytes_buf += bytes.fromhex(line)
        checksum = self.crc32_(bytes_buf)
        self.entryss3_crc["state"] = "normal"
        self.entryss3_crc.delete(0, "end")
        self.entryss3_crc.insert(INSERT, bytes_buf1)
        self.entryss3_crc["state"] = "readonly"
        self.entryss1_crc["state"] = "normal"
        self.entryss1_crc.delete(0, "end")
        self.entryss1_crc.insert(INSERT, hex(sizecount))
        self.entryss1_crc["state"] = "readonly"
        self.entryss2_crc["state"] = "normal"
        self.entryss2_crc.delete(0, "end")
        self.entryss2_crc.insert(INSERT, hex(checksum))
        self.entryss2_crc["state"] = "readonly"
        # print(hex(checksum))

    def crc32_(self, data_B):
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


if __name__ == "__main__":
    file_d = Get_files()
    BSL_pack = BSL_Pack()
    UART_S = UART_send()
    Conver_F = TXT_to_h()
    root = Tk()
    root.iconbitmap("imag/Capture.ico")
    root.geometry("700x520+500+500")
    root.title("MSPM0 Bootloader GUI  v1.2")
    app = Tkinter_app(root)
    root.mainloop()
