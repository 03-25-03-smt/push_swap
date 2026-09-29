/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lis.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** Longest increasing subsequence of a, read cyclically starting from the
** minimum (so a rotated sorted stack is recognised as fully sorted).
** Patience sorting, O(n log n) CPU:
**   tail[p] = index (in seq) of the smallest possible last element of an
**             increasing subsequence of length p + 1,
**   prev[k] = index of the element before seq[k] in its best subsequence.
*/
static int	lis_search(t_lis *l, int x)
{
	int	lo;
	int	hi;
	int	mid;

	lo = 0;
	hi = l->len;
	while (lo < hi)
	{
		mid = (lo + hi) / 2;
		if (l->seq[l->tail[mid]] < x)
			lo = mid + 1;
		else
			hi = mid;
	}
	return (lo);
}

static void	lis_build(t_lis *l, int n)
{
	int	k;
	int	p;

	l->len = 0;
	k = -1;
	while (++k < n)
	{
		p = lis_search(l, l->seq[k]);
		l->prev[k] = -1;
		if (p > 0)
			l->prev[k] = l->tail[p - 1];
		l->tail[p] = k;
		if (p == l->len)
			l->len++;
	}
}

static void	lis_mark(t_lis *l, char *keep)
{
	int	k;

	k = l->tail[l->len - 1];
	while (k >= 0)
	{
		keep[l->seq[k]] = 1;
		k = l->prev[k];
	}
}

/*
** keep[rank] is set to 1 for every element of the LIS (values are ranks).
** Returns the LIS length, or -1 on allocation failure.
*/
int	mark_lis(t_ps *ps, char *keep)
{
	t_lis	l;
	int		n;
	int		mi;
	int		k;

	n = ps->a.size;
	l.seq = malloc(sizeof(int) * n);
	l.tail = malloc(sizeof(int) * n);
	l.prev = malloc(sizeof(int) * n);
	l.len = -1;
	if (l.seq && l.tail && l.prev)
	{
		mi = idx_of_min(&ps->a);
		k = -1;
		while (++k < n)
			l.seq[k] = st_get(&ps->a, (mi + k) % n);
		lis_build(&l, n);
		lis_mark(&l, keep);
	}
	free(l.seq);
	free(l.tail);
	free(l.prev);
	return (l.len);
}
