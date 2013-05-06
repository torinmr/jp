#ifndef CONGESTION_CONTROL
#define CONGESTION_CONTROL

#include "fake_headers.h"

enum {
  LOSS_TYPE_FAST_RETRANSMIT = 0,
  LOSS_TYPE_TIMEOUT
};

int congestion_ack(struct fake_sock *sk, int num_bytes);
int congestion_loss(struct fake_sock *sk, int type);
int congestion_bytes_available(struct fake_sock *sk);

#endif
