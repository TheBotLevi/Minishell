
#include "../../inc/minishell.h"

void	free_tokens(t_token *tokens)
{
    t_token	*current;
    t_token	*next;

    current = tokens;
    while (current)
    {
        next = current->next;
        free(current);
        current = next;
    }
}

void	free_middle_tokens_inclusive(t_token *start, t_token *end)
{
    t_token	*tmp;
    t_token	*next;

    start->prev = NULL;
    end->next = NULL;
    tmp = start;
    while (tmp)
    {
        next = tmp->next;
        free(tmp);
        if (tmp == end)
            break ;
        tmp = next;
    }
}

void free_redirections(t_redirect* redir) {
    t_redirect* tmp;

    while (redir) {
        tmp = redir->next;
        if (redir->filename)
            free(redir->filename);
        free(redir);
        redir = tmp;
    }
}

void	free_cmds(t_command *cmd)
{
    t_command	*tmp;

    if (!cmd)
        return ;
    while (cmd)
    {
        tmp = cmd->next;
        if (cmd->redirections)
            free_redirections(cmd->redirections);
        if (cmd->args)
            free_args(cmd->args);
        free(cmd);
        cmd = tmp;
    }
    /*while (cmd[i])
    {
        if (cmd[i]->argv)
            free_args(cmd[i]->argv);
        free(cmd[i]);
        i++;
    }*/
}