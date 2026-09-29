/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	print_count(const char *name, long count, const char *end)
{
	put_str(2, name);
	put_str(2, ": ");
	put_nbr(2, count);
	put_str(2, end);
}

static const char	*strategy_name(int strat)
{
	if (strat == S_SIMPLE)
		return ("Simple");
	if (strat == S_MEDIUM)
		return ("Medium");
	if (strat == S_COMPLEX)
		return ("Complex");
	return ("Adaptive");
}

/* \302\262 is UTF-8 for the superscript 2, \342\210\232 for the root sign. */
static const char	*class_name(int cls)
{
	if (cls == S_SIMPLE)
		return ("O(n\302\262)");
	if (cls == S_MEDIUM)
		return ("O(n\342\210\232n)");
	return ("O(n log n)");
}

/* Disorder in percent, rounded to two decimals: mis / tot * 100. */
static void	print_disorder(t_args *ar)
{
	long	hundredths;

	hundredths = 0;
	if (ar->tot > 0)
		hundredths = (ar->mis * 20000 + ar->tot) / (2 * ar->tot);
	put_str(2, "[bench] disorder: ");
	put_nbr(2, hundredths / 100);
	put_str(2, ".");
	if (hundredths % 100 < 10)
		put_str(2, "0");
	put_nbr(2, hundredths % 100);
	put_str(2, "%\n");
}

/* Benchmark report on stderr, printed after the operations. */
void	print_bench(t_args *ar, t_ops *ops)
{
	long	cnt[OP_COUNT];
	int		i;

	i = -1;
	while (++i < OP_COUNT)
		cnt[i] = 0;
	i = -1;
	while (++i < ops->len)
		cnt[(int)ops->d[i]]++;
	print_disorder(ar);
	put_str(2, "[bench] strategy: ");
	put_str(2, strategy_name(ar->strat));
	put_str(2, " / ");
	put_str(2, class_name(ar->used));
	print_count("\n[bench] total_ops", ops->len, "\n[bench] ");
	i = -1;
	while (++i < OP_COUNT)
	{
		if (i == OP_PB)
			print_count(op_name(i), cnt[i], "\n[bench] ");
		else if (i == OP_RRR)
			print_count(op_name(i), cnt[i], "\n");
		else
			print_count(op_name(i), cnt[i], " ");
	}
}
