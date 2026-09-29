/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	error_exit(t_args *ar, int *ranks)
{
	if (ar)
		free(ar->vals);
	free(ranks);
	put_str(2, "Error\n");
	return (1);
}

/* Runs the strategy, optimizes and prints the operations, then frees. */
static int	solve(t_ps *ps, t_args *ar)
{
	if (!run_strategy(ps, ar) || ps->fail)
	{
		ps_free(ps);
		return (error_exit(NULL, NULL));
	}
	optimize_ops(&ps->ops);
	print_ops(&ps->ops);
	if (ar->bench)
		print_bench(ar, &ps->ops);
	ps_free(ps);
	return (0);
}

/*
** 1. parse numbers and flags   2. replace values by ranks (checks duplicates)
** 3. measure the disorder      4. run the strategy (records the operations)
** 5. optimize and print them   6. optional benchmark on stderr
*/
int	main(int ac, char **av)
{
	t_args	ar;
	t_ps	ps;
	int		*ranks;

	if (ac < 2)
		return (0);
	if (!parse_args(ac, av, &ar, 1))
		return (error_exit(NULL, NULL));
	if (ar.n == 0)
	{
		free(ar.vals);
		return (0);
	}
	ranks = make_ranks(ar.vals, ar.n);
	if (!ranks)
		return (error_exit(&ar, NULL));
	compute_disorder(ranks, ar.n, &ar.mis, &ar.tot);
	if (!ps_init(&ps, ranks, ar.n))
		return (error_exit(&ar, ranks));
	free(ranks);
	free(ar.vals);
	return (solve(&ps, &ar));
}
