/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** Operation names. Each one is stored in a fixed 4-byte slot, so the name of
** operation number op starts at offset op * 4 ("rra" + '\0' is 4 bytes).
*/
const char	*op_name(int op)
{
	const char	*table;

	table = "sa\0\0sb\0\0ss\0\0pa\0\0pb\0\0ra\0\0rb\0\0rr\0\0rra\0rrb\0rrr";
	return (table + op * 4);
}

/* Returns the operation code for a name, or -1 if it is not an operation. */
int	op_code(const char *s)
{
	int	op;

	op = 0;
	while (op < OP_COUNT)
	{
		if (ft_strcmp(s, op_name(op)) == 0)
			return (op);
		op++;
	}
	return (-1);
}

/* Prints all operations with a single write when memory allows it. */
void	print_ops(t_ops *ops)
{
	char		*buf;
	const char	*s;
	long		n;
	int			i;

	buf = malloc((long)ops->len * 4 + 1);
	i = -1;
	n = 0;
	while (++i < ops->len)
	{
		s = op_name(ops->d[i]);
		if (!buf)
		{
			put_str(1, s);
			put_str(1, "\n");
		}
		while (buf && *s)
			buf[n++] = *s++;
		if (buf)
			buf[n++] = '\n';
	}
	if (buf)
		write_all(1, buf, n);
	free(buf);
}
