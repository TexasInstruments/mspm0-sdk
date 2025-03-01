/* eslint-disable @typescript-eslint/no-explicit-any */
/* eslint-disable no-prototype-builtins */
import { Buffer } from 'buffer';
const createBuffer = (value, encoding) => Buffer.from(value, encoding);
export default createBuffer;
