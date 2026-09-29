/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quick.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** Where the three parts of a split go. The "max" part always goes to a,
** as far as possible from b; the "min" part goes deepest into b.
** Each destination differs from the source, so each element moves once.
*/
static void	set_split_locs(int loc, t_split *s)
{
	s->min.loc = BOT_B;
	s->mid.loc = TOP_B;
	s->max.loc = TOP_A;
	if (loc == TOP_A)
		s->max.loc = BOT_A;
	if (loc == TOP_B || loc == BOT_B)
		s->mid.loc = BOT_A;
	if (loc == BOT_B)
		s->min.loc = TOP_B;
	s->min.size = 0;
	s->mid.size = 0;
	s->max.size = 0;
}

static void	send_to(t_ps *ps, int from, t_chunk *dst)
{
	move_elem(ps, from, dst->loc);
	dst->size++;
}

/*
** Split a chunk of consecutive ranks [max - size + 1, max] in three parts:
**   value > max - p2               -> max part (the top third)
**   max - p1 < value <= max - p2   -> mid part
**   value <= max - p1              -> min part
** Every element of the chunk is looked at (and moved) exactly once.
*/
void	split_chunk(t_ps *ps, t_chunk *c, t_split *s)
{
	int	p1;
	int	p2;
	int	max;
	int	v;

	set_split_locs(c->loc, s);
	p2 = c->size / 3;
	p1 = 2 * c->size / 3;
	max = chunk_max(ps, c);
	while (c->size > 0 && !ps->fail)
	{
		v = chunk_value(ps, c, 0);
		if (v > max - p2)
			send_to(ps, c->loc, &s->max);
		else if (v > max - p1)
			send_to(ps, c->loc, &s->mid);
		else
			send_to(ps, c->loc, &s->min);
		c->size--;
	}
}

/*
** Recursive 3-way quicksort. The biggest part is sorted first: it goes on
** top of a, then the mid part is sorted above it, then the min part.
*/
static void	sort_chunk(t_ps *ps, t_chunk *c)
{
	t_split	s;

	if (ps->fail)
		return ;
	chunk_to_top(ps, c);
	if (c->size == 3)
		sort_three(ps, c);
	else if (c->size == 2)
		sort_two(ps, c);
	else if (c->size == 1)
		sort_one(ps, c);
	if (c->size <= 3)
		return ;
	split_chunk(ps, c, &s);
	sort_chunk(ps, &s.max);
	sort_chunk(ps, &s.mid);
	sort_chunk(ps, &s.min);
}

/*
** Complex method: 3-way quicksort on chunks. Each level of recursion moves
** every element once (1 to 3 ops) and divides chunk sizes by 3, so there
** are log3(n) levels: O(n log n) operations.
*/
int	strat_complex(t_ps *ps)
{
	t_chunk	all;

	all.loc = TOP_A;
	all.size = ps->a.size;
	sort_chunk(ps, &all);
	return (!ps->fail);
}
