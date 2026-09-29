/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	idx_of_min(t_stack *s)
{
	int	i;
	int	best;

	i = 0;
	best = 0;
	while (++i < s->size)
		if (st_get(s, i) < st_get(s, best))
			best = i;
	return (best);
}

int	idx_of_max(t_stack *s)
{
	int	i;
	int	best;

	i = 0;
	best = 0;
	while (++i < s->size)
		if (st_get(s, i) > st_get(s, best))
			best = i;
	return (best);
}

/*
** a is sorted up to a rotation and its minimum is at index mi.
** Returns the index that must be on top of a so that "pa" of x keeps a
** sorted: the smallest element bigger than x, or the minimum if x is the
** biggest. Binary search over the logical (unrotated) order.
*/
int	idx_target(t_stack *a, int x, int mi)
{
	int	lo;
	int	hi;
	int	mid;

	if (a->size == 0)
		return (0);
	lo = 0;
	hi = a->size;
	while (lo < hi)
	{
		mid = (lo + hi) / 2;
		if (st_get(a, (mi + mid) % a->size) > x)
			hi = mid;
		else
			lo = mid + 1;
	}
	return ((mi + lo) % a->size);
}

/* Rotates by r: r > 0 means r times "up", r < 0 means -r times "down". */
void	rotate_by(t_ps *ps, int r, int up, int down)
{
	while (r > 0)
	{
		do_op(ps, up);
		r--;
	}
	while (r < 0)
	{
		do_op(ps, down);
		r++;
	}
}

/* Shortest rotation that brings index i of a stack of this size to top. */
int	shortest_rot(int i, int size)
{
	if (i <= size / 2)
		return (i);
	return (i - size);
}
