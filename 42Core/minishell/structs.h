/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 18:33:02 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/21 14:40:43 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct __attribute__((__packed__)) s_vars
{
	int				print;
	char			*value;
	char			*key;
	struct s_vars	*next;
}				t_vars;

typedef struct __attribute__((__packed__)) s_process
{
	int		child;
	int		tmpin;
	int		tmpout;
	int		fdout;
	int		fdin;
	pid_t	pid;
}				t_process;

typedef struct __attribute__((__packed__)) s_redir
{
	int	doc_flag;
	int	in;
	int	open_error;
	int	no_redirect;
}				t_redirect;

typedef struct __attribute__((__packed__)) s_token
{
	char			*value;
	int				type;
	struct s_token	*next;
	struct s_token	*prev;
}				t_token;

typedef struct __attribute__((__packed__)) s_shell
{
	int			n_locals;
	int			env_count;
	int			size;
	int			echo_flag;
	int			running;
	int			error;
	int			env_print;
	char		**path;
	char		**env;
	char		**cmds;
	t_vars		**local;
	t_vars		**env_vars;
	t_process	proc;
	t_redirect	redir;
}				t_shell;

#endif
