

# ifndef PHILO_HPP
# define PHILO_HPP


# include <unistd.h>
# include <iostream>
# include <string>
# include <vector>
# include <thread>
# include <mutex>
# include <chrono>

extern std::mutex lastMealMutex;
extern std::mutex mealsMutex;

class Philo {
private:
    int id;
    int nb_meals;
    size_t start_time;
    size_t last_meal;
    int time_ToDie;
    int time_ToEat;
    int time_ToSleep;
    int numberOfPhilosophers;
    std::mutex *leftFork;
    std::mutex *rightFork;
    std::mutex *printMutex;
    std::mutex timeMutex;
    std::vector<Philo*> *philos;
    int number_of_meals;
    std::atomic<int> is_dead;
public:
    std::mutex deadMutex;
    Philo();
    ~Philo();
    void eat();
    static void *monitor(void *arg) ;
    void think();
    void print(const std::string& message);
    void setId(int id);
    int getId();

    void setTime_ToDie(int time_ToDie);
    void setTime_ToEat(int time_ToEat);
    void setTime_ToSleep(int time_ToSleep);
    void setLeftFork(std::mutex *leftFork);


    void setRightFork(std::mutex *rightFork);
    void setPrintMutex(std::mutex *printMutex)
    {
        this->printMutex = printMutex;
    }
    int getNb_meals();
    int getTime_ToDie();
    int getTime_ToEat();
    int getTime_ToSleep();
    std::mutex *getLeftFork();
    std::mutex *getRightFork();
    std::mutex *getPrintMutex()
    {
        return printMutex;
    }
    void setNb_meals(int nb_meals);
    void setNumberOfPhilosophers(int numberOfPhilosophers);
    int getNumberOfPhilosophers();
    std::vector<Philo*> *getPhilos()
    {
        return philos;
    }
    size_t getStart_time();
    void setPhilos(std::vector<Philo*> *philos);
    void setStart_time(size_t start_time);
    void setNumber_of_meals(int number_of_meals);
    void setIs_dead(int is_dead);
    void setLast_meal(size_t last_meal);
    void create_philos(Philo *data);
    int getIs_dead();
    int getNumber_of_meals();
    size_t getLast_meal();
};

size_t get_time();
void my_sleep(size_t time);
void init(Philo *data, int ac, char **av);

# endif