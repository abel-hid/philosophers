/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 08:56:30 by abel-hid          #+#    #+#             */
/*   Updated: 2024/06/12 08:57:18 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.hpp"

std::mutex lastMealMutex;
std::mutex mealsMutex;

void *run(void* arg) 
{
    Philo* philo = static_cast<Philo*>(arg);
    if (philo->getId() % 2 == 0)
        usleep(100);
    while (!philo->getIs_dead())
    {
        philo->eat();
        philo->think();
    }
    return NULL;
}

int check_all_have_eaten(Philo *data) 
{
    for (int i = 0; i < data->getNumberOfPhilosophers(); ++i) 
    {
        mealsMutex.lock();
        if (data->getPhilos()->at(i)->getNumber_of_meals() != data->getNb_meals())
        {
            mealsMutex.unlock();
            return 0;
        }
        mealsMutex.unlock();
    }
    return 1;
}

void *Philo::monitor(void *arg) 
{
    Philo *data = static_cast<Philo*>(arg);
    size_t current_time;

    while (true) 
    {
        bool all_eaten = false;
        for (int i = 0; i < data->getNumberOfPhilosophers(); ++i) 
        {
            current_time = get_time();
            {
                lastMealMutex.lock();
                if (current_time - data->getPhilos()->at(i)->getLast_meal() > static_cast<size_t>(data->getPhilos()->at(i)->getTime_ToDie()))
                {
                    for (int j = 0; j < data->getNumberOfPhilosophers(); ++j)
                        data->getPhilos()->at(j)->setIs_dead(1);
                    (*data->getPhilos()->at(i)->getPrintMutex()).lock();
                    std::cout << current_time - data->getPhilos()->at(i)->getStart_time() << " " << data->getPhilos()->at(i)->getId() << " died" << std::endl;
                    (*data->getPhilos()->at(i)->getPrintMutex()).unlock();
                    data->setIs_dead(1);
                    lastMealMutex.unlock();
                    exit(0);
                }
                lastMealMutex.unlock();
                if(check_all_have_eaten(data))
                    all_eaten = true;
            }
            if(all_eaten)
            {
                for (int j = 0; j < data->getNumberOfPhilosophers(); ++j)
                    data->getPhilos()->at(j)->setIs_dead(1);
                return NULL;
            }
        }
    }
    return NULL;
}
void Philo::create_philos(Philo *data) 
{
    int num_philos = data->getNumberOfPhilosophers();
    std::vector<Philo*> philos(num_philos);
    std::vector<pthread_t> threads(num_philos);
    std::vector<std::mutex> forks(num_philos);
    std::mutex printMutex;

    for (int i = 0; i < num_philos; ++i) 
    {
        philos[i] = new Philo();
        philos[i]->setStart_time(get_time());
        philos[i]->setLast_meal(get_time());
        philos[i]->setNumberOfPhilosophers(num_philos);
        philos[i]->setPhilos(&philos);
        philos[i]->setId(i + 1);
        philos[i]->setTime_ToDie(data->getTime_ToDie());
        philos[i]->setTime_ToEat(data->getTime_ToEat());
        philos[i]->setTime_ToSleep(data->getTime_ToSleep());
        philos[i]->setNb_meals(data->getNb_meals());
        philos[i]->setLeftFork(&forks[i]);
        philos[i]->setRightFork(&forks[(i + 1) % num_philos]);
        philos[i]->setPrintMutex(&printMutex);
    }

    for (int i = 0; i < num_philos; ++i) 
    {
        pthread_create(&threads[i], NULL, run, philos[i]);
    }

    pthread_t monitor_thread;
    pthread_create(&monitor_thread, NULL, Philo::monitor, philos[0]);

    for (int i = 0; i < num_philos; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_join(monitor_thread, NULL);

    for (int i = 0; i < num_philos; ++i)
    {
        delete philos[i];
    }
}

int parse_args(Philo *data, int ac, char **av) 
{
    if(data->getIs_dead() < 0 || data->getNumberOfPhilosophers() < 0 || data->getTime_ToDie() < 0 
        || data->getTime_ToEat() < 0 || data->getTime_ToSleep() < 0)
        return 1;
    if(ac == 6)
    {
        if (std::stoi(av[5]) < 0)
            return 1;
    }
    return 0;
}


int main(int ac, char **av) 
{
    if (ac < 5 || ac > 6) 
    {
        std::cerr << "Usage: ./philo [number of philosophers] [time to die] [time to eat] [time to sleep] [number of meals]" << std::endl;
        return 1;
    }
    Philo data;
    try
    {
        init(&data, ac, av);
    }
    catch (std::exception &e)
    {
        std::cerr << "Invalid arguments" << std::endl;
        return 1;
    }
    if(parse_args(&data, ac, av))
    {
        std::cerr << "Invalid arguments" << std::endl;
        return 1;
    }
    data.create_philos(&data);
    return 0;
}
