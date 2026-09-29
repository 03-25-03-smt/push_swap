/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_ops_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker_bonus.h"

static int	buf_grow(t_buf *b, long need)
{
	char	*nd;
	long	i;

	if (need <= b->cap)
		return (1);
	while (b->cap < need)
		b->cap *= 2;
	nd = malloc(b->cap);
	if (!nd)
		return (0);
	i = -1;
	while (++i < b->len)
		nd[i] = b->d[i];
	free(b->d);
	b->d = nd;
	return (1);
}

/* Reads everything from fd into b. Returns 0 on read or malloc error. */
int	read_all(int fd, t_buf *b)
{
	long	r;

	b->len = 0;
	b->cap = READ_SIZE;
	b->d = malloc(b->cap);
	if (!b->d)
		return (0);
	r = 1;
	while (r > 0)
	{
		if (!buf_grow(b, b->len + READ_SIZE))
			return (0);
		r = read(fd, b->d + b->len, READ_SIZE);
		if (r > 0)
			b->len += r;
	}
	return (r == 0);
}

static int	ops_append(t_ops *o, int op)
{
	char	*nd;
	int		i;

	if (o->len == o->cap)
	{
		nd = malloc(o->cap * 2);
		if (!nd)
			return (0);
		i = -1;
		while (++i < o->len)
			nd[i] = o->d[i];
		free(o->d);
		o->d = nd;
		o->cap *= 2;
	}
	o->d[o->len++] = op;
	return (1);
}

/*
** Every instruction must be exactly one operation name followed by '\n'
** (no spaces, no empty lines, no missing final newline).
*/
int	parse_ops(t_buf *in, t_ops *out)
{
	char	name[4];
	long	pos;
	int		k;
	int		i;
	int		op;

	pos = 0;
	while (pos < in->len)
	{
		k = 0;
		while (pos + k < in->len && in->d[pos + k] != '\n' && k < 4)
			k++;
		if (k < 2 || k > 3 || pos + k >= in->len)
			return (0);
		i = -1;
		while (++i < k)
			name[i] = in->d[pos + i];
		name[k] = '\0';
		op = op_code(name);
		if (op < 0 || !ops_append(out, op))
			return (0);
		pos += k + 1;
	}
	return (1);
}
