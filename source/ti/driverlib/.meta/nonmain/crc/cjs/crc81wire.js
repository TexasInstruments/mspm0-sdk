"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const crc81wire_js_1 = __importDefault(require("./calculators/crc81wire.js"));
const define_crc_js_1 = __importDefault(require("./define_crc.js"));
exports.default = (0, define_crc_js_1.default)('dallas-1-wire', crc81wire_js_1.default);
