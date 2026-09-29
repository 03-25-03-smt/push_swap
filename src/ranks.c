/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ranks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	merge_halves(int *a, int *tmp, int mid, int n)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = mid;
	k = 0;
	while (i < mid || j < n)
	{
		if (j >= n || (i < mid && a[i] <= a[j]))
			tmp[k++] = a[i++];
		else
			tmp[k++] = a[j++];
	}
	i = -1;
	while (++i < n)
		a[i] = tmp[i];
}

static void	merge_sort(int *a, int *tmp, int n)
{
	if (n < 2)
		return ;
	merge_sort(a, tmp, n / 2);
	merge_sort(a + n / 2, tmp, n - n / 2);
	merge_halves(a, tmp, n / 2, n);
}

static int	find_index(int *sorted, int n, int x)
{
	int	lo;
	int	hi;
	int	mid;

	lo = 0;
	hi = n - 1;
	while (lo < hi)
	{
		mid = (lo + hi) / 2;
		if (sorted[mid] < x)
			lo = mid + 1;
		else
			hi = mid;
	}
	return (lo);
}

/*
** Replaces every value by its rank (0 = smallest, n - 1 = biggest).
** Only the relative order matters for sorting, and ranks make everything
** else simpler. Returns NULL on duplicates or allocation failure.
*/
int	*make_ranks(int *vals, int n)
{
	int	*sorted;
	int	*ranks;
	int	i;
	int	dup;

	sorted = malloc(sizeof(int) * n);
	ranks = malloc(sizeof(int) * n);
	dup = (!sorted || !ranks);
	i = -1;
	while (!dup && ++i < n)
		sorted[i] = vals[i];
	if (!dup)
		merge_sort(sorted, ranks, n);
	i = 0;
	while (!dup && ++i < n)
		dup = (sorted[i] == sorted[i - 1]);
	i = -1;
	while (!dup && ++i < n)
		ranks[i] = find_index(sorted, n, vals[i]);
	free(sorted);
	if (!dup)
		return (ranks);
	free(ranks);
	return (NULL);
}

/* Counts the pairs (i < j) with a[i] > a[j], exactly like the subject. */
void	compute_disorder(int *r, int n, long *mis, long *tot)
{
	int	i;
	int	j;

	*mis = 0;
	*tot = 0;
	i = -1;
	while (++i < n)
	{
		j = i;
		while (++j < n)
		{
			(*tot)++;
			if (r[i] > r[j])
				(*mis)++;
		}
	}
}
