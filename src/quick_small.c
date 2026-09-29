/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quick_small.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** Final placement of chunks of 1, 2 or 3 elements: they always end up
** sorted on top of a (above the bigger, already sorted, elements).
*/

void	sort_one(t_ps *ps, t_chunk *c)
{
	if (c->loc != TOP_A)
		move_elem(ps, c->loc, TOP_A);
	c->size = 0;
}

void	sort_two(t_ps *ps, t_chunk *c)
{
	if (c->loc != TOP_A)
	{
		move_elem(ps, c->loc, TOP_A);
		move_elem(ps, c->loc, TOP_A);
	}
	if (st_get(&ps->a, 0) > st_get(&ps->a, 1))
		do_op(ps, OP_SA);
	c->size = 0;
}

/* Top of a: x y z. Put the max in third position, then sort the top two. */
static void	three_top_a(t_ps *ps, int max)
{
	if (ps->a.size == 3)
	{
		sort_three_plain(ps);
		return ;
	}
	if (st_get(&ps->a, 0) == max)
		do_op(ps, OP_SA);
	if (st_get(&ps->a, 1) == max)
	{
		do_op(ps, OP_PB);
		do_op(ps, OP_SA);
		do_op(ps, OP_PA);
	}
	if (st_get(&ps->a, 0) > st_get(&ps->a, 1))
		do_op(ps, OP_SA);
}

/* Top of b: bring the 3 elements to a with the max arriving first. */
static void	three_top_b(t_ps *ps, int max)
{
	if (st_get(&ps->b, 1) == max)
		do_op(ps, OP_SB);
	if (st_get(&ps->b, 2) == max)
	{
		do_op(ps, OP_PA);
		do_op(ps, OP_SB);
		do_op(ps, OP_PA);
		do_op(ps, OP_SA);
	}
	else
	{
		do_op(ps, OP_PA);
		do_op(ps, OP_PA);
	}
	do_op(ps, OP_PA);
	if (st_get(&ps->a, 0) > st_get(&ps->a, 1))
		do_op(ps, OP_SA);
}

void	sort_three(t_ps *ps, t_chunk *c)
{
	int	max;

	max = chunk_max(ps, c);
	if (c->loc == TOP_A)
		three_top_a(ps, max);
	else if (c->loc == TOP_B)
		three_top_b(ps, max);
	else if (c->loc == BOT_A)
		three_bot_a(ps, c, max);
	else
		three_bot_b(ps, c, max);
	c->size = 0;
}
