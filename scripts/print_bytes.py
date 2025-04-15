#!/usr/bin/python3 

import sys

def print_gpio_frames(filepath):
    with open(filepath, "rb") as f:
        frame_index = 0
        while True:
            frame_bytes = f.read(5)
            if len(frame_bytes) < 5:
                break  # End of file

            gpio_byte = frame_bytes[0]
            hold_us = int.from_bytes(frame_bytes[1:], byteorder="little", signed=False)

            # Print raw bytes and decima; values
            hex_bytes = ' '.join('{0:02X}.format(b)' for b in frame_bytes)
            print("Frame {0}: GPIO: 0b{1:08b} | Hold: {2} s".format(frame_index, gpio_byte, hold_us))

            frame_index += 1

if __name__ == "__main__":
    file = sys.argv[1]
    print_gpio_frames(file)
