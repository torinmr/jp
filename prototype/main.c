#include <stdio.h>
#include <stdlib.h>
#include "dispatcher.h"
#include "segment_table.h"

void print_subflow_info(struct fake_sock *sk) {
  struct fake_sock *current;
  struct fake_sock *master = sk->master;

  printf("flow_id     cwind       unacked_bytes\n");
  current = master;
  do {
    printf("%-12d%-12d%-12d\n", current->iface, current->cwind, current->unacked_bytes);
    current = current->next;
  } while (current != master);
}

int main(void)
{
  int num_flows, num_bytes, i, first_byte, last_byte, flow_id;
  struct fake_sock *last, *current, *master;
  struct sack_segment *sack, *new_segment, *current_segment;

  printf("Welcome to the MPServal simulator prototype. Functionality is rudimentary at best.\n");

  printf("How many subflows would you like to create? Enter a number.\n");
  scanf("%d", &num_flows);

  while (num_flows < 1) {
    printf("Please enter a number greater than 0.\n");
    scanf("%d", &num_flows);
  }

  printf("Creating %d subflows, numbered 0 to %d...", num_flows, num_flows - 1);
  
  master = malloc(sizeof(struct fake_sock));
  master->master = master;
  master->cstate = malloc(sizeof(struct connection_state));
  master->cstate->segment_table = NULL;
  master->iface = 0;
  master->last = master;
  congestion_init(master);

  for (i = 1; i < num_flows; i++) {
    current = malloc(sizeof(struct fake_sock));
    current->prev = last;
    current->master = master;
    current->cstate = master->cstate;
    current->iface = i;
    congestion_init(master);

    last->next = current;
    last = current;
  }
  
  last->next = master;
  master->prev = last;

  printf(" Done!\n");


  printf("How many bytes would you like to send?\n");
  scanf("%d", &num_bytes);
  while (num_bytes < 1) {
    printf("Please enter a number greater than 0.\n");
    scanf("%d", &num_bytes);
  }

  printf("Sending %d bytes!\n", num_bytes);
  dispatcher_send_bytes(master, 0, num_bytes);

  printf("\n\n");
  printf("Enter a SACK to see its effect on the protocol.\n");
  printf("Format is: 1 0 3 5 9 11 20 ...\n");
  printf("Where the first number is the id of the subflow the SACK is received on,\n");
  printf("and each pair of numbers thereafter represents a SACK for the interval [a, b)\n");
  printf("Enter as many as you like, terminate with \"0 0\".\n");
  printf("Begin your command with a -1 to exit.\n\n");
  
  while (1) {   
    printf("\n");
    print_subflow_info(master);
    printf("\n");
    segment_table_print(master);
    printf("\n");

    scanf("%d", &flow_id);
    if (flow_id == -1) { break; }

    current = master;
    do {
      if (current->iface == flow_id) { break; }
      current = current->next;
    } while (current != master);

    if (current->iface != flow_id) {
      printf("Please enter a valid flow_id, an integer between 0 and the number\n");
      printf("of flows you created at the beginning. Here, try again.\n\n");
      continue;
    }

    while (1) {
      scanf("%d %d", &first_byte, &last_byte);
      
      if (first_byte == 0 && last_byte == 0) { break; }
      
      new_segment = malloc(sizeof(struct sack_segment));
      new_segment->first = first_byte;
      new_segment->last = last_byte;
      new_segment->next = NULL;
      
      if (sack == NULL) {
        sack = new_segment;
    } else {
        current_segment = sack;
        while (current_segment->next != NULL) { current_segment = current_segment->next; }
        current_segment->next = new_segment;
      }
    }

    dispatcher_receive_ack(current, sack);

    current_segment = sack;
    while (current_segment != NULL) {
      sack = current_segment;
      current_segment = current_segment->next;
      free(sack);
    }
    sack = NULL;
  }

  return 0;
}
