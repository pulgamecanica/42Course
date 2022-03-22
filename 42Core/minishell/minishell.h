/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmachado <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 15:36:12 by bmachado          #+#    #+#             */
/*   Updated: 2022/02/23 14:35:13 by bmachado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include "libft/libft.h"
# include "macros.h"
# include "structs.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <signal.h>
# include <termios.h>
# include <curses.h>
# include <errno.h>

void	print_echo(char **str, t_shell *shell);
void	print_pwd(char **cmd);
void	print_env(t_list **env);
void	print_prompt(void);
void	handle_cd(t_shell *table, char **str);
void	free_vars(t_vars *var);
void	free_table(t_shell *table);
void	envtable(t_shell *table, char **environ);
void	hash_insert(t_shell *table, char *key, char *value);
void	hash_insert_env(t_shell *table, char *key, char *value);
void	print_table_env(t_shell *table);
void	tokenizer(t_token **token_lst, char *input);
void	clear_token_lst(t_token **token_lst);
void	add_token(t_token **token_lst, t_token *new_token);
void	expand_vars(t_token **token_lst, t_shell *table);
void	var_to_token(char *input, int i, t_token **token_lst);
void	pass_to_token(char *input, int i, t_token **token_lst);
void	close_open_pipes(t_shell *table, int *fd);
void	set_initial_input(t_shell *s);
void	restore_initial_input(t_shell *s);
void	dup_close_fds(t_shell *s, int type);
void	wait_for_child(int child_quantity);
void	add_quoted(char *input, t_token **token_lst);
void	add_after(char *input, int i, t_token **token_lst);
void	search_for_variable(char *str, t_token **token_lst);
void	interrupt(int signal);
void	exec_signal(void);
void	input_signal(void);
void	quit(int signal);
void	redisplay(int signal);
void	ft_export_env(t_shell *table, char *key, char *value, int set_print);
void	send_to_cmd(char **cmds, int i, char *joined, t_token *token);
void	exit_function(t_shell *table, char **cmds);
void	unset_function(t_shell *table, char **cmds);
void	unset(char *key, t_shell *table);
void	here_doc(char *limiter, t_shell *tab);
void	last_piped_cmd(t_shell *tab, char **cmds, int start, int end);
void	minishell_env(t_shell *table);
void	sort_env(char **sorted, t_shell *table);
void	open_file(char *file, int type, t_shell *table);
void	init_term(void);
void	free_cmd_lst(char **cmd_lst);
void	input_argv(char **argv, char **input);
void	exec_parsed_cmdlst(t_shell *tab, char **cmdlst, t_token **tkn_lst);
void	exit_process(t_shell *tab);
void	join_args(char **input, char *join);
void	cd_home(t_shell *table);

char	*join_before_value(char *cmd, char *var_ext);
char	*expanded_var(char *key, t_shell *table);
char	**create_cmd_lst(t_token **token_lst, char **cmd, int i);
char	**piped(char **cmds, int cmd_pos);
char	**redirected(t_shell *table, char **cmds, int start, int end);

int		is_whitespace(int c);
int		is_local_declare(char *var);
int		get_input(t_shell *shell, char **str);
int		get_position(t_shell *table, char *key);
int		get_position_env(t_shell *table, char *key);
int		is_var_repeated(t_shell *table, char *key);
int		is_in_env(char *key, t_shell *table);
int		is_var_call(char *key);
int		token_size(t_token *token_lst);
int		define_type(char c);
int		peek_next_one(char *input, int i);
int		only_char_before(char *input, int i);
int		is_empty_var(t_shell *table, char *str);
int		look_for_pipes(char *input, int type, int i, t_token **token_lst);
int		not_var_name(char *input, int i);
int		is_builtin(char *cmd);
int		do_builtin(char	**cmd, t_shell *table);
int		exec_pipe(t_shell *table, char **cmds);
int		is_chained_command(t_token **tk_lst);
int		execute_cmd(t_shell *table, char **cmds);
int		execute_shell(t_shell *table, char **cmds);
int		is_redir(char *input, int i);
int		pass_redir_to_token(int ret, t_token **token_lst);
int		split_redir(char *input, int i, t_token **token_lst);
int		is_operator(int type);
int		redir_type(int type);
int		parse_quote(char *input, int i, t_token **token_lst);
int		add_quoted_var(char *input, int i, t_token **token_lst);
int		split_singlequote(char *input, int i, t_token **token_lst);
int		split_doublequote(char *input, int i, t_token **token_lst);
int		split_equal(char *input, int i, t_token **token_lst);
int		is_quote(int input);
int		quoted_len(char *input, int i);
int		no_dollarsign(char *str);
int		export_function(char **cmd, t_shell *table);
int		is_number(char *num);
int		count_cmds(char **cmds);
int		define_exit_code(int input_code);
int		valid_quote(char *input);
int		error_msg(int type);
int		check_for_redir(char **cmds, int start, t_shell *table);
int		piped_cmd(t_shell *table, char **cmds, int start, int end);
int		in_redirected(char **cmds, t_shell *table);
int		split_type(int next, char *input, int i, t_token **token_lst);
int		create_locals(char *var, t_shell *table);
int		defdoc(int type);
int		redir_status(int i, t_shell *status);
int		redir_in_backwards(char **cmds, t_shell *table);
int		redir_in(char **cmds, int start, int end);
int		search_in_redirections(char **cmds, t_shell *tab);
int		check_operator(t_token *token);
int		passing_operators(t_token *token);
int		send_operator(t_token *token);
int		get_operator(t_token *token, char **cmd, int i);
int		check_pathexits(char *str);
int		free_tokenlst(t_token **token_lst);
int		set_cmd_lst(t_shell *table, t_token *token_lst, char *input);
int		invalid_assig(t_token **tk_lst, t_shell *table);

t_token	*create_token(char *value, int type);
t_token	*find_last_token(t_token *token);
t_token	**hold_tokenlst(t_token **lst);
t_shell	*create_table(int size);
t_vars	*create_vars(char *key, char *value);

t_shell	g_shell;

#endif
