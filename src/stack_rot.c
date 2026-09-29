/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_rot.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/* ra/rb: the top element becomes the last one. */
int	st_rot(t_stack *s)
{
	int	x;

	if (s->size < 2)
		return (0);
	x = st_pop(s);
	s->v[(s->head + s->size) % s->cap] = x;
	s->size++;
	return (1);
}

/* rra/rrb: the last element becomes the first one. */
int	st_rrot(t_stack *s)
{
	int	x;

	if (s->size < 2)
		return (0);
	x = s->v[(s->head + s->size - 1) % s->cap];
	s->size--;
	st_push(s, x);
	return (1);
}

int	is_sorted(t_ps *ps)
{
	int	i;

	if (ps->b.size != 0)
		return (0);
	i = 0;
	while (i + 1 < ps->a.size)
	{
		if (st_get(&ps->a, i) > st_get(&ps->a, i + 1))
			return (0);
		i++;
	}
	return (1);
}

/* True if the stack is sorted up to a rotation (at most one descent). */
int	is_cyclic_sorted(t_stack *s)
{
	int	i;
	int	descents;

	i = 0;
	descents = 0;
	while (i < s->size)
	{
		if (st_get(s, i) > st_get(s, (i + 1) % s->size))
			descents++;
		i++;
	}
	return (descents <= 1);
}
