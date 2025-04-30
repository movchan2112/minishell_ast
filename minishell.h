#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef enum e_token_type {
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_WORD,
	TOKEN_SEMI
}t_token_type;


typedef struct s_token {
	char *value;
	t_token_type type;
	struct s_token *next;
} t_token;


typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_REDIR_OUT,
	NODE_REDIR_IN,
	NODE_REDIR_APPEND,
	NODE_HEREDOC
} t_node_type;

typedef struct s_cmd
{
	char *cmd;
	char **args;
	char *outfile;
	int append;
	char *infile;
	char *heredoc;
} t_cmd;

typedef struct s_ast
{
	t_node_type type;
	struct s_ast *left;
	struct s_ast *right;
	t_cmd *cmd; // Только если type == NODE_CMD
} t_ast;

typedef struct s_env
{
    char *key;
    char *val;
    struct s_env *next;
}t_env;


t_token 	*parse_line(char *line);
t_token 	*init_token(const char *value);
t_env 		*init_env(char **line);

void 		add_token_back(t_token **head, t_token *new);

char 		*ft_strndup(const char *s, size_t n);
char		*ft_strchr(const char *s, int c);
char 		*get_env_value(t_env *env, const char *key);

int			ft_flag(const char *line, int pos);
int 		ft_strcmp(const char *s1, const char *s2);
int 		buildin_checker(char *cmd);


#endif