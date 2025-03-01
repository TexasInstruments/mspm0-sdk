/// <reference types="node" />
import { Buffer } from 'buffer';
import { BufferInput } from './types.js';
declare const createBuffer: (value: BufferInput, encoding?: BufferEncoding) => Buffer;
export default createBuffer;
