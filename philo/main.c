/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jomanuel <jomanuel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:49:47 by jomanuel          #+#    #+#             */
/*   Updated: 2025/09/05 18:30:12 by jomanuel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return(write(2, ERROR_INIT, 40), 1);
	if (!valid_args(argc, argv))
		return (free(data), 1);
	if (data_init(argc, argv, data))
		return (free(data), write(2, ERROR_INIT, 40), 1);
	data_manager(data);
	free(data);
	return (0);
}
