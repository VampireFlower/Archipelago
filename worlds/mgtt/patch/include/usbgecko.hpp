#include <types.h>

#pragma once

// Checks if there is a USB Gecko on EXI channel `chan`
bool ugProbe(int chan);

// Try to send `len` bytes from `data` via the USB Gecko on channel 1.
// Returns number of bytes sent or negative on error.
int send(const void * data, int len);

// Try to receive `len` bytes into `data` via the USB Gecko on channel 1.
// Returns number of bytes received or negative on error.
int recv(void * data, int len);

// Blocking. Busy loops until sufficient data has arrived
void ugRecvAll(void * data, int len);