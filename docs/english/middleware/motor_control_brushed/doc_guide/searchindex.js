Search.setIndex({"docnames": ["doc_guide-srcs/Brushed_Motor_Control_Quick_Start_Guide", "doc_guide-srcs/Brushed_Motor_Control_Software_Overview", "doc_guide-srcs/drv8706-q1/DRV8706-Q1_GUI_User_Guide", "doc_guide-srcs/drv8706-q1/DRV8706-Q1_Hardware_User_Guide", "doc_guide-srcs/index"], "filenames": ["doc_guide-srcs/Brushed_Motor_Control_Quick_Start_Guide.rst", "doc_guide-srcs/Brushed_Motor_Control_Software_Overview.rst", "doc_guide-srcs/drv8706-q1/DRV8706-Q1_GUI_User_Guide.rst", "doc_guide-srcs/drv8706-q1/DRV8706-Q1_Hardware_User_Guide.rst", "doc_guide-srcs/index.rst"], "titles": ["Quick Start Guide", "Library Software Overview", "DRV8706-Q1 EVM GUI User Guide", "DRV8706-Q1 EVM GUI Hardware Guide", "Brushed Motor Control User Guide"], "terms": {"softwar": [0, 4], "overview": [0, 4], "api": 0, "mspm0": [0, 1, 2], "launchpad": [0, 2, 4], "lp": [0, 1], "mspm0l1306": [0, 1], "us": [0, 1, 3, 4], "link": 0, "evm": [0, 4], "gui": [0, 1, 4], "user": [0, 1, 3], "hardwar": [0, 4], "devic": 0, "page": 0, "": [1, 2, 3], "brush": [1, 3], "control": [1, 2], "i": [1, 2, 3], "made": 1, "up": [1, 2], "two": [1, 2], "kei": [1, 2], "The": [1, 2, 3], "provid": 1, "applic": 1, "program": [1, 2], "interfac": [1, 2], "manipul": 1, "configur": [1, 2], "mcu": [1, 2, 3], "peripher": 1, "Then": [1, 2], "figur": 1, "diagram": 1, "show": [1, 2, 3], "how": [1, 2, 3], "interact": 1, "each": [1, 2, 3], "other": [1, 2, 3], "microcontrol": [1, 2], "abstract": 1, "layer": 1, "gener": 1, "were": 1, "develop": [1, 2], "allow": [1, 2, 3], "chang": [1, 2], "without": [1, 2], "requir": [1, 2, 3], "ani": [1, 2, 3], "code": 1, "purpos": 1, "meant": 1, "onli": [1, 2], "need": [1, 2, 4], "A": 1, "free": 1, "expand": 1, "support": [1, 2, 3, 4], "featur": 1, "its": [1, 2], "strongli": 1, "recommend": [1, 3], "modifi": [1, 2, 3], "exist": 1, "ensur": 1, "eas": 1, "migrat": 1, "new": 1, "version": 1, "thi": [1, 2], "No": 1, "specif": 1, "should": [1, 2, 3], "instead": 1, "treat": 1, "manner": 1, "For": [1, 3], "exampl": [1, 2, 3], "like": [1, 2], "ha": [1, 2], "nsleep": [1, 2, 3], "state": [1, 2], "gpio": [1, 2], "output": [1, 3, 4], "It": 1, "respons": [1, 2], "heavili": 1, "util": 1, "refer": 1, "differ": [1, 2], "As": [1, 2], "mention": 1, "abov": [1, 3], "dictat": 1, "which": [1, 2], "physic": [1, 2], "valu": [1, 2], "ar": [1, 2], "given": [1, 2], "an": [1, 2, 3], "let": [1, 2], "take": 1, "closer": 1, "look": [1, 2], "gpio_output_pin_2": 1, "repres": [1, 2], "5": [1, 4], "wai": 1, "determin": [1, 2], "mani": 1, "want": [1, 2, 3], "keep": 1, "mind": 1, "maximum": 1, "amount": 1, "thei": [1, 2], "map": 1, "snippet": 1, "from": [1, 3, 4], "now": 1, "gen_gpio_output_xxxx": 1, "defin": 1, "discuss": [1, 2], "later": 1, "note": [1, 2, 3], "those": 1, "driverlib": 1, "4": [1, 4], "inform": [1, 4], "hal_setgpiooutput": 1, "gpio_output_pin": 1, "Its": 1, "import": [1, 2], "here": [1, 2], "translat": 1, "call": 1, "intern": [1, 2], "structur": 1, "arrai": 1, "one": [1, 2, 3], "drv8706": [1, 4], "q1": [1, 4], "correspond": [1, 2], "nhiz1": [1, 3], "6": [1, 4], "final": [1, 2], "when": [1, 2], "while": [1, 2, 3], "make": [1, 2], "although": 1, "focus": 1, "concept": 1, "same": 1, "everi": [1, 2], "pwm": [1, 2], "adc": 1, "input": [1, 2], "spi": [1, 4], "whose": 1, "simplifi": 1, "tool": 1, "almost": 1, "all": [1, 2, 3], "sdk": 1, "document": [1, 2, 4], "onlin": 1, "goe": [1, 3], "depth": 1, "about": 1, "work": [1, 2], "But": 1, "why": 1, "you": [1, 2, 3], "see": 1, "initi": 1, "syconfig": 1, "object": 1, "easi": 1, "7": [1, 4], "contain": 1, "variabl": 1, "nhiz2": [1, 3], "done": 1, "below": [1, 2, 3], "8": 1, "common": 1, "target": 1, "also": 1, "foundat": 1, "build": 1, "top": [1, 3], "drive": [1, 2], "mode": [1, 4], "half": [1, 4], "bridg": [1, 4], "phase": [1, 4], "enabl": [1, 4], "switch": [1, 2], "multistep": 1, "process": 1, "therefor": [1, 2], "simpl": [1, 2], "perform": [1, 2], "step": [1, 2], "via": [1, 2, 3], "singl": [1, 2], "9": [1, 3], "regist": 1, "set": [1, 2, 3], "won": 1, "t": [1, 2], "ton": 1, "bitfield": 1, "howev": 1, "shown": [1, 3], "spiupdateregist": 1, "function": 1, "read": 1, "write": 1, "oper": [1, 4], "good": 1, "can": [1, 2, 3], "fit": 1, "If": [1, 2], "through": 1, "tend": 1, "veri": 1, "thank": 1, "complex": 1, "hidden": 1, "within": [1, 2, 3], "mean": [1, 2], "your": [1, 2, 3], "own": 1, "either": [1, 2], "becaus": 1, "doesn": 1, "have": [1, 2, 3], "someth": 1, "more": [1, 2], "tailor": 1, "flexibl": 1, "reusabl": 1, "graphic": 2, "wa": 2, "evalu": 2, "being": 2, "ti": 2, "go": 2, "over": 2, "part": 2, "run": [2, 3], "quickli": 2, "access": 2, "describ": 2, "view": 2, "learn": 2, "right": [2, 3], "must": [2, 3], "sleep": 2, "low": 2, "try": 2, "fail": 2, "result": 2, "garbag": 2, "data": 2, "put": 2, "than": 2, "1m": 2, "revert": 2, "default": 2, "onc": 2, "out": 2, "drvoff": [2, 3], "well": 2, "ic_ctrl": 2, "en_drv": 2, "gate": 2, "certain": 2, "automat": 2, "back": 2, "high": 2, "form": 2, "select": 2, "match": 2, "hookup": 2, "time": 2, "ie": 2, "disabl": 2, "even": 2, "thing": 2, "direct": 2, "duti": 2, "cycl": 2, "0": 2, "hi": 2, "z": 2, "To": [2, 3], "demo": 2, "first": 2, "follow": 2, "plug": 2, "usb": 2, "cabl": 2, "comput": 2, "launch": 2, "necessari": 2, "firmwar": 2, "everyth": 2, "success": 2, "bottom": 2, "left": 2, "corner": 2, "sai": 2, "scheme": 2, "drop": 2, "down": 2, "between": 2, "independ": 2, "frequenc": 2, "Will": 2, "nhizx": 2, "associ": 2, "imped": 2, "One": 2, "pvdd": [2, 3], "gnd": [2, 3], "what": [2, 4], "leav": 2, "unk": 2, "forc": 2, "remain": 2, "minim": 2, "number": 2, "timer": 2, "lp_mspm0l1306": 2, "hal": [2, 4], "modul": [2, 4], "both": [2, 3], "signal": 2, "channel": 2, "second": 2, "so": [2, 3], "notic": 2, "slider": 2, "vice": 2, "versa": 2, "forward": 2, "backward": 2, "toggl": 2, "spin": 2, "precaut": 2, "activ": 2, "freewheel": 2, "side": [2, 3], "brg_ctrl": 2, "brg_fw": 2, "power": [2, 4], "reset": 2, "whenev": 2, "return": 2, "flag": 2, "grei": 2, "hasn": 2, "occur": 2, "red": 2, "These": [2, 3], "led": 2, "updat": 2, "dure": 2, "doe": 2, "clear": 2, "do": 2, "manual": 2, "raw": 2, "click": 2, "nor": 2, "librari": [2, 4], "variou": 2, "avoid": 2, "base": 2, "specifi": 2, "dropdown": 2, "field": 2, "includ": 2, "0x": 2, "prefix": 2, "hit": 2, "safe": 2, "overwrit": 2, "posit": 2, "bit": 2, "mask": 2, "shift": 2, "actual": 2, "hex": 2, "enter": 2, "csa_ctrl": 2, "csa_blk": 2, "75": 2, "101b": 2, "sinc": 2, "max": 2, "sure": 2, "0x5": 2, "reflect": 2, "anytim": 2, "written": 2, "after": 2, "been": 2, "complet": 2, "graph": 2, "shunt": 2, "plot": 2, "refresh": 2, "latest": 2, "16": 2, "m": 2, "properli": 3, "solder": 3, "iron": 3, "remov": 3, "onboard": 3, "resistor": 3, "14x": 3, "femal": 3, "jumper": 3, "wire": 3, "dc": 3, "suppli": 3, "voltag": [3, 4], "current": [3, 4], "yellow": 3, "rectangl": 3, "desold": 3, "extern": 3, "tabl": 3, "some": 3, "adjust": 3, "3v3": 3, "dvdd": 3, "pa12": 3, "in1": 3, "en": 3, "pa13": 3, "j3": 3, "in2": 3, "ph": 3, "sw2": 3, "pa11": 3, "j14": 3, "sclk": 3, "pa5": 3, "sdi": 3, "pa10": 3, "sdo": 3, "pa15": 3, "j1": 3, "nsc": 3, "pa27": 3, "j13": 3, "pa24": 3, "j5": 3, "pa21": 3, "pa6": 3, "pa22": 3, "j6": 3, "pa17": 3, "nfault": 3, "varieti": 3, "depend": 3, "end": 3, "out1": 3, "out2": 3, "desir": 3, "vbat": 3, "connector": 3, "stai": 3, "v": 3, "37": 3, "satisfi": 3, "analog": 3, "rang": 3, "quick": 4, "start": 4, "1": 4, "2": 4, "driver": 4, "3": 4, "get": 4, "pin": 4, "button": 4, "indic": 4, "fault": 4, "statu": 4, "sens": 4, "amplifi": 4, "board": 4, "modif": 4, "connect": 4, "h": 4, "solenoid": 4}, "objects": {}, "objtypes": {}, "objnames": {}, "titleterms": {"quick": 0, "start": [0, 2], "guid": [0, 2, 3, 4], "1": [0, 1, 2, 3], "brush": [0, 4], "motor": [0, 1, 2, 3, 4], "librari": [0, 1], "document": 0, "2": [0, 1, 2, 3], "support": 0, "driver": [0, 1, 2, 3], "drv8706": [0, 2, 3], "q1": [0, 2, 3], "softwar": 1, "overview": [1, 2, 3], "hal": 1, "modul": 1, "enum": 1, "3": [1, 2, 3], "ti": 1, "sysconfig": 1, "pin": [1, 2, 3], "associ": 1, "api": 1, "creat": 1, "evm": [2, 3], "gui": [2, 3], "user": [2, 4], "us": 2, "inform": 2, "hardwar": [2, 3], "connect": [2, 3], "enabl": [2, 3], "spi": 2, "commun": 2, "output": 2, "4": [2, 3], "protect": 2, "get": 2, "mode": [2, 3], "half": [2, 3], "bridg": [2, 3], "h": [2, 3], "phase": [2, 3], "solenoid": [2, 3], "button": 2, "indic": 2, "5": [2, 3], "fault": 2, "statu": 2, "from": 2, "oper": 2, "6": [2, 3], "write": 2, "regist": 2, "bitfield": 2, "read": 2, "7": 2, "current": 2, "sens": 2, "amplifi": 2, "voltag": 2, "what": 3, "need": 3, "board": 3, "modif": 3, "launchpad": 3, "lp": 3, "mspm0l1306": 3, "control": [3, 4], "power": 3}, "envversion": {"sphinx.domains.c": 2, "sphinx.domains.changeset": 1, "sphinx.domains.citation": 1, "sphinx.domains.cpp": 6, "sphinx.domains.index": 1, "sphinx.domains.javascript": 2, "sphinx.domains.math": 2, "sphinx.domains.python": 3, "sphinx.domains.rst": 2, "sphinx.domains.std": 2, "sphinx": 56}})