#include "minishell.h"

char *ft_strndup(const char *s, size_t n)
{
    size_t len = 0;
    while (s[len] && len < n)
        len++;

    char *dup = malloc(len + 1);
    if (!dup)
        return NULL;

    for (size_t i = 0; i < len; i++)
        dup[i] = s[i];

    dup[len] = '\0';
    return dup;
}
char *ft_strdup(const char *s)
{
    size_t len = strlen(s);
    char *dup = malloc(len + 1);  // +1 for the null terminator

    if (!dup)
        return NULL;

    for (size_t i = 0; i <= len; i++)
        dup[i] = s[i];

    return dup;
}

t_env *new_env_node(const char *key, const char *value)
{
    t_env *node = malloc(sizeof(t_env));
    if (!node)
        return NULL;
    node->key = strdup(key);
    node->val = value ? strdup(value) : NULL;
    node->next = NULL;
    return node;
}

void add_env_node(t_env **env_list, const char *key, const char *value)
{
    t_env *new_node = new_env_node(key, value);
    if (!new_node)
        return;

    if (!*env_list)
    {
        *env_list = new_node;
        return;
    }

    t_env *current = *env_list;
    while (current->next)
        current = current->next;
    current->next = new_node;
}


t_env *init_env(char **line)
{
    t_env *head = NULL;
    int i = 0;

    while (line[i])
    {
        char *c = ft_strchr(line[i],'=');
        if(c)
        {
            int key_len = c - line[i];
            char *key = ft_strndup(line[i],key_len);
            char *val = ft_strdup(c+1);
            add_env_node(&head,key,val);
            free(key);
            free(val);
        }
        i++;
    }
    return(head);
}

char *get_env_value(t_env *env, const char *key)
{
    while (env)
    {
        if (strcmp(env->key, key) == 0)
            return env->val;
        env = env->next;
    }
    return NULL;
}
