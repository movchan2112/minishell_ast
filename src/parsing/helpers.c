#include "../inc/minishell.h"

void print_tokens(t_token *tokens)
{
	char *token_type_str[] = {
		"TOKEN_PIPE",
		"TOKEN_REDIR_IN",
		"TOKEN_REDIR_OUT",
		"TOKEN_REDIR_APPEND",
		"TOKEN_HEREDOC",
		"TOKEN_WORD",
		"TOKEN_SEMI"
	};

	printf("Tokens list:\n");
	while (tokens)
	{
		if (tokens->type >= 0 && tokens->type <= TOKEN_SEMI)
			printf("Value: %-10s | Type: %s\n", tokens->value, token_type_str[tokens->type]);
		else
			printf("Value: %-10s | Type: UNKNOWN\n", tokens->value);
		tokens = tokens->next;
	}
}
void free_argv_array(char **argv) {
    if (!argv) return;
    for (int i = 0; argv[i]; i++) {
        free(argv[i]);
    }
    free(argv);
}

