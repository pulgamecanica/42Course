/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malloc                                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18                               #+#    #+#             */
/*   Updated: 2025/01/18                              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "zone.h"
#include "block.h"
#include "alloc_hdr.h"
#include <unistd.h>
#include <stdint.h>

/*
** ft_putstr()
**
** Writes a string to stdout.
** Simple helper for show_alloc_mem() output.
*/

static void	ft_putstr(const char *s)
{
	size_t	len;
	ssize_t	ret;

	if (!s)
		return ;
	len = 0;
	while (s[len])
		len++;
	ret = write(1, s, len);
	(void)ret;
}

/*
** ft_puthex()
**
** Writes a hexadecimal number to stdout.
** Used for displaying memory addresses.
*/

static void	ft_puthex(uintptr_t n)
{
	const char	*hex = "0123456789ABCDEF";
	char		buf[18];
	int			i;

	buf[0] = '0';
	buf[1] = 'x';
	i = 17;
	buf[i--] = '\0';
	if (n == 0)
	{
		buf[i--] = '0';
	}
	else
	{
		while (n > 0 && i >= 2)
		{
			buf[i--] = hex[n % 16];
			n /= 16;
		}
	}
	ft_putstr(&buf[i + 1]);
}

/*
** ft_putnbr()
**
** Writes a decimal number to stdout.
** Used for displaying byte counts.
*/

static void	ft_putnbr(size_t n)
{
	char	buf[32];
	int		i;

	i = 31;
	buf[i--] = '\0';
	if (n == 0)
	{
		buf[i--] = '0';
	}
	else
	{
		while (n > 0 && i >= 0)
		{
			buf[i--] = '0' + (n % 10);
			n /= 10;
		}
	}
	ft_putstr(&buf[i + 1]);
}

/*
** ft_show_zone_type()
**
** Displays all allocations in zones of a given type.
** Format:
** TINY : 0xA0000
** 0xA0020 - 0xA004A : 42 bytes
*/

static size_t	ft_show_zone_type(ft_zone_t *zone_list, const char *type_name)
{
	ft_zone_t		*zone;
	ft_block_t		*block;
	ft_alloc_hdr_t	*hdr;
	void			*user_ptr;
	size_t			total;

	total = 0;
	zone = zone_list;
	while (zone)
	{
		ft_putstr(type_name);
		ft_putstr(" : ");
		ft_puthex((uintptr_t)zone);
		ft_putstr("\n");
		block = zone->first_block;
		while (block)
		{
			if (!block->is_free)
			{
				hdr = (ft_alloc_hdr_t *)ft_block_data_ptr(block);
				user_ptr = ft_alloc_hdr_to_ptr(hdr);
				ft_puthex((uintptr_t)user_ptr);
				ft_putstr(" - ");
				ft_puthex((uintptr_t)user_ptr + hdr->size);
				ft_putstr(" : ");
				ft_putnbr(hdr->size);
				ft_putstr(" bytes\n");
				total += hdr->size;
			}
			block = block->next;
		}
		zone = zone->next;
	}
	return (total);
}

/*
** show_alloc_mem()
**
** Displays all allocated memory zones sorted by type (TINY, SMALL, LARGE).
** Shows each allocation's address range and size.
** Ends with total bytes allocated.
*/

void	show_alloc_mem(void)
{
	size_t	total;

	total = 0;
	total += ft_show_zone_type(g_zone_mgr.tiny_zones, "TINY");
	total += ft_show_zone_type(g_zone_mgr.small_zones, "SMALL");
	total += ft_show_zone_type(g_zone_mgr.large_zones, "LARGE");
	ft_putstr("Total : ");
	ft_putnbr(total);
	ft_putstr(" bytes\n");
}

