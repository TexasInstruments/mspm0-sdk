"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const crc16kermit_js_1 = __importDefault(require("./calculators/crc16kermit.js"));
const define_crc_js_1 = __importDefault(require("./define_crc.js"));
exports.default = (0, define_crc_js_1.default)('kermit', crc16kermit_js_1.default);
