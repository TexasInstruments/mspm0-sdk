const crc1 = (current, previous = 0) => {
    let crc = ~~previous;
    let accum = 0;
    for (let index = 0; index < current.length; index++) {
        accum += current[index];
    }
    crc += accum % 256;
    return crc % 256;
};
export default crc1;
