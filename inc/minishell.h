/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmynbyae <mynbyae@student.42prague.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 17:46:06 by bmynbyae          #+#    #+#             */
/*   Updated: 2025/05/28 13:45:29 by bmynbyae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include <stdint.h>
# include <errno.h>
# include <signal.h>

typedef enum e_token_type
{
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_WORD,
	TOKEN_SEMI
}	t_token_type;


typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_REDIR_OUT,
	NODE_REDIR_IN,
	NODE_REDIR_APPEND,
	NODE_HEREDOC
}	t_node_type;

typedef struct s_cmd
{
	char	*cmd;
	char	**args;
	char	*outfile;
	int		append;
	char	*infile;
	char	*heredoc;
}	t_cmd;

typedef struct s_ast
{
	t_node_type		type;
	struct s_ast	*left;
	struct s_ast	*right;
	t_cmd			*cmd;
}	t_ast;

typedef struct s_env
{
	char			*key;
	char			*val;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_token	*tokens;
	t_ast	*ast;
	t_env	*env;
	int		exit_status;
}	t_shell;

typedef struct s_parse_state
{
	const char	*line;
	int			i;
	int			start_of_token;
	int			token_in_progress;
}	t_parse_state;

typedef struct s_str_copy_params
{
	const char	*s1;
	const char	*s2;
	size_t		len1;
	int			add_newline;
}				t_str_copy_params;

typedef struct s_buffer_info
{
	char	**buffer;
	size_t	*current_size;
	size_t	*current_len;
}				t_buffer_info;

// Helper structure for populating args_array in create_cmd_node_from_tokens
typedef struct s_args_builder
{
	char    **args_array;
	char    *cmd_name_val;
	int     current_idx;
	int     error_flag;
} t_args_builder;


t_token		*parse_line(const char *input_line, t_shell *shell);
t_token		*init_token(const char *value);
t_env		*init_env(char **line);

void		free_token_list(t_token *token);
void		free_ast(t_ast *node);
void		add_token_back(t_token **head, t_token *new);

char		*get_env_value(t_env *env, const char *key);

int			ft_flag(const char *line, int pos);
int			ft_strcmp(const char *s1, const char *s2);
int			buildin_checker(char *cmd);

//	env
void		add_env_node(t_env **env_list, const char *key, const char *value);
t_env		*new_env_node(const char *key, const char *value);
t_env		*find_env_key(t_env *env, const char *key);
void		ft_free_env(t_env *env);

//	standart
void		ft_putstr(const char *s);
char		*str_append(const char *s1, const char *s2, int add_newline);
char		*ft_strdup(const char *s);
char		*ft_strndup(const char *s, size_t n);
char		*ft_strchr(const char *s, int c);
char		*read_q(char *line);
char		*ft_strncpy(char *dest, const char *src, size_t n);

//	build-in
int			find_buildin(t_shell *shell, t_cmd *cmd);
int			ft_env(t_shell *shell);
int			ft_echo(t_shell *shell, t_cmd *cmd);
void		print_export(t_env *env);
int			ft_export(t_shell *shell, t_cmd *cmd);
void		cleanup_shell(t_shell *shell);
int			ft_exit(t_shell *shell, t_cmd *cmd);
int			is_numeric(const char *s);
int			ft_unset(t_shell *shell, t_cmd *cmd);
void		change_env_value(t_env **env_list_ptr, char *key, char *value);
int			ft_cd(t_shell *shell, t_cmd *cmd);
int 		ft_pwd(t_shell *shell, t_cmd *cmd);
int 		ft_echo_status(t_shell *shell, t_cmd *cmd);

//	cmds
int			exec_external(t_shell *shell, t_cmd *cmd);
char		*find_command_path(char *cmd, t_env *envp);
int			is_valid_env_key(const char *key);
int			set_env_variable(t_env **env_list_ptr, \
	const char *key, const char *value);
int			exec_tree(t_shell *shell, t_ast *ast);
void		label_tokens(t_token *tokens);
t_ast		*build_ast_from_tokens(t_token *start, t_token *end);
void		free_command_resources(t_shell *shell);
void		finalize_and_add_token(t_token **list, t_parse_state *s, \
	t_shell *shell);
void		add_operator_as_token(t_token **list, const char *op_str);
int			process_quoted_segment(const char *l, int *i, char **r_str);
int			process_exit_status_expansion(int *i, char **r_str, t_shell *sh);
int			process_variable_expansion(const char *l, \
	int *i, char **r_str, t_shell *sh);
int			process_regular_char(const char *l, int *i, char **r_str);
char		*expand_vars(const char *line, t_shell *p_shell);
char		*strip_quotes(char *line);
void		process_line_character(t_parse_state *state, \
	t_token **token_list, t_shell *shell);
int			exec_pipe_node(t_shell *shell, t_ast *ast);
int			exec_redir_node(t_shell *shell, t_ast *ast);
int			exec_heredoc_node(t_shell *shell, t_ast *ast);
int			exec_cmd_node(t_shell *shell, t_ast *ast);
void		write_error(char *s1,char *s2,char *s3);


void 		print_tokens(t_token *tokens);
void 		print_ast(t_ast *node, int level);
int			exec_redir_chain(t_shell *shell, t_ast *ast);
char		*read_q(char *line);
char		*trim_spaces(char *str);
int			ft_isspace(char c);
void		delete_empty_value_nodes(t_token **head);
void 		free_shell(t_shell *shell);
void		add_new_env_node_to_list(t_env **env_list_ptr, t_env *new_node);
char		*ft_strdup_checked(char *str, char *err_msg);
char		*get_new_val_dup(char *value, char *key_to_free);
t_env		*create_new_env_node(char *key,char *value);
int			update_existing_env_value(t_env *env_list, char *key, char *value);
int			update_existing_value(t_env *node, const char *value);
t_env		*create_new_node(const char *key, const char *value);
int			dup_key_value(const char *key, const char *value,
			char **key_dup, char **value_dup);
int			is_n_option(const char *arg);
int	handle_export_with_args(t_shell *shell, t_cmd *cmd);
int	process_export_argument(t_shell *shell, char *arg);
int	handle_export_assignment(t_shell *shell, char *arg,
							char *equal, int key_len);
int remove_env_variable(t_env **env_list_ptr, const char *key_to_unset);
int	remove_env_node(t_env **head, t_env **curr,
					t_env **prev, const char *key);

char	*get_oldpwd(void);
int	check_cd_args(t_cmd *cmd, char **oldpwd, t_shell *shell);

int	calculate_new_buffer_size(size_t *size, size_t required);
int	ensure_buffer_capacity(t_buffer_info *buf_info,
				size_t additional_needed);
int	append_char_to_buffer(char c, t_buffer_info *buf_info);
int	append_string_to_buffer(const char *str, t_buffer_info *buf_info);
int	handle_dollar_question(int *line_idx_ptr, t_buffer_info *buf_info,
				int last_exit_status);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void free_argv_array(char **argv);
t_ast	*create_ast_node(t_node_type type);
t_ast	*create_cmd_node(char *cmd_name_allocated, char **args_allocated);
int	prepare_cmd_components(t_token *start, t_token *end,
	char **cmd_name_dup, char ***args_array);
t_ast	*create_cmd_node_from_tokens(t_token *start, t_token *end);
t_token	*find_last_pipe_token_in_segment(t_token *start, \
	t_token *end);
int	count_words_in_segment(t_token *start, t_token *end);
char	**populate_args_array(t_token *start, t_token *end,
			int word_count, t_args_builder *bs);
int	handle_redir_if_token(t_token *cur,
				t_token **op, t_token **file);
void	find_last_redir_info_in_segment(t_token *start,
				t_token *end,
				t_token **op_token_ptr,
				t_token **file_token_ptr);
t_ast	*init_redir_node_cmd_fields(t_ast *node);
t_node_type	get_redir_node_type_from_token(t_token_type token_type);
t_ast	*process_pipe_segment(t_token *s, \
	t_token *e, t_token *p_tok);
t_token	*copy_token_node(const t_token *orig);
int	attach_redir_child(t_ast *node,
				t_token *s, t_token *e,
				t_token *op_t, t_token *file_t);
int	set_redir_target(t_ast *node, t_token *file_t);
t_ast	*process_redir_segment(t_token *s, t_token *e,
				t_token *op_t, t_token *file_t);
t_ast	*create_redir_node_from_token(t_token *op_t);
t_token	*build_token_list_excluding_two(t_token *start, t_token *end,
	t_token *exclude1, t_token *exclude2);
int	open_input_redir(t_ast *node, int *fd, char *err_msg);
int	open_output_redir(t_ast *node, int *fd, char *err_msg);
int	do_dup_and_close(int fd, int target, char *err_msg, int *to_close);
int	handle_heredoc_redir(t_shell *shell, t_ast *node);
int	apply_single_redirection(t_shell *shell,
		t_ast *redir_node, int *fd_to_close_later);



#endif
