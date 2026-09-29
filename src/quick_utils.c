/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quick_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** A chunk is a group of consecutive ranks stored contiguously at one of the
** four "locations": top of a, bottom of a, top of b, bottom of b.
*/

static int	loc_in_a(int loc)
{
	return (loc == TOP_A || loc == BOT_A);
}

/* The i-th element of the chunk, counted from its own end of the stack. */
int	chunk_value(t_ps *ps, t_chunk *c, int i)
{
	if (c->loc == TOP_A)
		return (st_get(&ps->a, i));
	if (c->loc == BOT_A)
		return (st_get(&ps->a, ps->a.size - 1 - i));
	if (c->loc == TOP_B)
		return (st_get(&ps->b, i));
	return (st_get(&ps->b, ps->b.size - 1 - i));
}

int	chunk_max(t_ps *ps, t_chunk *c)
{
	int	i;
	int	max;

	max = chunk_value(ps, c, 0);
	i = 0;
	while (++i < c->size)
		if (chunk_value(ps, c, i) > max)
			max = chunk_value(ps, c, i);
	return (max);
}

/*
** Moves the first element of location "from" to location "to" in 1 to 3
** operations: bring it to the top of its stack, push it to the other stack
** if needed, then send it to the bottom if the target is a bottom.
*/
void	move_elem(t_ps *ps, int from, int to)
{
	if (from == BOT_A && ps->a.size > 1)
		do_op(ps, OP_RRA);
	else if (from == BOT_B && ps->b.size > 1)
		do_op(ps, OP_RRB);
	if (loc_in_a(from) && !loc_in_a(to))
		do_op(ps, OP_PB);
	else if (!loc_in_a(from) && loc_in_a(to))
		do_op(ps, OP_PA);
	if (to == BOT_A && ps->a.size > 1)
		do_op(ps, OP_RA);
	else if (to == BOT_B && ps->b.size > 1)
		do_op(ps, OP_RB);
}

/* A bottom chunk that fills its whole stack is also a top chunk. */
void	chunk_to_top(t_ps *ps, t_chunk *c)
{
	if (c->loc == BOT_A && ps->a.size == c->size)
		c->loc = TOP_A;
	if (c->loc == BOT_B && ps->b.size == c->size)
		c->loc = TOP_B;
}
