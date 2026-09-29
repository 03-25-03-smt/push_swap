/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ps.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	ps_init(t_ps *ps, int *ranks, int n)
{
	int	i;

	ps->n = n;
	ps->fail = 0;
	ps->ops.len = 0;
	ps->ops.cap = 1024;
	ps->ops.d = malloc(ps->ops.cap);
	ps->b.v = NULL;
	if (!st_init(&ps->a, n) || !st_init(&ps->b, n) || !ps->ops.d)
	{
		ps_free(ps);
		return (0);
	}
	i = n;
	while (--i >= 0)
		st_push(&ps->a, ranks[i]);
	return (1);
}

void	ps_free(t_ps *ps)
{
	free(ps->a.v);
	free(ps->b.v);
	free(ps->ops.d);
	ps->a.v = NULL;
	ps->b.v = NULL;
	ps->ops.d = NULL;
}

/* Applies one operation to the stacks (no recording). */
void	ps_exec(t_ps *ps, int op)
{
	if (op == OP_SA || op == OP_SS)
		st_swap(&ps->a);
	if (op == OP_SB || op == OP_SS)
		st_swap(&ps->b);
	if (op == OP_RA || op == OP_RR)
		st_rot(&ps->a);
	if (op == OP_RB || op == OP_RR)
		st_rot(&ps->b);
	if (op == OP_RRA || op == OP_RRR)
		st_rrot(&ps->a);
	if (op == OP_RRB || op == OP_RRR)
		st_rrot(&ps->b);
	if (op == OP_PA && ps->b.size > 0)
		st_push(&ps->a, st_pop(&ps->b));
	if (op == OP_PB && ps->a.size > 0)
		st_push(&ps->b, st_pop(&ps->a));
}

/* Applies one operation and appends it to the recorded list. */
void	do_op(t_ps *ps, int op)
{
	char	*nd;
	int		i;

	ps_exec(ps, op);
	if (ps->fail)
		return ;
	if (ps->ops.len == ps->ops.cap)
	{
		nd = malloc(ps->ops.cap * 2);
		if (!nd)
		{
			ps->fail = 1;
			return ;
		}
		i = -1;
		while (++i < ps->ops.len)
			nd[i] = ps->ops.d[i];
		free(ps->ops.d);
		ps->ops.d = nd;
		ps->ops.cap *= 2;
	}
	ps->ops.d[ps->ops.len++] = op;
}
