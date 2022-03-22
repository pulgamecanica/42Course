/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 11:40:47 by bmachado          #+#    #+#             */
/*   Updated: 2021/12/15 15:22:34 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	defdoc(int type)
{
	int	flags;

	flags = 42;
	if (type == CREATDOC)
		flags = O_WRONLY | O_CREAT | O_EXCL | O_TRUNC;
	else if (type == CLEARDOC)
		flags = O_WRONLY | O_TRUNC;
	return (flags);
}
