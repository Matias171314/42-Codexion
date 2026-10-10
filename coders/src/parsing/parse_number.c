/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_number.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvasquez <mvasquez@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/02 18:42:35 by mvasquez          #+#    #+#             */
/*   Updated: 2026/10/02 18:42:56 by mvasquez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <limits.h>

static int	parse_digits(const char *str, t_ms *value)
{
	int		i;
	int		digit;
	t_ms	result;

	if (!str || str[0] == '\0')
		return (0);
	i = 0;
	result = 0;
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		digit = str[i] - '0';
		if (result > (LLONG_MAX - digit) / 10)
			return (0);
		result = result * 10 + digit;
		i++;
	}
	*value = result;
	return (1);
}

int	parse_ms(const char *str, t_ms *value)
{
	return (parse_digits(str, value));
}

int	parse_int_nonnegative(const char *str, int *value)
{
	t_ms	number;

	if (!parse_digits(str, &number))
		return (0);
	if (number > INT_MAX)
		return (0);
	*value = (int)number;
	return (1);
}

int	parse_int_positive(const char *str, int *value)
{
	if (!parse_int_nonnegative(str, value))
		return (0);
	if (*value == 0)
		return (0);
	return (1);
}
