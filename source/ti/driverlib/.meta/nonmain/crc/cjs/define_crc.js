"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const create_buffer_js_1 = __importDefault(require("./create_buffer.js"));
function defineCrc(model, calculator) {
    const result = (value, previous) => calculator((0, create_buffer_js_1.default)(value), previous) >>> 0;
    result.signed = (value, previous) => calculator((0, create_buffer_js_1.default)(value), previous);
    result.unsigned = result;
    result.model = model;
    return result;
}
exports.default = defineCrc;
