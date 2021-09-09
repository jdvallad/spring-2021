#include "io.h"

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

// buffer and index to keep track of write_code().
static uint8_t code_buffer[BLOCK];
static uint32_t code_index = 0;
// statistic variables to track compression data.
extern uint64_t bytes_read;
extern uint64_t bytes_written;

// using the file descriptor infile, attempts to read nbytes number of bytes into the byte pointer
// buf. Returns the total number of bytes that were read. If number of bytes read is less than the
// number of bytes that was requested, this typically means that the end of file was reached.
int read_bytes(int infile, uint8_t *buf, int nbytes) {
    int count = 0, total = 0;
    do {
        count = read(infile, buf, nbytes);
        bytes_read += count;
        total += count;
        buf += count;
        nbytes -= count;
    } while (count != 0);
    return total;
}

// using the file descriptor infile, attempts to write nbytes number of bytes from the byte pointer
// buf. Returns the total number of bytes that were written. If number of bytes written is less than
// the number of bytes that was requested, this typically means that some sort of error occurred.
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    int count = 0, total = 0;
    do {
        count = write(outfile, buf, nbytes);
        bytes_written += count;
        total += count;
        buf += count;
        nbytes -= count;
    } while (count != 0);
    return total;
}

// will return the next bit from the file described by the file descriptor infile. Behavior only
// works as intended if repeated calls to read_bit() are made.
//
// Returns whether or not there are still more bits to be read from the file.
bool read_bit(int infile, uint8_t *bit) {
    static uint8_t buffer[BLOCK];
    static uint32_t index = 0;
    static uint32_t count = BLOCK;
    if (index == 0) {
        count = read_bytes(infile, buffer, BLOCK);
        *bit = (buffer[index / 8] >> (index % 8)) & 1UL;
        index++;
        if (count == 0) {
            count = BLOCK;
            return false;
        }
        return true;
    }
    *bit = (buffer[index / 8] >> (index % 8)) & 1UL;
    index++;
    if (index == 8 * count) {
        count = read_bytes(infile, buffer, BLOCK);
        index = 0;
        if (count == 0) {
            count = BLOCK;
            return false;
        }
    }
    return true;
}
// sets the indexth bit in input_byte to input_bit
void set_bit(uint8_t *input_byte, uint8_t input_bit, uint32_t index) {
    *input_byte = (input_bit) ? (*input_byte | (1 << index)) : (*input_byte & (~(1 << index)));
    return;
}
// writes all of the bits in Code c to outfile. This is done with a buffer once enough calls to
// write_code() have been made.
void write_code(int outfile, Code *c) {
    for (uint32_t i = 0; i < code_size(c); i++) {
        set_bit(&code_buffer[code_index / 8], code_get_bit(c, i), code_index % 8);
        code_index++;
        if (code_index == 8 * BLOCK) {
            write_bytes(outfile, code_buffer, BLOCK);
            code_index = 0;
        }
    }
    return;
}
// writes the remaining codes from code_buffer to the file. This should be called once all codes
// have been written and you would like any that are still sitting in the buffer to be written.
void flush_codes(int outfile) {
    if (code_index != 0) {
        for (uint32_t i = code_index; i < code_index + 8; i++) {
            set_bit(&code_buffer[i / 8], 0, i % 8);
        }
        write_bytes(outfile, code_buffer, 1 + (code_index / 8));
        code_index = 0;
    }
    return;
}
