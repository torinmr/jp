#ifndef SEGMENT_TABLE
#define SEGMENT_TABLE

#include "fake_headers.h"

int segment_table_register_bytes(struct fake_sock *conn_sk,
                                 struct fake_sock *flow_sk,
                                 int first, int last);
int segment_table_update(struct fake_sock *sk, struct sack_segment *sack);
void segment_table_print(struct fake_sock *sk);
#endif
