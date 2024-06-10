#include <iostream>
#include <vector>
#include <pthread.h>
#include <mutex>
#include <unistd.h>
#include <sys/time.h>
#include <string>
#include <atomic>

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
    std::mutex mealsMutex;
    std::mutex *printMutex;
    std::mutex timeMutex;
    std::vector<Philo> *philos;
    int number_of_meals;
    std::atomic<int> is_dead;
public:
    std::mutex deadMutex;
    std::mutex lastMealMutex;
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
    void setNumber_of_meals(int number_of_meals) { this->number_of_meals = number_of_meals; }
    int getNumber_of_meals() { return number_of_meals; }
    void setIs_dead(int is_dead) { this->is_dead.store(is_dead); } // Use atomic store
    int getIs_dead() { return this->is_dead.load(); } // Use atomic load
    void create_philos(Philo *data);
    void setLast_meal(size_t last_meal) { this->last_meal = last_meal; }
    size_t getLast_meal() 
    {
        return last_meal; 
    }
};

Philo::~Philo() {}

Philo::Philo() : id(0), nb_meals(0), time_ToDie(0), time_ToEat(0), time_ToSleep(0), numberOfPhilosophers(0), leftFork(nullptr), rightFork(nullptr), printMutex(nullptr), philos(nullptr) 
{
    number_of_meals = 0;
    start_time = 0;
    is_dead = 0;
}

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


void Philo::eat() 
{
    leftFork->lock();
    print("has taken a fork");
    rightFork->lock();
    print("has taken a fork");
    print("is eating");
    my_sleep(time_ToEat);
    leftFork->unlock();
    rightFork->unlock();
    
    // Lock access to last_meal using last_meal_mutex
    std::lock_guard<std::mutex> lock(lastMealMutex);
    last_meal = get_time();
    
    if (nb_meals != -1) 
    {
        std::lock_guard<std::mutex> lock(mealsMutex);
        number_of_meals++;
    }
}

void Philo::think() 
{
    print("is sleeping");
    my_sleep(time_ToSleep);
    print("is thinking");
}

void Philo::print(const std::string& message) 
{
    if (is_dead.load())
        return;
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
    if (philo->getId() % 2 == 0)
        usleep(100);
    while (!philo->getIs_dead())
    {
        philo->eat();
        if (philo->getNumber_of_meals() == philo->getNb_meals() && philo->getNb_meals() != -1 && philo->getIs_dead() == 0)
            break;
        philo->think();
    }
    return NULL;
}

void *monitor(void *arg) 
{
    Philo *data = static_cast<Philo*>(arg);
    size_t current_time;

    while (true) 
    {
        bool all_eaten = true;
        for (int i = 0; i < data->getNumberOfPhilosophers(); ++i) 
        {
            current_time = get_time();
            data->deadMutex.lock();
            data->lastMealMutex.lock();
            if (current_time - data->getPhilos()->at(i).getLast_meal() > static_cast<size_t>(data->getTime_ToDie()))
            {
                std::lock_guard<std::mutex> lock(*data->getPrintMutex());
                for (int j = 0; j < data->getNumberOfPhilosophers(); ++j) 
                {
                    data->getPhilos()->at(j).setIs_dead(1);
                }
                std::cout << current_time - data->getPhilos()->at(i).getStart_time() << " " << data->getPhilos()->at(i).getId() << " died" << std::endl;
                data->lastMealMutex.unlock();
                data->deadMutex.unlock();
                return NULL;
            }
            data->lastMealMutex.unlock();
            data->deadMutex.unlock();
            if (data->getPhilos()->at(i).getNumber_of_meals() != data->getNb_meals() && data->getNb_meals() != -1)
                all_eaten = false;
        }
        if (all_eaten && data->getNb_meals() != -1) 
        {
            std::lock_guard<std::mutex> lock(*data->getPrintMutex());
            std::cout << "All philosophers have eaten " << data->getNb_meals() << " times" << std::endl;
            for (int j = 0; j < data->getNumberOfPhilosophers(); ++j) 
            {
                data->getPhilos()->at(j).setIs_dead(1);
            }
            return NULL;
        }
    }
}

void Philo::create_philos(Philo *data) 
{
    int num_philos = data->getNumberOfPhilosophers();
    std::vector<Philo> philos(num_philos);
    std::vector<pthread_t> threads(num_philos);
    std::vector<std::mutex> forks(num_philos);
    std::mutex printMutex;

    for (int i = 0; i < num_philos; ++i) 
    {
        philos[i].setStart_time(get_time());
        philos[i].setLast_meal(get_time());
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

    pthread_t monitor_thread;
    pthread_create(&monitor_thread, NULL, monitor, &philos[0]);

    for (int i = 0; i < num_philos; ++i) 
    {
        pthread_join(threads[i], NULL);
    }
    pthread_detach(monitor_thread);
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
    data.create_philos(&data);
    return 0;
}
