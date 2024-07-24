/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damin <damin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 16:37:00 by damin             #+#    #+#             */
/*   Updated: 2024/07/22 20:26:01 by damin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_putchar_fd(char c, int fd)
{
	return (write(fd, &c, 1));
}

int	ft_putstr_fd(char *s, int fd)
{
	return (write(fd, s, ft_strlen(s)));
}

int	ft_putnbr_fd(int n, int fd)
{
	char	c;
	int		res;

	res = 0;
	if (n < 0)
	{
		res += ft_putchar_fd('-', fd);
		n *= -1;
	}
	if (n >= 10)
		res += ft_putnbr_fd(n / 10, fd);
	c = n % 10 + '0';
	res += ft_putchar_fd(c, fd);
	return (res);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	res;

	i = 0;
	sign = 1;
	res = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * sign);
}

int	err_return(const char *str)
{
	printf("%s", str);
	return (1);
}