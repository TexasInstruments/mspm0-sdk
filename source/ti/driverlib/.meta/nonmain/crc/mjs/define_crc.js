import createBuffer from './create_buffer.js';
export default function defineCrc(model, calculator) {
    const result = (value, previous) => calculator(createBuffer(value), previous) >>> 0;
    result.signed = (value, previous) => calculator(createBuffer(value), previous);
    result.unsigned = result;
    result.model = model;
    return result;
}
