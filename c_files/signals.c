/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azhadan <azhadan@student.42lisboa.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 10:51:06 by azhadan           #+#    #+#             */
/*   Updated: 2023/10/23 10:51:45 by azhadan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/minishell.h"

void handle_sigint(int signum) {
    (void)signum;
    write(1, "\nminishell> ", 12);
}

void handle_sigquit(int signum) {
    (void)signum;
}
