# PS Vita Stream — MrWrack v0.2 Wi-Fi

Low-latency LAN/Wi-Fi prototype.

## Design
PS Vita -> local Wi-Fi -> Windows PC

- UDP video transport: no retransmit queue.
- Every frame has a sequence number and timestamp.
- Frames are split into MTU-friendly chunks.
- PC keeps only the newest complete frame.
- Missing/incomplete old frames are discarded.
- No intentional multi-frame playback buffer.
- Native Vita capture target: 960x544.
- 1920x1080 is performed on the PC as an upscale.

## Run
PC: `VitaStreamPC.exe 5000`

Vita: set `PC_IP` in `vita/src/config.h` to your PC's local IPv4 address, then build/install the VPK.

Made by MrWrack
