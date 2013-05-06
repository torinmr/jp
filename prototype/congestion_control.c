#include "fake_headers.h"
#include "congestion_control.h"

/* This file implements Linked-Increases Algorithm (LIA) as specified
 * in RFC 6356.
 *
 * This could be substituted for the Opportunistic Linked-Increases
 * Algorithm (OLIA) proposed by Khalili, (draft-khalili-mptcp-
 * congestion-control-00) or any other congestion control algorithm.
 */

void update_alpha(struct fake_sock *sk)
{
  int max = 0; 
  int current_max = 0;
  int sum = 0;
  int alpha;
  struct fake_sock *current = sk;

  do {
    current_max = current->cwind / (current->rtt * current->rtt);
    if (current_max > max) { max = current_max; }
    sum += current->cwind / current->rtt;
  } while (current != sk);

  alpha = sk->cstate->cwind_total << ALPHA_SCALE_FACTOR;
  alpha *= max;
  alpha /= sum*sum;

  sk->cstate->alpha = alpha;
}

int congestion_init(struct fake_sock *sk)
{
  sk->unacked_bytes = 0;
  sk->cwind = ?;
  
}

int congestion_ack(struct fake_sock *sk, int num_bytes)
{ 
  int multipath_increase;
  int tcp_increase;
  int increase;

  sk->unacked_bytes -= num_bytes;
 
  multipath_increase = sk->cstate->alpha * num_bytes * sk->mss;
  multipath_increase /= (sk->cstate->cwnd_total << ALPHA_SCALE_FACTOR);

  tcp_increase = num_bytes * sk->mss / sk->cwind;

  if (multipath_increase < tcp_increase) { increase = multipath_increase; }
  else { increase = tcp_increase; }

  sk->cwind += increase;
  sk->cstate->cwind_total += increase;
  
  return 0;
}

int congestion_loss(struct fake_sock *sk, int type)
{
  return 0;
}

int congestion_bytes_available(struct fake_sock *sk)
{
  int diff = sk->cwind - sk->unacked_bytes;
  if (diff < 0) diff = 0;
  return diff;
}
