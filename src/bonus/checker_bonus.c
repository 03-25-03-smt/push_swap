/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker_bonus.h"

static int	fail(t_args *ar, int *ranks, t_ps *ps, t_buf *in)
{
	if (ar)
		free(ar->vals);
	free(ranks);
	if (ps)
		ps_free(ps);
	if (in)
		free(in->d);
	put_str(2, "Error\n");
	return (1);
}

/* Reads the instructions, executes them, prints OK or KO. */
static int	check(t_ps *ps)
{
	t_buf	in;
	int		i;

	in.d = NULL;
	if (!read_all(0, &in) || !parse_ops(&in, &ps->ops))
		return (fail(NULL, NULL, ps, &in));
	free(in.d);
	i = -1;
	while (++i < ps->ops.len)
		ps_exec(ps, ps->ops.d[i]);
	if (is_sorted(ps))
		put_str(1, "OK\n");
	else
		put_str(1, "KO\n");
	ps_free(ps);
	return (0);
}

int	main(int ac, char **av)
{
	t_args	ar;
	t_ps	ps;
	int		*ranks;

	if (ac < 2)
		return (0);
	if (!parse_args(ac, av, &ar, 0))
		return (fail(NULL, NULL, NULL, NULL));
	if (ar.n == 0)
	{
		free(ar.vals);
		return (0);
	}
	ranks = make_ranks(ar.vals, ar.n);
	if (!ranks)
		return (fail(&ar, NULL, NULL, NULL));
	if (!ps_init(&ps, ranks, ar.n))
		return (fail(&ar, ranks, NULL, NULL));
	free(ranks);
	free(ar.vals);
	return (check(&ps));
}
