/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strat_simple.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** Sorts a when it holds 2 or 3 elements, with at most 2 operations:
** put the biggest at the bottom, then swap the top two if needed.
*/
void	sort_three_plain(t_ps *ps)
{
	int	imax;

	if (ps->a.size == 3)
	{
		imax = idx_of_max(&ps->a);
		if (imax == 0)
			do_op(ps, OP_RA);
		else if (imax == 1)
			do_op(ps, OP_RRA);
	}
	if (ps->a.size >= 2 && st_get(&ps->a, 0) > st_get(&ps->a, 1))
		do_op(ps, OP_SA);
}

static int	a_is_sorted(t_stack *a)
{
	int	i;

	i = 0;
	while (i + 1 < a->size)
	{
		if (st_get(a, i) > st_get(a, i + 1))
			return (0);
		i++;
	}
	return (1);
}

/*
** Selection sort ("min extraction"): bring the minimum of a to the top by
** the shortest rotation, push it to b, repeat until 3 remain (or a is already
** sorted), sort those 3, then push everything back.
** Each extraction costs at most size/2 rotations + 1 push, so the total is
** at most n/2 * n + 2n operations: O(n^2).
*/
int	strat_simple(t_ps *ps)
{
	while (ps->a.size > 3 && !a_is_sorted(&ps->a))
	{
		rotate_by(ps, shortest_rot(idx_of_min(&ps->a), ps->a.size),
			OP_RA, OP_RRA);
		do_op(ps, OP_PB);
	}
	sort_three_plain(ps);
	while (ps->b.size > 0)
		do_op(ps, OP_PA);
	return (!ps->fail);
}
