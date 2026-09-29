/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <stdlib.h>
# include <unistd.h>
# include <limits.h>

/* Operation codes (also the index in the benchmark counters). */
# define OP_SA 0
# define OP_SB 1
# define OP_SS 2
# define OP_PA 3
# define OP_PB 4
# define OP_RA 5
# define OP_RB 6
# define OP_RR 7
# define OP_RRA 8
# define OP_RRB 9
# define OP_RRR 10
# define OP_COUNT 11

/* Strategies (S_SIMPLE / S_MEDIUM / S_COMPLEX double as complexity class). */
# define S_ADAPTIVE 0
# define S_SIMPLE 1
# define S_MEDIUM 2
# define S_COMPLEX 3

/* Chunk locations used by the quicksort. */
# define TOP_A 0
# define BOT_A 1
# define TOP_B 2
# define BOT_B 3

/* Circular buffer: element i (0 = top) is v[(head + i) % cap]. */
typedef struct s_stack
{
	int	*v;
	int	head;
	int	size;
	int	cap;
}	t_stack;

/* Recorded operations (one op code per byte). */
typedef struct s_ops
{
	char	*d;
	int		len;
	int		cap;
}	t_ops;

typedef struct s_ps
{
	t_stack	a;
	t_stack	b;
	t_ops	ops;
	int		n;
	int		fail;
}	t_ps;

/* Command line: numbers, selected strategy, bench flag and disorder. */
typedef struct s_args
{
	int		*vals;
	int		n;
	int		strat;
	int		used;
	int		bench;
	int		nsel;
	long	mis;
	long	tot;
}	t_args;

/* Rotations (signed) of a and b before a "pa", and their total cost. */
typedef struct s_move
{
	int	ra;
	int	rb;
	int	cost;
}	t_move;

typedef struct s_chunk
{
	int	loc;
	int	size;
}	t_chunk;

typedef struct s_split
{
	t_chunk	min;
	t_chunk	mid;
	t_chunk	max;
}	t_split;

/* One segment (between two pushes) of the optimizer: a-moves and b-moves. */
typedef struct s_seg
{
	char	*a;
	int		na;
	char	*b;
	int		nb;
}	t_seg;

typedef struct s_lis
{
	int	*seq;
	int	*tail;
	int	*prev;
	int	len;
}	t_lis;

/* stack.c, stack_rot.c */
int			st_init(t_stack *s, int cap);
int			st_get(t_stack *s, int i);
void		st_push(t_stack *s, int x);
int			st_pop(t_stack *s);
int			st_swap(t_stack *s);
int			st_rot(t_stack *s);
int			st_rrot(t_stack *s);
int			is_sorted(t_ps *ps);
int			is_cyclic_sorted(t_stack *s);

/* ps.c, output.c */
int			ps_init(t_ps *ps, int *ranks, int n);
void		ps_free(t_ps *ps);
void		ps_exec(t_ps *ps, int op);
void		do_op(t_ps *ps, int op);
const char	*op_name(int op);
int			op_code(const char *s);
void		print_ops(t_ops *ops);

/* parse.c, ranks.c, utils.c */
int			parse_args(int ac, char **av, t_args *ar, int with_flags);
int			*make_ranks(int *vals, int n);
void		compute_disorder(int *r, int n, long *mis, long *tot);
int			is_space(char c);
int			ft_strcmp(const char *a, const char *b);
void		write_all(int fd, const char *s, long len);
void		put_str(int fd, const char *s);
void		put_nbr(int fd, long n);

/* optimize.c, optimize_merge.c, bench.c */
void		optimize_ops(t_ops *o);
int			rewrite_segment(t_seg *sg, char *dst);
void		print_bench(t_args *ar, t_ops *ops);

/* moves.c, moves_cost.c */
int			idx_of_min(t_stack *s);
int			idx_of_max(t_stack *s);
int			idx_target(t_stack *a, int x, int mi);
void		rotate_by(t_ps *ps, int r, int up, int down);
int			shortest_rot(int i, int size);
void		set_move(t_move *m, int ia, int ib, t_ps *ps);
void		exec_move(t_ps *ps, t_move *m);
void		greedy_insert(t_ps *ps, int window);
void		align_a(t_ps *ps);

/* strategies */
int			run_strategy(t_ps *ps, t_args *ar);
void		sort_three_plain(t_ps *ps);
int			strat_simple(t_ps *ps);
int			mark_lis(t_ps *ps, char *keep);
int			strat_lis(t_ps *ps);
int			strat_medium(t_ps *ps);
int			strat_complex(t_ps *ps);

/* quick*.c */
int			chunk_value(t_ps *ps, t_chunk *c, int i);
int			chunk_max(t_ps *ps, t_chunk *c);
void		move_elem(t_ps *ps, int from, int to);
void		chunk_to_top(t_ps *ps, t_chunk *c);
void		split_chunk(t_ps *ps, t_chunk *c, t_split *s);
void		sort_one(t_ps *ps, t_chunk *c);
void		sort_two(t_ps *ps, t_chunk *c);
void		sort_three(t_ps *ps, t_chunk *c);
void		three_bot_a(t_ps *ps, t_chunk *c, int max);
void		three_bot_b(t_ps *ps, t_chunk *c, int max);

#endif
