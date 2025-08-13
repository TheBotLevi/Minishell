#include "../../inc/minishell.h"

//todo change signature
void	free_heredocs(t_redirect *redir)
{
    t_redirect	*tmp;

    while (redir)
    {
        tmp = redir->next;
        if (redir->filename)
            free(redir->filename);
        free(redir);
        redir = tmp;
    }
}
