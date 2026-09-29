/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strat_lis.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/* Index of the nearest element of a (from top or bottom) not in keep. */
static int	nearest_unkept(t_stack *a, char *keep)
{
	int	i;

	i = 0;
	while (i <= a->size / 2)
	{
		if (!keep[st_get(a, i)])
			return (i);
		if (!keep[st_get(a, a->size - 1 - i)])
			return (-1 - i);
		i++;
	}
	return (0);
}

/* Pushes to b the "left" elements of a that are not in the LIS. */
static void	push_unkept(t_ps *ps, char *keep, int left)
{
	while (left > 0 && !ps->fail)
	{
		rotate_by(ps, nearest_unkept(&ps->a, keep), OP_RA, OP_RRA);
		do_op(ps, OP_PB);
		left--;
	}
}

/*
** Low-disorder method (adaptive, disorder < 0.2): "LIS + insertion".
** 1. The longest increasing subsequence stays in a: it is already sorted.
** 2. Every other element is pushed to b (nearest one first).
** 3. Each element of b is inserted back at its place in a, always choosing
**    the cheapest one (rotations of a and b are shared with rr / rrr).
** 4. a is rotated so that its minimum is on top.
** When the input is nearly sorted the LIS is long and b stays small, so very
** few operations are needed. Upper bound: n pushes + for each of the n
** insertions at most n rotations -> O(n^2).
*/
int	strat_lis(t_ps *ps)
{
	char	*keep;
	int		left;
	int		i;

	keep = malloc(ps->n);
	if (!keep)
		return (0);
	i = -1;
	while (++i < ps->n)
		keep[i] = 0;
	left = mark_lis(ps, keep);
	if (left < 0)
	{
		free(keep);
		return (0);
	}
	push_unkept(ps, keep, ps->n - left);
	free(keep);
	greedy_insert(ps, ps->n);
	align_a(ps);
	return (!ps->fail);
}
