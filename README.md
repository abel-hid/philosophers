# Philosopher Project 42

Philosopher Project 42 is a simulation based on the classic dining philosophers problem. It aims to demonstrate concurrent programming concepts using threads and mutexes to manage resource allocation and avoid deadlock situations.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)

## Introduction

In this project, philosophers sit at a round table with a bowl of spaghetti in the center. They alternate between eating, thinking, and sleeping. Each philosopher requires two forks to eat, and forks are shared between neighboring philosophers. The simulation ensures that no philosopher starves and manages the synchronization of forks using threads and mutexes.

## Features

- **Concurrency Management:** Demonstrates thread creation and synchronization using mutexes.
- **Real-time Simulation:** Shows the real-time status of philosophers (eating, thinking, sleeping).
- **Starvation Prevention:** Ensures no philosopher starves during the simulation.
- **Configurable Parameters:** Allows customization of the number of philosophers and timings for eating, sleeping, and starving.

## Installation

To install and run Philosopher Project 42, follow these steps:

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/abel-hid/philosophers.git
    cd philosophers
    ```
## Usage

To run the simulation, execute the following command:

```bash
make
./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat]
```
Replace the placeholders with the desired values for the simulation:

- `number_of_philosophers`: The number of philosophers sitting at the table.
- `time_to_die`: The time in milliseconds after which a philosopher dies if they have not started eating.
- `time_to_eat`: The time in milliseconds it takes for a philosopher to eat.
- `time_to_sleep`: The time in milliseconds it takes for a philosopher to sleep.
- `number_of_times_each_philosopher_must_eat`: The number of times each philosopher must eat before the simulation ends.


