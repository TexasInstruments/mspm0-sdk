"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const crc32mpeg2_js_1 = __importDefault(require("./calculators/crc32mpeg2.js"));
const define_crc_js_1 = __importDefault(require("./define_crc.js"));
exports.default = (0, define_crc_js_1.default)('crc-32-mpeg', crc32mpeg2_js_1.default);
