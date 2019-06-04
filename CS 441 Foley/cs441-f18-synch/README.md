# CS441/541 Project 5

## Author(s):
    Ian Edward Neville
    Wanqian Huang

## Date:
    Created Date: Nov 1st.


## Description:
    This project will have a costume shop, and It will help with ninja and pirate's make up. Pirate and ninja are not allowed to be in costume shop together because they will try to kill each others, and ninja and pirate has different lines since they cannot meet with each others. Normally ninja will start first to use costume shop. After ninjas leave costume shop, waiting pirate will enter to get service. Ninja and pirate need to follow first in and first out rule in their lines.

## How to build the software
    make costume
    We use threads to simulate each ninja and pirate. We also have two threads. One for pirate lines, and one for ninja line. We also have binary semaphores for each ninja and pirate.

## How to use the software
    The number of arguments should be a number between 2 and 4. Time of live is required, and It's a positive integer number. The number of pirate is optional, and the default number is 5. The number of ninja is optional, and the default number is 5. The number of booth is optional, and the default number is 2. They all need to be a positive integer.

## How the software was tested
    ./costume
    It will print an error message.

    ./costume -1
    It will print an error message.

    ./costume 1 -1
    It will print an error message.

    ./costume 1 1 -1
    It will print an error message.

    ./costume 1 1 1 -1
    It will print an error message.

    ./costume 1 1 1 1 1
    It will print an error message.

    ./costume 5 5 1
    Ninja and Pirate are using different lines.
    If pirate's line has more than or equal two pirate, earliest two pirate will enter and use costume room one by one.
    If pirate's line has less than two pirate, all pirate will enter use costume shop.
    Same as ninja

    ./costume 5 5 1 10
    Since the number of pirate and the number of ninja are both less than the number of booth.
    All waiting ninja will enter costume shop one by one. After all ninja leave costume shop, all Waiting pirate will enter costume shop one by one...

    ./costume 5 5 5 1
    First ninja in ninja lines will enter costume shop first and leave. Then first pirate in pirate line will enter costume shop and leave...

    ./costume 5 5 1 100
    Since the number of pirate and the number of ninja are both less than the number of booth.
    All waiting ninja will enter costume shop one by one. After all ninja leave costume shop, all Waiting pirate will enter costume shop one by one...

    ./costume 5 100 5 1
    First ninja in ninja lines will enter costume shop first and leave. Then first pirate in pirate line will enter costume shop and leave...

    ./costume 5 5 100 1
    First ninja in ninja lines will enter costume shop first and leave. Then first pirate in pirate line will enter costume shop and leave...

## Known bugs and problem areas
    Its not nearly as efficient as Professor Foley's Example output.

## After Finish Coding

### 1. Solution to the problem
         In this project, we focus on two things.
            1. Ninjas and Pirates need to follow first in and first out rule.
            2. Pirate and Ninja cannot be in the costume department at the same time.
         We have one line of ninjas and one line of pirates. When a ninja/pirate arrive, they will enter the end of their respective line. When the costume department is empty, It will ask ninja to enter by order until costume department full or all ninjas are entered. After all ninja in costume department are left, costume department will allow pirates to enter, even if ninjas are waiting
### 2. Details
#### a. How to avoid deadlock
         Ninja and pirate has their own lines, and ninja will only enter costume shop when pirates are left and costume shop is empty. It had a deadlock when no ninjas or pirates were waiting, then we decided if no ninjas are waiting, we will give ninja's opportunity to pirate, and vice versa.

#### b. How to prevent pirate and ninjas from being in the costume department at the same Time
    with semaphores that lock and bar pirates from entering when ninjas have entered 
#### c. How to avoid depriving one side or the other access to the costume department? How is our solution "fair"?
    we use a FIFO queue system as well as alternating order for ninjas and pirates always allowing both of them two enter in groups one after another.
#### d. How to prevent "starvation"?
    the same as question c, we never starve a single pirate or ninja even if there are more ninjas waiting than pirates and vice versa
#### e. How to preserve the order of pirates/ninjas waiting on the costume department?
    witha FIFO queue determined by when threads arrive at the costume entrance
#### f. How does varying the number of booths impact the throughput of the system? Is it more or less fair with more booths and why?
    the average throughput is the same for all booth counts.

### 3. Describe how you verified that your implementation matched your design.
    we used algorithms from the Little Book of Semaphores, this consisted of studying the FIFO Barbershop synchronization problem along with the fairness Unisex Bathroom problem these two problems in combination were able to solve the ninjas and pirates problem effectively and fairly according order of arrival. We have one line of ninjas and one line of pirates. When a ninja/pirate arrive, they will enter the end of their respective line. When the costume department is empty, It will ask ninja to enter by order until costume department full or all ninjas are entered. After all ninja in costume department are left, costume department will allow pirates to enter, even if ninjas are waiting.
    Nmutex and Pmutex --- to protect global data
    Costume_Nmutex Costume_Pmutex --- to protect global data
    costume_shop_switch_pirate, costume_shop_switch_ninja --- this is a counting semaphore dependant on how many ninjas or pirates will be permitted in the costume department (depends on booths and how many ninjas or pirates are in the queue before entering the department).

### 4. Describe how you tested your implementation.
    ./costume 5 1 1 1
    It will print alternating ninja and pirate.

    ./costume 5 5 5 5
    It will print ninja and pirate is First in and First out.

    ./costume 5 1 1
    To test when the number of ninja and the number of pirate are less than the number of booth.

    ./costume 5 4 4
    To test when the number of ninja and the number of pirate are greater than the number of booth.

    ./costume 5 5 1
    To test when the number of ninja is greater than the number of booth, and the number of pirate is less than the number booth.

    ./costume 5 100 1
    To test when there are a lot of pirate and one ninja.
