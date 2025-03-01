"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.crcjam = exports.crc32mpeg2 = exports.crc32 = exports.crc24 = exports.crc16kermit = exports.crc16xmodem = exports.crc16modbus = exports.crc16ccitt = exports.crc16 = exports.crc8dvbs2 = exports.crc81wire = exports.crc8 = exports.crc1 = void 0;
const crc1_js_1 = __importDefault(require("./crc1.js"));
exports.crc1 = crc1_js_1.default;
const crc8_js_1 = __importDefault(require("./crc8.js"));
exports.crc8 = crc8_js_1.default;
const crc81wire_js_1 = __importDefault(require("./crc81wire.js"));
exports.crc81wire = crc81wire_js_1.default;
const crc8dvbs2_js_1 = __importDefault(require("./crc8dvbs2.js"));
exports.crc8dvbs2 = crc8dvbs2_js_1.default;
const crc16_js_1 = __importDefault(require("./crc16.js"));
exports.crc16 = crc16_js_1.default;
const crc16ccitt_js_1 = __importDefault(require("./crc16ccitt.js"));
exports.crc16ccitt = crc16ccitt_js_1.default;
const crc16modbus_js_1 = __importDefault(require("./crc16modbus.js"));
exports.crc16modbus = crc16modbus_js_1.default;
const crc16xmodem_js_1 = __importDefault(require("./crc16xmodem.js"));
exports.crc16xmodem = crc16xmodem_js_1.default;
const crc16kermit_js_1 = __importDefault(require("./crc16kermit.js"));
exports.crc16kermit = crc16kermit_js_1.default;
const crc24_js_1 = __importDefault(require("./crc24.js"));
exports.crc24 = crc24_js_1.default;
const crc32_js_1 = __importDefault(require("./crc32.js"));
exports.crc32 = crc32_js_1.default;
const crc32mpeg2_js_1 = __importDefault(require("./crc32mpeg2.js"));
exports.crc32mpeg2 = crc32mpeg2_js_1.default;
const crcjam_js_1 = __importDefault(require("./crcjam.js"));
exports.crcjam = crcjam_js_1.default;
exports.default = {
    crc1: crc1_js_1.default,
    crc8: crc8_js_1.default,
    crc81wire: crc81wire_js_1.default,
    crc8dvbs2: crc8dvbs2_js_1.default,
    crc16: crc16_js_1.default,
    crc16ccitt: crc16ccitt_js_1.default,
    crc16modbus: crc16modbus_js_1.default,
    crc16xmodem: crc16xmodem_js_1.default,
    crc16kermit: crc16kermit_js_1.default,
    crc24: crc24_js_1.default,
    crc32: crc32_js_1.default,
    crc32mpeg2: crc32mpeg2_js_1.default,
    crcjam: crcjam_js_1.default,
};
