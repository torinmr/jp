#include "fake_headers.h"
#include "segment_table.h"
#include "congestion_control.h"
#include "dispatcher.h"

#include <stdio.h>
#include <stdlib.h>

void print_sack(struct sack_segment *sack)
{
  printf("first       last\n");
  while (sack != NULL) {
    printf("%-12d%-12d\n", sack->first, sack->last);
    sack = sack->next;
  }
}

/* Implement a policy for deciding which subflow to retransmit packets on. */
struct fake_sock *pick_retransmission_subflow(struct table_entry *entry)
{
  return entry->flow;
}

int retransmit(struct table_entry *entry, int loss_type) {
  congestion_loss(entry->flow, loss_type);
  entry->timeout = -1; /* TODO: Actually update the timeout */
  entry->dup_acks = 0;
  
  entry->flow = pick_retransmission_subflow(entry);
  return dispatcher_put_to_network(entry->flow, entry->first, entry->last);
}

int delete_entry(struct fake_sock *sk, struct table_entry *entry)
{
  struct table_entry *current = sk->cstate->segment_table;

  if (entry == current) {
    sk->cstate->segment_table = entry->next;
    goto end;
  }
  
  while (current->next != entry && current->next != NULL) {
    current = current->next;
  }
  
  if (current->next == NULL) {
    printf("Tried to delete an entry that's not in the table!\n");
    return -1;
  }

  /* We should be at the element just before the one we want to remove */
  current->next = current->next->next;
  
 end:
  free(entry);

  return 0;
}

int segment_table_register_bytes(struct fake_sock *conn_sk,
                                 struct fake_sock *flow_sk,
                                 int first, int last)
{
  struct table_entry *entry = conn_sk->cstate->segment_table;
  struct table_entry *new_entry = malloc(sizeof(struct table_entry));

  if (new_entry == NULL) {
    printf("Out of memory!\n");
    return -1;
  }

  new_entry->flow = flow_sk;
  new_entry->first = first;
  new_entry->last = last;
  new_entry->dup_acks = 0;
  new_entry->timeout = -1; /* TODO: implement real timeouts */
  new_entry->next = NULL;

  if (entry == NULL) {
    conn_sk->cstate->segment_table = new_entry;
    return 0;
  }
  
  while (entry->next != NULL) {
    entry = entry->next;
  }

  entry->next = new_entry;
  return 0;
}

int segment_table_update(struct fake_sock *sk, struct sack_segment *sack)
{
  struct table_entry *entry = sk->cstate->segment_table;
  struct table_entry *next;
  struct sack_segment *segment = sack;
  
  print_sack(sack);
  

  while (segment != NULL) {
    if (entry == NULL) { goto range_error; }

    while (entry->first < segment->first) {
      if (entry->next == NULL) {
        if (entry->last < segment->last) { goto range_error; }
        goto exit;
      }
      entry = entry->next;
    }

    while (entry->last < segment->last) {
      next = entry->next;
      if (congestion_ack(entry->flow, entry->last - entry->first) != 0) { return -1; }
      if (delete_entry(sk, entry) != 0) { return -1; }
      entry = next;
      if (entry == NULL) { goto range_error; }
    }

    if (entry->last == segment->last) {
      if (congestion_ack(entry->flow, entry->last - entry->first) != 0) { return -1; }
      if (delete_entry(sk, entry) != 0) { return -1; }
      segment = segment->next;
      continue;
    }

    if (entry->first < segment->last) {
      if (congestion_ack(entry->flow, segment->last - entry->first) != 0) { return -1; }
      entry->timeout = -1; /* TODO: Actually update timeout to NUM_SECS seconds after now */
      entry->first = segment->last;
      entry->dup_acks = 0;
    } else if (entry->first == segment->last && sk == entry->flow) {
      entry->dup_acks++;
      printf("INCing dup acks!\n");
      if (entry->dup_acks >= 3) {
        if (retransmit(entry, LOSS_TYPE_FAST_RETRANSMIT) != 0) { return -1; }
      }
    }

    segment = segment->next;
  }

 exit:
  return 0;

 range_error:
  printf("SACK received for byte not in segment table.\n");
  return -1;
}

void segment_table_print(struct fake_sock *sk)
{
  struct table_entry *entry = sk->cstate->segment_table;
  printf("flow_id     first_byte  last_byte    dup_acks    timeout\n");

  
  while (entry != NULL) {
    printf("%-12d%-12d%-12d%-12d%-12d\n", entry->flow->iface, entry->first,
           entry->last, entry->dup_acks, entry->timeout);
    entry = entry->next;
  }
}
