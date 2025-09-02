/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomanuel <jomanuel@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 22:25:29 by jomanuel          #+#    #+#             */
/*   Updated: 2025/09/02 21:31:37 by jomanuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_pos_within_int(const char *str)
{
	int		negative;
	size_t	len;

	negative = 0;
	if (*str == '-' || *str == '+')
		negative = (*str++ == '-');
	if (negative)
		return (0);
	while (*str == '0')
		str++;
	len = strlen(str);
	if (len == 0)
		return (1);
	if (len > 10)
		return (0);
	if (len < 10)
		return (1);
	return (ft_strcmp(str, INT_MAX_VAL) <= 0);
}

int	valid_args(int argc, char **argv)
{
	int	i;

	i = 1;
	if (argc < 5 || argc > 6)
	{
		write(2, PHILO_ERR, 37);
		write(2, PHILO_USAGE, 129);
		return (0);
	}
	while (i < argc)
	{
		if (!is_pos_within_int(argv[i]))
		{
			write(2, INVALID_ARGS_ERR, 78);
			return (0);
		}
		i++;
	}
	if (!ft_strcmp(argv[1], "0"))
	{
		write(2, PHILO_NUM_ERR, 38);
		return (0);
	}
	return (1);
}
