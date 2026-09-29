/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/* Circular-buffer stack: index 0 is always the top of the stack. */

int	st_init(t_stack *s, int cap)
{
	s->v = malloc(sizeof(int) * (cap + 1));
	s->head = 0;
	s->size = 0;
	s->cap = cap + 1;
	return (s->v != NULL);
}

int	st_get(t_stack *s, int i)
{
	return (s->v[(s->head + i) % s->cap]);
}

void	st_push(t_stack *s, int x)
{
	s->head = (s->head + s->cap - 1) % s->cap;
	s->v[s->head] = x;
	s->size++;
}

int	st_pop(t_stack *s)
{
	int	x;

	x = s->v[s->head];
	s->head = (s->head + 1) % s->cap;
	s->size--;
	return (x);
}

int	st_swap(t_stack *s)
{
	int	tmp;
	int	second;

	if (s->size < 2)
		return (0);
	second = (s->head + 1) % s->cap;
	tmp = s->v[s->head];
	s->v[s->head] = s->v[second];
	s->v[second] = tmp;
	return (1);
}
