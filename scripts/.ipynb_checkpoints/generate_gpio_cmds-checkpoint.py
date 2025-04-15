import struct
import random

def generate_gpio_frames(num_frames, output_file):
    with open(output_file, "wb") as f:
        for _ in range(num_frames):
            gpio_byte = random.randint(0, 0xFF)  # Random 8-bit GPIO value
            hold_us = random.randint(1000, 50000)  # e.g., 1ms to 50ms hold

            frame = struct.pack("<BI", gpio_byte, hold_us)
            f.write(frame)

    print(f"Wrote {num_frames} GPIO frames to '{output_file}'")

# Example usage
if __name__ == "__main__":
    generate_gpio_frames(num_frames=50, output_file="gpio_frames.bin")

