/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosta-g <lcosta-g@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 09:46:18 by lhenriqu          #+#    #+#             */
/*   Updated: 2025/05/04 00:52:13 by lcosta-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "lexing.h"
# include "minishell.h"

// Expand the variables and wildcards changing the token list.
void			expand(t_token_list **token_list);

// ================================= EXPANDERS ================================

// Expands the variables in the token list.
void			expand_vars(t_content_part *part);
// Expands the wildcards in the token list.
t_token_list	*expand_wildcards(t_token *token, t_token_list **list);
// Transform wildcards in a special character
// to be used in the pattern matching.
void			prepare_wildcard(t_content_part *part);
// Transform the special character back to the wildcard.
void			finish_wildcard(t_token *token);

// ================================= UTILS ====================================

// Returns the previous token list node that contains the specified token.
t_token_list	*get_left_token(t_token_list *list, t_token *token);
// Returns the token list node that contains the specified token.
t_token_list	*get_current_token(t_token_list *list, t_token *token);
// Returns TRUE if the pattern matches the filename, otherwise FALSE.
t_bool			match_wildcard(const char *pattern, const char *filename);

#endif
