"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
/* eslint-disable @typescript-eslint/no-explicit-any */
/* eslint-disable no-prototype-builtins */
const buffer_1 = require("buffer");
const createBuffer = (value, encoding) => buffer_1.Buffer.from(value, encoding);
exports.default = createBuffer;
