/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: masselgu <masselgu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 16:43:31 by masselgu          #+#    #+#             */
/*   Updated: 2026/09/04 16:43:32 by masselgu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_sign(char *str, int i)
{
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
		{
			i++;
			if (str[i] != '0')
				return (-1);
		}
		else
		{
			i++;
			if (str[i] < '0' || str[i] > '9')
				return (-1);
		}
	}
	return (i);
}

int	valid_number(char *str)
{
	int	i;
	int	result;

	i = 0;
	result = 0;
	while (str[i] == ' ')
		i++;
	if (str[i] == '\0')
		return (1);
	result = check_sign(str, i);
	if (result == -1)
		return (1);
	i = result;
	while (str[i] && str[i] != ' ')
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	while (str[i] == ' ')
		i++;
	if (str[i] != '\0')
		return (1);
	return (0);
}

int	skip_sign(char *str, int i)
{
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
		{
			i++;
			while (str[i] == '0')
				i++;
		}
		else
			i++;
	}
	return (i);
}

long	ft_atoi(char *str)
{
	long	result;
	int		i;

	result = 0;
	i = 0;
	while (str[i] == ' ')
		i++;
	i = skip_sign(str, i);
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = (result * 10) + (str[i] - '0');
		if (result > 2147483647)
			return (-1);
		i++;
	}
	while (str[i] == ' ')
		i++;
	return (result);
}
