/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   optimize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** Peephole optimizer, run once on the recorded operation list.
**
** Between two pushes (pa/pb), operations on a and operations on b are
** independent. So each such "segment" is split into a list of a-moves and a
** list of b-moves. Inverse neighbours cancel (ra+rra, sa+sa), then the two
** lists are merged back, pairing ra+rb into rr, rra+rrb into rrr, sa+sb into
** ss as often as possible. Adjacent pb+pa or pa+pb also cancel.
** The final state of both stacks is unchanged, only the list gets shorter.
*/

static int	is_push(int op)
{
	return (op == OP_PA || op == OP_PB);
}

static void	seg_add(char *list, int *n, int op)
{
	int	last;

	if (*n > 0)
	{
		last = list[*n - 1];
		if ((op == last && (op == OP_SA || op == OP_SB))
			|| (op == OP_RA && last == OP_RRA)
			|| (op == OP_RRA && last == OP_RA)
			|| (op == OP_RB && last == OP_RRB)
			|| (op == OP_RRB && last == OP_RB))
		{
			(*n)--;
			return ;
		}
	}
	list[*n] = op;
	(*n)++;
}

static void	seg_split(t_seg *sg, int op)
{
	if (op == OP_SA || op == OP_SS)
		seg_add(sg->a, &sg->na, OP_SA);
	if (op == OP_SB || op == OP_SS)
		seg_add(sg->b, &sg->nb, OP_SB);
	if (op == OP_RA || op == OP_RR)
		seg_add(sg->a, &sg->na, OP_RA);
	if (op == OP_RB || op == OP_RR)
		seg_add(sg->b, &sg->nb, OP_RB);
	if (op == OP_RRA || op == OP_RRR)
		seg_add(sg->a, &sg->na, OP_RRA);
	if (op == OP_RRB || op == OP_RRR)
		seg_add(sg->b, &sg->nb, OP_RRB);
}

static int	opt_pass(t_ops *o, t_seg *sg, char *out)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	while (i < o->len)
	{
		if (is_push(o->d[i]))
		{
			if (n > 0 && is_push(out[n - 1]) && out[n - 1] != o->d[i])
				n--;
			else
				out[n++] = o->d[i];
			i++;
		}
		else
		{
			sg->na = 0;
			sg->nb = 0;
			while (i < o->len && !is_push(o->d[i]))
				seg_split(sg, o->d[i++]);
			n += rewrite_segment(sg, out + n);
		}
	}
	return (n);
}

/* Repeats passes while they shorten the list. On malloc failure: no-op. */
void	optimize_ops(t_ops *o)
{
	t_seg	sg;
	char	*out;
	int		n;
	int		improved;

	sg.a = malloc(o->len + 1);
	sg.b = malloc(o->len + 1);
	out = malloc((long)o->len * 2 + 1);
	improved = (sg.a && sg.b && out);
	while (improved)
	{
		n = opt_pass(o, &sg, out);
		improved = (n < o->len);
		if (improved)
		{
			o->len = n;
			while (--n >= 0)
				o->d[n] = out[n];
		}
	}
	free(sg.a);
	free(sg.b);
	free(out);
}
