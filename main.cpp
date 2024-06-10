#include <iostream>
#include <vector>
#include <pthread.h>
#include <mutex>
#include <unistd.h>
#include <sys/time.h>
#include <string>

class Philo {
private:
    int id;
    int nb_meals;
    size_t start_time;
    int time_ToDie;
    int time_ToEat;
    int time_ToSleep;
    int numberOfPhilosophers;
    std::mutex *leftFork;
    std::mutex *rightFork;
    std::mutex *printMutex;
    std::vector<Philo> *philos;
public:
    Philo();
    ~Philo();
    void eat();
    void think();
    void print(const std::string& message);
    void setId(int id) { this->id = id; }
    int getId() { return id; }
    void setTime_ToDie(int time_ToDie) { this->time_ToDie = time_ToDie; }
    void setTime_ToEat(int time_ToEat) { this->time_ToEat = time_ToEat; }
    void setTime_ToSleep(int time_ToSleep) { this->time_ToSleep = time_ToSleep; }
    void setLeftFork(std::mutex *leftFork) { this->leftFork = leftFork; }
    void setRightFork(std::mutex *rightFork) { this->rightFork = rightFork; }
    void setPrintMutex(std::mutex *printMutex) { this->printMutex = printMutex; }
    int getNb_meals() { return nb_meals; }
    int getTime_ToDie() { return time_ToDie; }
    int getTime_ToEat() { return time_ToEat; }
    int getTime_ToSleep() { return time_ToSleep; }
    std::mutex *getLeftFork() { return leftFork; }
    std::mutex *getRightFork() { return rightFork; }
    std::mutex *getPrintMutex() { return printMutex; }
    void setNb_meals(int nb_meals) { this->nb_meals = nb_meals; }
    void setNumberOfPhilosophers(int numberOfPhilosophers) { this->numberOfPhilosophers = numberOfPhilosophers; }
    int getNumberOfPhilosophers() { return numberOfPhilosophers; }
    void setPhilos(std::vector<Philo> *philos) { this->philos = philos; }
    std::vector<Philo> *getPhilos() { return philos; }
    void setStart_time(size_t start_time) { this->start_time = start_time; }
    size_t getStart_time() { return start_time; }
};

Philo::~Philo() {}

Philo::Philo() : id(0), nb_meals(0), time_ToDie(0), time_ToEat(0), time_ToSleep(0), numberOfPhilosophers(0), leftFork(nullptr), rightFork(nullptr), printMutex(nullptr), philos(nullptr) {}

size_t get_time() 
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void my_sleep(int time) 
{
    size_t start = get_time();
    while (get_time() - start < time)
        usleep(100);
}

void Philo::eat() 
{
    std::unique_lock<std::mutex> left(*leftFork);
    print("has taken a fork");
    std::unique_lock<std::mutex> right(*rightFork);
    print("has taken a fork");
    print("is eating");
    usleep(time_ToEat * 1000);
    print("has finished eating");
    nb_meals++;
}

void Philo::think() 
{
    print("is thinking");
    usleep(time_ToSleep * 1000);
}

void Philo::print(const std::string& message) 
{
    std::lock_guard<std::mutex> lock(*printMutex);
    size_t current_time = get_time() - start_time;
    std::cout << current_time << " " << id << " " << message << std::endl;
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


void *run(void* arg) 
{
    Philo* philo = static_cast<Philo*>(arg);
    if(philo->getId() % 2 == 0)
        usleep(100);
    while (true)
    {
        philo->eat();
        philo->think();
    }
    return NULL;
}

void create_philos(Philo *data) 
{
    int num_philos = data->getNumberOfPhilosophers();
    std::vector<Philo> philos(num_philos);
    std::vector<pthread_t> threads(num_philos);
    std::vector<std::mutex> forks(num_philos);
    std::mutex printMutex;

    for (int i = 0; i < num_philos; ++i) 
    {
        philos[i].setStart_time(get_time());
        philos[i].setNumberOfPhilosophers(num_philos);
        philos[i].setPhilos(&philos);
        philos[i].setId(i + 1);
        philos[i].setTime_ToDie(data->getTime_ToDie());
        philos[i].setTime_ToEat(data->getTime_ToEat());
        philos[i].setTime_ToSleep(data->getTime_ToSleep());
        philos[i].setNb_meals(data->getNb_meals());
        philos[i].setLeftFork(&forks[i]);
        philos[i].setRightFork(&forks[(i + 1) % num_philos]);
        philos[i].setPrintMutex(&printMutex);
    }

    for (int i = 0; i < num_philos; ++i) 
    {
        pthread_create(&threads[i], NULL, run, &philos[i]);
    }
    for (int i = 0; i < num_philos; ++i) 
    {
        pthread_join(threads[i], NULL);
    }
}

int main(int ac, char **av) 
{
    if (ac < 5 || ac > 6) 
    {
        std::cerr << "Usage: ./philo [number of philosophers] [time to die] [time to eat] [time to sleep] [number of meals]" << std::endl;
        return 1;
    }
    Philo data;
    init(&data, ac, av);
    create_philos(&data);

    return 0;
}
