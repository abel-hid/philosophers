/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 08:56:47 by abel-hid          #+#    #+#             */
/*   Updated: 2024/06/12 08:56:48 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.hpp"
#include <sys/time.h>

size_t get_time() 
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void my_sleep(size_t time) 
{
    size_t start = get_time();
    while (get_time() - start < time)
        usleep(100);
}

void init(Philo *data, int ac, char **av) 
{
    data->setNumberOfPhilosophers(std::stoi(av[1]));
    data->setTime_ToDie(std::stoi(av[2]));
    data->setTime_ToEat(std::stoi(av[3]));
    data->setTime_ToSleep(std::stoi(av[4]));
    if (ac == 6)
        data->setNb_meals(std::stoi(av[5]));
    else
        data->setNb_meals(-1);
}
