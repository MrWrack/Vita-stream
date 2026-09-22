# VSTR Wi-Fi protocol v2

Each UDP datagram contains magic VSTR, protocol version 2, frame ID, timestamp, dimensions, pitch, pixel format, chunk index/count, payload size and payload.

The receiver drops stale/incomplete frames instead of waiting for retransmission to minimize latency.
