/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quick_bottom.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** 3-element chunk at the bottom of a (c1 = bottom-most, c2, c3).
** After two rra, a starts with c2 c1 and c3 is still at the bottom.
** Each branch leaves the max in third position; sort_two finishes.
*/
void	three_bot_a(t_ps *ps, t_chunk *c, int max)
{
	do_op(ps, OP_RRA);
	do_op(ps, OP_RRA);
	if (st_get(&ps->a, 0) == max)
	{
		do_op(ps, OP_SA);
		do_op(ps, OP_RRA);
	}
	else if (st_get(&ps->a, 1) == max)
		do_op(ps, OP_RRA);
	else
	{
		do_op(ps, OP_PB);
		do_op(ps, OP_RRA);
		do_op(ps, OP_SA);
		do_op(ps, OP_PA);
	}
	c->loc = TOP_A;
	sort_two(ps, c);
}

/*
** 3-element chunk at the bottom of b. After two rrb, b starts with c2 c1.
** The max is sent to a first, the 2 others stay on top of b for sort_two.
*/
void	three_bot_b(t_ps *ps, t_chunk *c, int max)
{
	do_op(ps, OP_RRB);
	do_op(ps, OP_RRB);
	if (st_get(&ps->b, 0) == max)
	{
		do_op(ps, OP_PA);
		do_op(ps, OP_RRB);
	}
	else if (st_get(&ps->b, 1) == max)
	{
		do_op(ps, OP_SB);
		do_op(ps, OP_PA);
		do_op(ps, OP_RRB);
	}
	else
	{
		do_op(ps, OP_RRB);
		do_op(ps, OP_PA);
	}
	c->loc = TOP_B;
	sort_two(ps, c);
}
