import serial

PCKT_START = bytes([0xC1, 0x1F, 0xFC, 0xC1])
PCKT_SIZE  = 256-len(PCKT_START)

with open("trace", "wb") as fhandle:
    with serial.Serial("/dev/serial/by-id/usb-Raspberry_Pi_Pico_E66160F4232E5737-if00", 115200, timeout=None) as serdev:
        while True:
            serdev.read_until(PCKT_START)
            pckt = serdev.read(PCKT_SIZE)

            fhandle.write(PCKT_START)
            fhandle.write(pckt)
            print("Read 1 packet")
