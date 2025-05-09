/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhenriqu <lhenriqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 07:40:26 by lhenriqu          #+#    #+#             */
/*   Updated: 2025/04/29 18:39:10 by lhenriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXING_H
# define LEXING_H

# include "minishell.h"

# define SYNTAX_ERROR "\033[36mminishell: \
\033[31msyntax error near unexpected token\033[0m `%s'\n"
# define QUOTE_ERROR "\033[36mminishell: \
\033[31munclosed quote:\033[0m `%c'\n"
# define EOF_ERROR "\033[36mminishell: \
\033[31msyntax error: unexpected\033[0m end of file\n"

// enum for controling the state of the current token
typedef enum e_state
{
	S_ERROR = -1,
	S_START,
	S_PIPE,
	S_AND,
	S_REDIR_IN,
	S_REDIR_OUT,
	S_WORD,
	S_SINGLE_QOUTE,
	S_QOUTE,
	F_OR,
	F_AND,
	F_REDIR_HDOC,
	F_REDIR_OUT_APP,
	F_WORD,
	F_PIPE,
	F_REDIR_IN,
	F_REDIR_OUT,
	F_OPEN_BRACKET,
	F_CLOSE_BRACKET,
}						t_state;

// enum for listing the types of tokens
typedef enum e_token_type
{
	TOK_INVALID,
	TOK_OR = 8,
	TOK_AND,
	TOK_REDIR_HDOC,
	TOK_REDIR_OUT_APP,
	TOK_WORD,
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_OPEN_BRACKET,
	TOK_CLOSE_BRACKET,
}						t_token_type;

typedef struct s_content_part
{
	char				*str;
	char				quote;
}						t_content_part;

typedef struct s_prop
{
	t_content_part		*content;
	size_t				size;
	t_bool				error;
}						t_prop;

typedef struct s_token
{
	t_token_type		type;
	char				*full_content;
	t_prop				props;
}						t_token;

typedef struct s_token_list
{
	t_token				token;
	struct s_token_list	*file;
	struct s_token_list	*prev;
	struct s_token_list	*next;
}						t_token_list;

// =========================== EXTERN FUNCTIONS ===============================

extern void				expand(t_token_list **token_list);

// =========================== TOKENIZATION PART ==============================
t_token_list			*get_token_list(char *input);

// ============== UTILS ===============
t_bool					is_word(char c);
t_bool					ft_isspace(char c);
char					*left_trim(char *input);
t_state					get_initial_state(char c);
t_state					get_seccond_state(char *input, t_state state);
void					fill_full_content(t_token *node);
void					assign_redirects(t_token_list *list);

// =========================== VALIDATION PART ================================
t_token_list			*validate_tokens(t_token_list *tokens);

// ============== UTILS ===============
t_bool					can_finish(t_token c);
t_bool					is_separator(t_token token);
t_bool					is_redirector(t_token token);
t_bool					is_bracket(t_token token);
void					*print_token_error(t_token_list *node);

#endif
