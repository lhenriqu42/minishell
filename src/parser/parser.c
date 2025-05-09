/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosta-g <lcosta-g@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 11:07:30 by lhenriqu          #+#    #+#             */
/*   Updated: 2025/05/04 00:36:42 by lcosta-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexing.h"
#include "parser.h"

static t_token_list	*get_priority_token(t_token_list *token_list,
						t_tree_hierarchy hierarchy);
static t_exec_tree	*handle_redirect(t_exec_tree *tree, t_token_list *list);
static t_token_list	*extract_first_redirector(t_token_list **list);
t_tree_type			get_tree_type(t_token_type token_type);

t_exec_tree	*get_token_tree(t_token_list *token_list,
		t_tree_hierarchy hierarchy)
{
	t_token_list	*priority;
	t_exec_tree		*tree;

	if (!token_list)
		return (NULL);
	tree = ft_gc_malloc(sizeof(t_exec_tree));
	if (!tree)
		return (NULL);
	priority = get_priority_token(token_list, hierarchy);
	tree->type = get_tree_type(priority->token.type);
	if (priority->token.type == TOK_WORD)
		tree->command = ft_sublist(token_list, token_list, NULL);
	else if (is_bracket(priority->token))
		tree->subshell = get_token_tree(ft_sublist(token_list, token_list->next,
					ft_lstlast(token_list)), ROOT);
	else if (is_redirector(priority->token))
		tree->left = handle_redirect(tree, token_list);
	else
	{
		tree->left = get_token_tree(ft_sublist(token_list, token_list,
					priority), LEFT_CHILD);
		tree->right = get_token_tree(ft_sublist(token_list, priority->next,
					NULL), RIGHT_CHILD);
	}
	return (tree);
}

static t_exec_tree	*handle_redirect(t_exec_tree *tree, t_token_list *list)
{
	t_token_list	*new_list;
	t_token_list	*new_priority;

	new_list = ft_sublist(list, list, NULL);
	if (!new_list)
		return (NULL);
	new_priority = extract_first_redirector(&new_list);
	if (!new_priority)
		return (NULL);
	if (tree->type == TREE_REDIR_HDOC)
		tree->here_doc_fd = here_doc(new_priority->file->token.full_content);
	else
		tree->file = new_priority->file;
	tree->type = get_tree_type(new_priority->token.type);
	if (tree->here_doc_fd == -1)
	{
		get_shell()->heredoc_status = -1;
		return (NULL);
	}
	return (get_token_tree(new_list, LEFT_CHILD));
}

static t_token_list	*get_priority_token(t_token_list *token_list,
		t_tree_hierarchy hierarchy)
{
	t_token_list	*priority;
	t_token_list	*list;

	priority = NULL;
	if (hierarchy == ROOT || hierarchy == LEFT_CHILD)
		list = ft_lstlast(token_list);
	else
		list = token_list;
	if (is_subshell(token_list))
		return (list);
	while (list)
	{
		if (list->token.type == TOK_OPEN_BRACKET
			|| list->token.type == TOK_CLOSE_BRACKET)
			pass_brackets(&list, hierarchy);
		if (list->token.type == TOK_OR || list->token.type == TOK_AND)
			return (list);
		if (!priority || compare_priority(list->token.type,
				priority->token.type) == 1)
			priority = list;
		list = get_next_token(list, hierarchy);
	}
	return (priority);
}

static t_token_list	*extract_first_redirector(t_token_list **list)
{
	t_token_list	*temp;
	t_token_list	*prev;

	temp = *list;
	prev = NULL;
	while (temp)
	{
		if (is_redirector(temp->token))
		{
			if (prev)
				prev->next = temp->next;
			else
				*list = temp->next;
			if (temp->next)
				temp->next->prev = prev;
			return (temp);
		}
		prev = temp;
		temp = temp->next;
	}
	return (NULL);
}

t_tree_type	get_tree_type(t_token_type token_type)
{
	if (token_type == TOK_OR)
		return (TREE_OR);
	else if (token_type == TOK_AND)
		return (TREE_AND);
	else if (token_type == TOK_PIPE)
		return (TREE_PIPE);
	else if (token_type == TOK_REDIR_IN)
		return (TREE_REDIR_IN);
	else if (token_type == TOK_REDIR_OUT)
		return (TREE_REDIR_OUT);
	else if (token_type == TOK_REDIR_OUT_APP)
		return (TREE_REDIR_OUT_APP);
	else if (token_type == TOK_REDIR_HDOC)
		return (TREE_REDIR_HDOC);
	else if (token_type == TOK_OPEN_BRACKET || token_type == TOK_CLOSE_BRACKET)
		return (TREE_SUBSHELL);
	else
		return (TREE_CMD);
}
