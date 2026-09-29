/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves_cost.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** A move = rotate a by m->ra and b by m->rb (positive: r, negative: rr),
** then "pa". When both go the same direction the rotations are shared
** (rr / rrr), so the cost is the max instead of the sum.
*/
static void	try_move(t_move *m, int ra, int rb)
{
	int	ca;
	int	cb;
	int	cost;

	ca = ra;
	if (ca < 0)
		ca = -ca;
	cb = rb;
	if (cb < 0)
		cb = -cb;
	cost = ca + cb;
	if ((ra >= 0) == (rb >= 0) && ca > cb)
		cost = ca;
	else if ((ra >= 0) == (rb >= 0))
		cost = cb;
	if (cost < m->cost)
	{
		m->ra = ra;
		m->rb = rb;
		m->cost = cost;
	}
}

/* Cheapest of the 4 ways (up/down for each stack) to bring ia and ib up. */
void	set_move(t_move *m, int ia, int ib, t_ps *ps)
{
	m->cost = 1 << 30;
	try_move(m, ia, ib);
	try_move(m, ia - ps->a.size, ib - ps->b.size);
	try_move(m, ia, ib - ps->b.size);
	try_move(m, ia - ps->a.size, ib);
}

void	exec_move(t_ps *ps, t_move *m)
{
	while (m->ra > 0 && m->rb > 0)
	{
		do_op(ps, OP_RR);
		m->ra--;
		m->rb--;
	}
	while (m->ra < 0 && m->rb < 0)
	{
		do_op(ps, OP_RRR);
		m->ra++;
		m->rb++;
	}
	rotate_by(ps, m->ra, OP_RA, OP_RRA);
	rotate_by(ps, m->rb, OP_RB, OP_RRB);
}

/*
** Moves everything from b back into a, keeping a sorted (up to rotation).
** Each step picks, among the elements of b whose rank is > max(b) - window,
** the one that is the cheapest to insert. window >= n means "all of b".
*/
void	greedy_insert(t_ps *ps, int window)
{
	t_move	best;
	t_move	cur;
	int		j;
	int		lim;
	int		mi;

	while (ps->b.size > 0)
	{
		lim = st_get(&ps->b, idx_of_max(&ps->b)) - window;
		mi = idx_of_min(&ps->a);
		best.cost = 1 << 30;
		j = -1;
		while (++j < ps->b.size)
		{
			if (st_get(&ps->b, j) > lim)
			{
				set_move(&cur, idx_target(&ps->a, st_get(&ps->b, j), mi),
					j, ps);
				if (cur.cost < best.cost)
					best = cur;
			}
		}
		exec_move(ps, &best);
		do_op(ps, OP_PA);
	}
}

/* Final step: rotate a (the shortest way) so that its minimum is on top. */
void	align_a(t_ps *ps)
{
	rotate_by(ps, shortest_rot(idx_of_min(&ps->a), ps->a.size),
		OP_RA, OP_RRA);
}
