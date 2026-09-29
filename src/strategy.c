/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strategy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** Complexity class of the adaptive regime, from the disorder d = mis / tot:
**   d < 0.2        -> O(n^2)     (5 * mis < tot)
**   0.2 <= d < 0.5 -> O(n sqrt n) (2 * mis < tot)
**   d >= 0.5       -> O(n log n)
** Integer comparisons avoid any floating point rounding at the thresholds.
*/
static int	adaptive_regime(t_args *ar)
{
	if (5 * ar->mis < ar->tot)
		return (S_SIMPLE);
	if (2 * ar->mis < ar->tot)
		return (S_MEDIUM);
	return (S_COMPLEX);
}

/*
** Adaptive method. A stack that is only rotated needs rotations only.
** Tiny inputs (n <= 5) use the selection sort (at most 12 operations).
** Otherwise the regime chooses the internal technique:
**   low disorder    -> LIS + cheapest insertion (strat_lis)
**   medium disorder -> sqrt(n) chunk sort       (strat_medium)
**   high disorder   -> 3-way chunk quicksort    (strat_complex)
*/
static int	strat_adaptive(t_ps *ps, t_args *ar)
{
	if (is_cyclic_sorted(&ps->a))
	{
		align_a(ps);
		return (!ps->fail);
	}
	if (ps->n <= 5)
		return (strat_simple(ps));
	if (ar->used == S_SIMPLE)
		return (strat_lis(ps));
	if (ar->used == S_MEDIUM)
		return (strat_medium(ps));
	return (strat_complex(ps));
}

/* Runs the selected strategy. Returns 0 on allocation failure. */
int	run_strategy(t_ps *ps, t_args *ar)
{
	ar->used = ar->strat;
	if (ar->strat == S_ADAPTIVE)
		ar->used = adaptive_regime(ar);
	if (is_sorted(ps))
		return (1);
	if (ar->strat == S_SIMPLE)
		return (strat_simple(ps));
	if (ps->n <= 3)
		return (strat_simple(ps));
	if (ar->strat == S_MEDIUM)
		return (strat_medium(ps));
	if (ar->strat == S_COMPLEX)
		return (strat_complex(ps));
	return (strat_adaptive(ps, ar));
}
