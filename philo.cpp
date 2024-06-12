/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 08:56:43 by abel-hid          #+#    #+#             */
/*   Updated: 2024/06/12 08:56:44 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.hpp"

Philo::~Philo() {}

Philo::Philo() : id(0), nb_meals(0), time_ToDie(0), time_ToEat(0), time_ToSleep(0), numberOfPhilosophers(0), leftFork(nullptr), rightFork(nullptr), printMutex(nullptr), philos(nullptr) 
{
    number_of_meals = 0;
    start_time = 0;
    is_dead = 0;
}

void Philo::setId(int id)
{
    this->id = id;
}

int Philo::getId()
{
    return id;
}

void Philo::setTime_ToDie(int time_ToDie)
{
    this->time_ToDie = time_ToDie;
}

void Philo::setTime_ToEat(int time_ToEat)
{
    this->time_ToEat = time_ToEat;
}

void Philo::setTime_ToSleep(int time_ToSleep)
{
    this->time_ToSleep = time_ToSleep;
}

void Philo::setNumberOfPhilosophers(int numberOfPhilosophers)
{
    this->numberOfPhilosophers = numberOfPhilosophers;
}

int Philo::getNumberOfPhilosophers()
{
    return numberOfPhilosophers;
}

void Philo::setLeftFork(std::mutex *leftFork)
{
    this->leftFork = leftFork;
}

void Philo::setRightFork(std::mutex *rightFork)
{
    this->rightFork = rightFork;
}


int Philo::getNb_meals()
{
    return nb_meals;
}
int Philo::getTime_ToDie()
{
    return time_ToDie;
}
int Philo::getTime_ToEat()
{
    return time_ToEat;
}
int Philo::getTime_ToSleep()
{
    return time_ToSleep;
}
std::mutex *Philo::getLeftFork()
{
    return leftFork;
}

std::mutex *Philo::getRightFork()
{
    return rightFork;
}

void Philo::setNb_meals(int nb_meals)
{
    this->nb_meals = nb_meals;
}
void Philo::eat() 
{
    if (nb_meals != -1) 
    {
        mealsMutex.lock();
        if (number_of_meals >= nb_meals)
        {
            mealsMutex.unlock();
            return;
        }
        mealsMutex.unlock();
    }
    std::lock_guard<std::mutex> lock(*leftFork);
    print("has taken a fork");
    std::lock_guard<std::mutex> lock2(*rightFork);
    print("has taken a fork");
    print("is eating");
    my_sleep(time_ToEat);
    // std::this_thread::sleep_for(std::chrono::milliseconds(time_ToEat));

    lastMealMutex.lock();
    last_meal = get_time();
    lastMealMutex.unlock();
    
    if (nb_meals != -1) 
    {
        mealsMutex.lock();
        number_of_meals++;
        mealsMutex.unlock();
    }
}


void Philo::setPhilos(std::vector<Philo*> *philos)
{
    this->philos = philos;
}
void Philo::setStart_time(size_t start_time)
{
    this->start_time = start_time;
}
void Philo::setNumber_of_meals(int number_of_meals)
{
    this->number_of_meals = number_of_meals;
}
void Philo::setIs_dead(int is_dead)
{
    this->is_dead.store(is_dead);
}
void Philo::setLast_meal(size_t last_meal)
{
    this->last_meal = last_meal;
}
size_t Philo::getStart_time()
{
    return (start_time);
}

int Philo::getIs_dead()
{
    return (is_dead.load());
}
int Philo::getNumber_of_meals()
{
    return (number_of_meals);
}
size_t Philo::getLast_meal() 
{
    return (last_meal);
}

void Philo::think() 
{
    print("is sleeping");
    my_sleep(time_ToSleep);
    // std::this_thread::sleep_for(std::chrono::milliseconds(time_ToSleep));
    print("is thinking");
}

void Philo::print(const std::string& message) 
{   
    if (is_dead.load())
        return;
    (*printMutex).lock();
    size_t current_time = get_time() - start_time;
    std::cout << current_time << " " << id << " " << message << std::endl;
    (*printMutex).unlock();
}