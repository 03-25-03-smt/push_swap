/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strat_medium.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	isqrt(int n)
{
	int	r;

	r = 0;
	while ((long)(r + 1) *(r + 1) <= n)
		r++;
	return (r);
}

/*
** Phase 1: split ranks into chunks of size s ([0, s), [s, 2s), ...) and push
** them to b one chunk after the other: rotate a (ra only) until its top
** belongs to the current chunk, then pb. In one chunk, a is swept at most
** once. The lower half of each chunk is sent to the bottom of b (rb), so the
** biggest ranks of b always stay close to one of its ends.
*/
static void	push_chunks(t_ps *ps, int s)
{
	int	lim;
	int	pushed;
	int	x;

	lim = s;
	pushed = 0;
	while (ps->a.size > 3 && !ps->fail)
	{
		while (pushed >= lim)
			lim += s;
		while (st_get(&ps->a, 0) >= lim)
			do_op(ps, OP_RA);
		x = st_get(&ps->a, 0);
		do_op(ps, OP_PB);
		pushed++;
		if (x < lim - s / 2 && ps->b.size > 1)
			do_op(ps, OP_RB);
	}
}

/*
** Medium method: chunk sort, chunk size s = 5 * sqrt(n) (so sqrt(n) / 5
** chunks; the factor 5 was tuned on random inputs).
** Phase 1 (push_chunks), then sort the 3 elements left in a.
** Phase 2: insert back from b, choosing each time the cheapest element among
** the s biggest ones still in b. Those lie near the ends of b, and their
** place in a is among the last s inserted, so each insertion is O(s).
** Cost: phase 1 <= 2n (pb, rb) + n / s sweeps of a = O(n * n / s),
** phase 2 = n insertions * O(s). With s ~ sqrt(n): O(n sqrt(n)).
*/
int	strat_medium(t_ps *ps)
{
	int	s;

	s = isqrt(ps->n) * 5 + 2;
	push_chunks(ps, s);
	sort_three_plain(ps);
	greedy_insert(ps, s);
	align_a(ps);
	return (!ps->fail);
}
