/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** Reads one integer starting at s[*i]: optional sign, at least one digit,
** then a space or the end of the string. Rejects values outside int range.
*/
static int	parse_token(const char *s, int *i, int *out)
{
	long	v;
	long	sign;
	int		digits;

	sign = 1;
	if (s[*i] == '+' || s[*i] == '-')
	{
		if (s[*i] == '-')
			sign = -1;
		(*i)++;
	}
	v = 0;
	digits = 0;
	while (s[*i] >= '0' && s[*i] <= '9')
	{
		v = v * 10 + (s[*i] - '0');
		if (v * sign > INT_MAX || v * sign < INT_MIN)
			return (0);
		(*i)++;
		digits++;
	}
	if (digits == 0 || (s[*i] && !is_space(s[*i])))
		return (0);
	*out = (int)(v * sign);
	return (1);
}

/* One argument may hold several numbers: "3 2 1". Empty arguments fail. */
static int	parse_arg(const char *s, t_args *ar)
{
	int	i;
	int	found;

	i = 0;
	found = 0;
	while (s[i])
	{
		while (is_space(s[i]))
			i++;
		if (!s[i])
			break ;
		if (!parse_token(s, &i, &ar->vals[ar->n]))
			return (0);
		ar->n++;
		found = 1;
	}
	return (found);
}

/* Returns 1 if s is a known flag (and records it), 0 otherwise. */
static int	parse_flag(const char *s, t_args *ar)
{
	int	sel;

	sel = -1;
	if (ft_strcmp(s, "--bench") == 0)
	{
		ar->bench = 1;
		return (1);
	}
	if (ft_strcmp(s, "--simple") == 0)
		sel = S_SIMPLE;
	else if (ft_strcmp(s, "--medium") == 0)
		sel = S_MEDIUM;
	else if (ft_strcmp(s, "--complex") == 0)
		sel = S_COMPLEX;
	else if (ft_strcmp(s, "--adaptive") == 0)
		sel = S_ADAPTIVE;
	if (sel < 0)
		return (0);
	ar->strat = sel;
	ar->nsel++;
	return (1);
}

static long	count_capacity(int ac, char **av)
{
	long	cap;
	long	len;
	int		i;

	cap = 0;
	i = 0;
	while (++i < ac)
	{
		len = 0;
		while (av[i][len])
			len++;
		cap += len / 2 + 1;
	}
	return (cap);
}

/*
** Fills ar->vals with every number of the command line. When with_flags is 0
** (checker), strategy flags are treated as invalid numbers.
*/
int	parse_args(int ac, char **av, t_args *ar, int with_flags)
{
	int	i;
	int	ok;

	ar->n = 0;
	ar->strat = S_ADAPTIVE;
	ar->used = S_COMPLEX;
	ar->bench = 0;
	ar->nsel = 0;
	ar->vals = malloc(sizeof(int) * count_capacity(ac, av));
	if (!ar->vals)
		return (0);
	ok = 1;
	i = 0;
	while (ok && ++i < ac)
	{
		if (!with_flags || !parse_flag(av[i], ar))
			ok = parse_arg(av[i], ar);
	}
	if (ok && ar->nsel <= 1)
		return (1);
	free(ar->vals);
	ar->vals = NULL;
	return (0);
}
