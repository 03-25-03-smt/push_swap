/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vborodii <vborodii@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 10:00:00 by vborodii          #+#    #+#             */
/*   Updated: 2026/09/28 10:00:00 by vborodii         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	is_space(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	ft_strcmp(const char *a, const char *b)
{
	int	i;

	i = 0;
	while (a[i] && a[i] == b[i])
		i++;
	return ((unsigned char)a[i] - (unsigned char)b[i]);
}

/* Writes the whole buffer, retrying on partial writes. */
void	write_all(int fd, const char *s, long len)
{
	long	w;

	while (len > 0)
	{
		w = write(fd, s, len);
		if (w <= 0)
			return ;
		s += w;
		len -= w;
	}
}

void	put_str(int fd, const char *s)
{
	long	len;

	len = 0;
	while (s[len])
		len++;
	write_all(fd, s, len);
}

void	put_nbr(int fd, long n)
{
	char	c;

	if (n < 0)
	{
		put_str(fd, "-");
		n = -n;
	}
	if (n >= 10)
		put_nbr(fd, n / 10);
	c = '0' + n % 10;
	write_all(fd, &c, 1);
}
