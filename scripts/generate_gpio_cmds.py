#!/usr/bin/python3
import sys
import struct
import random

def generate_gpio_frames(num_frames, output_file="gpio_cmds.bin"):
    with open(output_file, "wb") as f:
        for _ in range(num_frames):
            gpio_byte = random.randint(0, 0xFF)  
            hold_us = random.randint(1, 5)  # 1 to 5s hold

            frame = struct.pack("<BI", gpio_byte, hold_us)
            f.write(frame)

    print("Wrote {0} GPIO frames to {1}".format(num_frames, output_file))

# Example usage
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: ./generate_gpio_cmds.py <No of frames> <output file>")
        sys.exit(1)

    num_frames = int(sys.argv[1])
    output_file = sys.argv[2]

    generate_gpio_frames(num_frames, output_file)

