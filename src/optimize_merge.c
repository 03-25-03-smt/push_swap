/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optimize_merge.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/* Combined operation for an (a-move, b-move) pair, or -1 if none exists. */
static int	merged(int op_a, int op_b)
{
	if (op_a == OP_SA && op_b == OP_SB)
		return (OP_SS);
	if (op_a == OP_RA && op_b == OP_RB)
		return (OP_RR);
	if (op_a == OP_RRA && op_b == OP_RRB)
		return (OP_RRR);
	return (-1);
}

/*
** Longest-common-subsequence table: t[i][j] = max number of merges possible
** between a[i..] and b[j..]. Stored flat with row width nb + 1.
*/
static void	fill_table(t_seg *sg, int *t)
{
	int	i;
	int	j;
	int	w;

	w = sg->nb + 1;
	i = sg->na + 1;
	while (--i >= 0)
	{
		j = sg->nb + 1;
		while (--j >= 0)
		{
			if (i == sg->na || j == sg->nb)
				t[i * w + j] = 0;
			else if (merged(sg->a[i], sg->b[j]) >= 0)
				t[i * w + j] = t[(i + 1) * w + j + 1] + 1;
			else if (t[(i + 1) * w + j] >= t[i * w + j + 1])
				t[i * w + j] = t[(i + 1) * w + j];
			else
				t[i * w + j] = t[i * w + j + 1];
		}
	}
}

static int	emit_plain(t_seg *sg, char *dst)
{
	int	i;

	i = -1;
	while (++i < sg->na)
		dst[i] = sg->a[i];
	i = -1;
	while (++i < sg->nb)
		dst[sg->na + i] = sg->b[i];
	return (sg->na + sg->nb);
}

/* Walks the table and writes the merged sequence (merging whenever able). */
static int	emit_merged(t_seg *sg, int *t, char *dst)
{
	int	i;
	int	j;
	int	n;
	int	w;

	w = sg->nb + 1;
	i = 0;
	j = 0;
	n = 0;
	while (i < sg->na || j < sg->nb)
	{
		if (i < sg->na && j < sg->nb && merged(sg->a[i], sg->b[j]) >= 0)
			dst[n++] = merged(sg->a[i++], sg->b[j++]);
		else if (j == sg->nb
			|| (i < sg->na && t[(i + 1) * w + j] >= t[i * w + j + 1]))
			dst[n++] = sg->a[i++];
		else
			dst[n++] = sg->b[j++];
	}
	return (n);
}

/* Writes the optimal merge of the a-list and b-list; returns its length. */
int	rewrite_segment(t_seg *sg, char *dst)
{
	int	*t;
	int	n;

	if (sg->na == 0 || sg->nb == 0
		|| (long)(sg->na + 1) *(sg->nb + 1) > 4000000)
		return (emit_plain(sg, dst));
	t = malloc(sizeof(int) * (sg->na + 1) * (sg->nb + 1));
	if (!t)
		return (emit_plain(sg, dst));
	fill_table(sg, t);
	n = emit_merged(sg, t, dst);
	free(t);
	return (n);
}
