#ifndef FAKE_HEADERS
#define FAKE_HEADERS

#define ALPHA_SCALE_FACTOR 9

struct table_entry {
  struct fake_sock *flow;
  int first;
  int last;
  int dup_acks;
  int timeout;
  struct table_entry *next;
};

struct byte_segment {
  int first;
  int last;
  struct byte_segment *next;
};

struct connection_state {
  struct table_entry *segment_table;
  struct byte_segment *queued_bytes;
  int cwind_total;
  int alpha;
};

struct fake_sock {
  struct fake_sock *prev;
  struct fake_sock *next;
  struct fake_sock *master;
  struct connection_state *cstate;
  int cwind;
  int rtt;
  int unacked_bytes;
  int iface;
  int mss;
};

struct sack_segment {
  int first;
  int last;
  struct sack_segment *next;
};

#endif
