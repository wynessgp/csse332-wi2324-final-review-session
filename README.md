# CSSE332-Final-Exam-Review-Session

## How to download and edit the files here:
First, create a new directory on your WSL. You can do this by running the following command:
``` 
mkdir <directory name>
```
Once you've done that, go ahead and cd into the directory. Then, you'll want to go ahead and click on the 'code' button on the main page. Select the ssh button, and then copy to clipboard. I believe the link should be the same for everyone, so you can also just go ahead and copy this:
```
git@github.com:wynessgp/CSSE332-Final-Exam-Review-Session.git
```
Once you are in your newly created directory and have the above copied into your clipboard, go back to your terminal. Once there, go ahead and type:
```
git clone git@github.com:wynessgp/CSSE332-Final-Exam-Review-Session.git
```
And that should go ahead and move all of the files over to your WSL. 

If you are having errors with GitHub saying it cannot authenticate you, you'll need to create a SSH key. You can either follow the instructions on [GitHub's website](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent) to generate one, or you can follow Comp Arch 1's tutorial [here](comp-arch-ssh-tutorial.md). If that is not the error, I'd suggest following whatever the console spits out - those hints usually tell you everything you need.

I'd recommend leaving this README page up on [GitHub](https://github.com/wynessgp/CSSE332-Final-Exam-Review-Session) and tabbing back and forth for instructions. Like before, I have provided you with some nice navigability links to jump around.

## A quick request
After writing three different practice exams, I'd like to gather some feedback about how I've been doing. 

If you are willing to give up a few minutes of your time, I'd appreciate you filling out the form below - you will be my favorite student if you do, just don't tell anyone else I said that!

Click [here](https://forms.gle/N2U9ViV65QEQpU446) for the form 

It's entirely anonymous, so feel free to be brutally honest. I'd find any thoughts on the following particularly helpful (also on the form):
- Did you find the question difficulty to be roughly the same as the exams? 
- How many review sessions did you physically attend?
- Did you find any of the review sessions more helpful than others?
- Are there any topics that finally *clicked* for you as a result of attending a review session and getting help?

All of the questions are optional, so if you wish to answer just a few of them and not others, that's an option. You can write as much or as little as you like - it's surprisingly tough writing practice exams, so any and all feedback helps.

(And yes, before you ask, OS has about as much of a toll on us SRTs and the professors as it does on you guys, there's a lot that can go wrong)

<a id="toc"></a>

## Table of Contents
Reminder that these links will only work if you're viewing this in a markdown renderer, or if you're on GitHub. I'd highly suggest using one of those.

[Exam 1 Content Notes](#exam_one_notes) <br>
[Exam 2 Content Notes](#exam_two_notes) <br>
[Problem 1](#problem_one) <br>
[Problem 2](#problem_two) <br>

<a id="admin_notes"></a>

## *Administrative* Notes
This GitHub repository, once again, will only contain the questions for a practice exam - the solutions will NOT be posted here. I've been explicitly asked not to post them here, as it will make the sample solutions public. You are more than welcome to ask me for hints, but please keep in mind I am also a student, and have finals. The best time to ask me questions is DURING the review session - that's time I've set aside for you to explicitly practice and ask me questions!

***Please respect my time off, especially as we go into finals week. If I do not respond to your first teams message, I will not respond if you send a billion more.***

That being said, let's talk about your final exam. 

For this exam, there is a **2** hour time limit, longer if you have accomodations. It will have **two** questions, with **two** parts for each. 

The first question will be dedicated to concepts you learned for Exam 1, the second question will be dedicated to concepts you learned for Exam 2. So, these questions will be on topics roughly as follows:
- Question 1: 
    - fork
    - exec 
    - pipe 
    - linux signals 
    - wait
- Question 2: 
    - threads 
    - conditional variables
    - signal, broadcast
    - safely utilizing shared memory space 

Additionally, you will be connecting to the remote server for this exam. This means no notes unless you've been allowed to move your repository over, so get comfortable with man pages! 

Also, make sure you get plenty of sleep before the exam - it is an 8am final! 

[Back to table of contents](#table-of-contents)

<a id="exam_one_notes"></a>

## Exam One *Content* Notes
Exam 1 primarily utilized fork, and communication between various different processes generated by fork. Since it's been a bit, I've beefed up the notes from my first practice exam and put them here.

### On Using `fork()`
Remember that the fork() command creates an entirely new process, but it creates a context that is **identical** to the currently running process, but it is also **ISOLATED** from the current process. It spawns this new process on the exact like that `fork()` is called on, meaning `fork()` essentially returns twice - once with the parent's process ID, and once with the child's process ID (which will be 0). 

The fact that the child process returns with `pid = 0` on Linux is how we can identify which process is which - otherwise creating diverging control flows (fancy terms for how tasks are run) would be more difficult. You might just remember how to make child processes run different code via a pattern, but this is how that pattern actually developed. Supposed on a different operating system `fork()` returned `pid = 12` for a child process - you'd change the child's part of an if statement to simply reflect that. 

Now that we've talked about why `fork()` calls tend to follow a pattern, here's the general code style we suggest when using `fork()`:
```
int pid = fork();
if (pid < 0) {
    perror("fork error");
    exit(1); // ensures the process exits if we do error
} else if (pid == 0) { // child process exclusive code
    // do something...
    exit(EXIT_SUCCESS); // makes sure the child process eventually exits
} else { // parent process exclusive code 
    // do something
}
```
Note that you might not always have an `else` block with your `fork()` calls; it isn't mandatory. You should, however, make a point to always do error handling. You don't want something to go horifically wrong during an exam and have absolutely no idea what caused it.

Generally speaking, you'd want some code to make the parent process `wait(...)` for the child process, but I'll discuss that more later.

[Back to table of contents](#table-of-contents)

### On Using `exec(...)`
It is **VERY** important to note that exec will replace the **CURRENT** process with whatever else you are attempting to `exec`. This is why you usually see any `exec()` calls paired with a `fork()` call just before it - otherwise we just throw out the code we're attempting to run, which defeats the point. That's not to say that couldn't be the point of a program, but we typically don't have you throw away the code you're writing in this class. 

Much like with `fork()`, we suggest you error-check any `exec()` calls you make by simply throwing a `perror("<msg>")` call **AFTER** the `exec()` call - something like this:
```
// some code here...
exec(...);
perror("exec failed, something bad is happening!");
exit(EXIT_FAILURE);
// ...
```
Since `exec` generally relies on the other process exiting normally, if you manage to reach the `perror` afterwards, you know something went wrong, and can make that process exit on your accord instead of creating a zombie process. 

### On Using `pipe()`
Pipes are meant to be the primary means through which processes communicate with each other. Think about it - with completely isolated processes after calling `fork()`, how are you actually meant to get a message from one process to another? The only feasible way is through a shared file; and that's precisely what pipes are. 

Remember that a `pipe` is a **unidirectional** channel of communication between two or more processes that share a common ancestor; that being the process who set up the pipe. So one end is strictly the **read** end (index 0) and one end is strictly the **write** end (index 1). To preserve the **unidirectional** nature of pipes, you should always close the end you are NOT going to use at the start of a particular process's code. If two processes are to communicate back and forth, you should utilize more than one pipe. (Things can get messy if you are setting up a chain of processes - this may have led to some confusion at my first practice exam...)

Additionally, it is vital that you call `pipe(...)` before you call `fork()`, otherwise the pipes will not get initialized correctly. 

One final thing to remember - `read()` and `write()` are **blocking** calls - what this means is that your program will stop in its tracks if you attempt to `read` say, 128 bytes from a pipe, but there are only 120 bytes' worth of data on the pipe. I have seen this error more than I care to admit to. Be vigilent of your data sizes!!!!

### On Using `wait()`
A good parent process should always wait for its child processes - if you don't, you'll end up with processes running in the background forever. Have you ever been careless with where you call `fork()` and end up creating a ton of extra processes? You probably noticed your laptop started running really slowly afterwards - that's how you fork bomb yourself. Don't do that. I don't think you want to restart your laptop during a 2 hour long exam, that's precious time down the drain. <br><br>
Using `wait()` in the appropriate places ensures that the child processes **PROPERLY** exit before the parent process exits. `wait()` also makes sure that the parent receives the appropriate data from the child process before attempting to do something with that data. It also enables the parent to see if the child process exited for any particular reason via the exit status. Odds are if you don't use a combination of `wait()` and `exit()` appropriately, you'll end up with defunct processes, which looks very bad to anyone grading.

[Back to table of contents](#table-of-contents)

<a id="exam_two_notes"></a>

## Exam Two *Content* Notes
Threads, unlike processes, have **SHARED** global memory... more specifically, they share a code section, data section, and some OS resources (opened files, signals, etc). However, threads do happen to have unique stacks, so local variables are largely unaffected.

### On Global Variables
This is why we have to be so cautious about how we utilize global variables. If there's a chance that another thread may need to access what your current thread is looking at, you'll need to use a lock to prevent an inconsistent state between the two separate threads. You don't want to have a thread actively updating a variable's value while another is doing a comparison with it - one should be resolved before another, otherwise you'll get incorrect behavior. So unless if you can guarantee that your threads will not overlap in **ANY** way, make sure you **LOCK!** 

Most of the time, you will need locks. The only situation I can possibly think of where you won't need one is if you can guarantee that threads access only their own specified index within an array. The original thread can then come back and clean up the data after the child threads are done.

[Back to table of contents](#table-of-contents)

### On Critical Sections
In order to have threads really run anything in parallel, it is imperative that you **UNLOCK** as soon as you are done with any thread overhead code - that way it gives other threads an opportunity to actually run the relevant code in the **CRITICAL SECTION**. If you do not unlock, your threads will run in a serial manner as opposed to a parallel one, which is WAY slower. I have written a small example to show this off, not unlike what you guys did for max in conditional variable basics: 

Properly unlocking example:
```
$ ./with_unlocks.bin
timing threads starting now!
Thread 1 printing!
Thread 4 printing!
Thread 5 printing!
Thread 2 printing!
Thread 3 printing!
Thread 7 printing!
Thread 8 printing!
Thread 6 printing!
Thread 11 printing!
Thread 9 printing!
Thread 13 printing!
Thread 10 printing!
Thread 15 printing!
Thread 12 printing!
Thread 14 printing!
Thread 16 printing!
Thread 17 printing!
Thread 18 printing!
Thread 19 printing!
Thread 20 printing!
Program took 3178 usecs
```
Improperly unlocking example:
```
$ ./without_unlocks.bin
timing threads starting now!
Thread 1 printing!
Thread 2 printing!
Thread 3 printing!
Thread 5 printing!
Thread 4 printing!
Thread 6 printing!
Thread 7 printing!
Thread 8 printing!
Thread 9 printing!
Thread 10 printing!
Thread 12 printing!
Thread 13 printing!
Thread 11 printing!
Thread 14 printing!
Thread 16 printing!
Thread 15 printing!
Thread 17 printing!
Thread 19 printing!
Thread 18 printing!
Thread 20 printing!
Program took 12454 usecs
```
As you can see, that's an almost 4x boost to performance by properly unlocking. If you want to tinker with this a bit, I have included my benchmark programs in the ```benchmarks``` folder in the GitHub. There is a makefile provided so you can change the ```#define``` statements at the top as you may wish. Note that there is only a 2 line difference between these files!

(Yes, I have included these benchmarks with the final exam repo as well)

[Back to table of contents](#table-of-contents)

### On Using `pthread_cond_signal()`, `pthread_cond_broadcast()`
Sometimes we will ask you to implement things that expect multiple threads to be running a specific way all at once. Think tunnel, three jobs, and the like. If you cannot guarantee exactly how many threads will be waiting in a queue, and you'd like for **MULTIPLE** of the waiting ones to start running at the same time, it is in your best interest to use *broadcast*. Even if you have multiple threads currently running, if there's any chance you might have *MORE* after this one, you should *broadcast*.

In the case of ```tunnel```, think about an ambulance interrupting normal traffic flow - cars might start piling up at the EW entrance, so when that ambulance leaves, it should *broadcast* to those EW cars. (This is a bit of an oversimplifiction, but you get the point) If it alerts only one car with signal, then suddenly the EW side of the tunnel shrinks to one lane! 

As far as signal goes, this is for a *targeted* number of threads. *Signal* itself only wakes up one thread in a queue - so if you have multiple threads running a *signal* call, you'll end up waking up however many corresponding threads. You can also have just one thread call it multiple times. 

It is important to note that these can both generate viable solutions to a problem depending on how you make threads wait, but switching between the two can sometimes vastly reduce the amount of code you have for a problem. 

An important thing to note is that these will **NOT** error if there are no threads waiting in a queue, it just simply won't wake anyone up. 

Additionally, you should aim to use both of these calls while you still have a lock. Why? Most of the time a program will still have correct behavior regardless if you signal while locked or unlocked, but if you want to 100% guarantee correctness, it is **safer** to use broadcast and signal while you are holding a lock, that way you know which thread is actually doing the signal or broadcast. I'm fairly certain this is what Mohammad and Hays expect from you, so you might as well do it!

### On Using `pthread_cond_wait()`
To make a long paragraph short: use while loops whenever you anticipate having a wait. It prevents any cases where the thread gets woken up and suddenly goes rogue, ruining every other part of your code. 

The more elaborate reason why we use while loops - whenever that thread gets woken up from that wait statement, it'll have to re-evaluate the condition of the while loop. So if the world still isn't a better place, the thread goes back to waiting in the queue, no harm done. It provides us with a lot more flexibility when it comes to signal and broadcast, since any threads that we still don't want to run still won't run. If you don't use while loops, we will find you. 

### One more quick thing...
If you are attempting to **immediately** go for the solution with as few conditional variables as possible, remember that it is often harder to get that correct than it is to just use a lot of conditional variables. Premature optimization is the root of all evil - remember that we grade on correctness first, so if your solution isn't correct, you can't get a lot of points!

[Back to table of contents](#table-of-contents)

<a id="problem_one"></a>

## Problem One: [Prime Sieve](sieve.c)
Deja vu, anyone? <br><br>
Length: Medium <br>
Objective: Familiarity with `fork()` chains, programmatically creating new processes. <br>
Files to modify: `sieve.c` <br>

### Description:
This is actually an *optional* problem that was on the Process Lab assignment from all that time ago. 

The objective is to set up a pipeline of processes that "sieves" out the first couple prime numbers that we know (the website suggests you limit it to be less than 35). The first process will generate all the numbers 2 through `n` (n can be whatever you want your input to be, I suggest 35), feeding them into the pipeline so long as it's **NOT** a multiple of 2. The second process is to eliminate multiples of 3 (second prime number), the third process is to eliminate multiples of 5, and so on. 

In order to have each process on the pipeline do its job, you'll need to set up a series of pipes going from one to another. The first number each new process receives should be printed; then any subsequent numbers they receive should be checked to see if the prime they are assigned divides that new number. If it doesn't, then pass it to your right neighbor. This should continue until you reach whatever your specified input is, then you should **safely** get rid of all of the processes - no zombies or orphans. Here's the more formal requirements that were given on the course website:
- Once the main process reaches `n`, it should wait until the entire pipeline terminates, include all children, grandchildren, etc.
- Processes should only be created on a **per-need** basis, i.e., you should not preallocate all processes and then run the sieve. 
- The program should terminate when all the pipeline has been destroyed, i.e., there shouldn't be any zombie or orphan processes.
- Each prime number must be printed by a separate process.

There is also psuedocode on the course website for what you should look to do if you're really stuck, as well as a figure that gives more detail on what your processes should look like.

To run this part, you can type `make` to generate the appropriate executable, namely, `./sieve.bin`. You can either run this with 1 argument, the number you want to run the prime sieve up to, or you can run it with no arguments, which will use 35 by default. So, either of these are acceptable:
```
./sieve.bin <number>
./sieve.bin
```
Here is some sample output:
```
./sieve.bin
prime 2
prime 3
prime 5
prime 7
prime 11
prime 13
prime 17
prime 19
prime 23
prime 29
prime 31
```

<a id="problem_two"></a>

## Problem Two: [Boat](boat.c)
Length: Medium <br>
Objective: Thread practice <br>
Files to modify: `boat.c` <br>

### Description: 
Suppose that there are two docks located at specific points on a very busy river - Northbound and Southbound docks. Each one of the docks can have an infinite amount of people waiting at them, but they will all wait for a boat which has a max capacity of `BOAT_CAPACITY = 3` passengers to take them from one dock to another. The company that runs this boat from dock to dock is very stingy and will NOT let the boat leave unless there are `BOAT_CAPACITY` people on board, even if there are people waiting on it at the other dock.

Let's model this problem in C, using threads. Here are the rules & assumptions that go with this problem:
- The boat starts in the Northbound direction (meaning it starts at the Southbound dock)
- If there is room on the boat, passengers should get onto the boat, until it is at `BOAT_CAPACITY` passengers. If the boat is not full, this means passengers may have to wait for a bit.
- When the boat arrives at the other dock, any passengers waiting there must continue to wait UNTIL every passenger who is currently **on board** leaves. So you cannot mix and match Northbound and Southbound passengers.
- If a passenger arrives at a dock while the boat is either at `BOAT_CAPACITY` or is not going in their direction must wait. 

Assumptions:
- There will always be some multiple of `BOAT_CAPACITY` people looking to travel across the river - so your code shouldn't deadlock as a result of there simply not being enough people to travel in one of the directions.
- All passengers will always be picked up and get to travel. There shouldn't be a case where there are passengers waiting for the boat while it is stuck at the other dock. 

These assumptions are given to you because if your code deadlocks, it means there's a bug in your solution to the problem, not the test cases. I have also provided you with 4 different scenario's worth of test cases, so you can debug individual situations. 

You should also keep track of the number of passengers who take a trip via the `total_passengers` global variable and the number of trips via the `total_trips` global variable.

Also, a quick note - do not be alarmed by things like `pr_red(...)` or `pr_magenta(...)` they are meant to function just like regular `printf(...)`, just with the console output as a different color. You may find these handy for debugging.

You can run your code by first typing `make`, and then by running `./boat.bin <test case>`. If you choose not to enter a test case, it will pick 1 by default. Valid options are `1-4` for the test case number.

Here is some sample output:
### Test Case 1
```
$ ./boat.bin 1
Running scenario   1: southbound passengers arrive first, wait for northbound passengers to take the trip.

Southbound passenger 1 arrived.
Southbound passenger 2 arrived.
Southbound passenger 3 arrived.
Northbound passenger 1 arrived.
Northbound passenger 1 on  board.
Northbound passenger 2 arrived.
Northbound passenger 2 on  board.
Northbound passenger 3 arrived.
Northbound passenger 3 on  board.
Boat is full, leaving dock in the northbound direction
Northbound passenger 1 getting off the boat.
Northbound passenger 2 getting off the boat.
Northbound passenger 3 getting off the boat.
Boat is back at dock and switching directions to southbound.
Southbound passenger 2 on board.
Southbound passenger 1 on board.
Southbound passenger 3 on board.
Boat is full, leaving dock in the southbound direction
Boat is back at dock and switching directions to northbound.

[Scenario 1]: Everything finished. The boat made   2 trips transporting   6 passengers.
```
### Test Case 2
```
$ ./boat.bin 2
Running scenario   2: test the boat going back to forth to get two sets of passengers across.

Southbound passenger 1 arrived.
Southbound passenger 2 arrived.
Southbound passenger 3 arrived.
Southbound passenger 4 arrived.
Southbound passenger 5 arrived.
Southbound passenger 6 arrived.
Northbound passenger 1 arrived.
Northbound passenger 1 on  board.
Northbound passenger 2 arrived.
Northbound passenger 2 on  board.
Northbound passenger 3 arrived.
Northbound passenger 3 on  board.
Boat is full, leaving dock in the northbound direction
Northbound passenger 4 arrived.
Northbound passenger 5 arrived.
Northbound passenger 6 arrived.
Northbound passenger 3 getting off the boat.
Northbound passenger 1 getting off the boat.
Northbound passenger 2 getting off the boat.
Boat is back at dock and switching directions to southbound.
Southbound passenger 6 on board.
Southbound passenger 1 on board.
Southbound passenger 3 on board.
Boat is full, leaving dock in the southbound direction
Boat is back at dock and switching directions to northbound.
Northbound passenger 4 on  board.
Northbound passenger 6 on  board.
Northbound passenger 5 on  board.
Boat is full, leaving dock in the northbound direction
Northbound passenger 5 getting off the boat.
Northbound passenger 6 getting off the boat.
Northbound passenger 4 getting off the boat.
Boat is back at dock and switching directions to southbound.
Southbound passenger 4 on board.
Southbound passenger 5 on board.
Southbound passenger 2 on board.
Boat is full, leaving dock in the southbound direction
Boat is back at dock and switching directions to northbound.

[Scenario 2]: Everything finished. The boat made   4 trips transporting  12 passengers.
```
### Test Case 3
```
$ ./boat.bin 3
Running scenario   3: test that the boat waits until it is full before heading out in any direction.

Northbound passenger 1 arrived.
Northbound passenger 1 on  board.
Northbound passenger 2 arrived.
Northbound passenger 2 on  board.
Southbound passenger 4 arrived.
Northbound passenger 3 arrived.
Northbound passenger 3 on  board.
Northbound passenger 5 arrived.
Boat is full, leaving dock in the northbound direction
Northbound passenger 3 getting off the boat.
Northbound passenger 2 getting off the boat.
Northbound passenger 1 getting off the boat.
Boat is back at dock and switching directions to southbound.
Southbound passenger 4 on board.
Southbound passenger 6 arrived.
Southbound passenger 6 on board.
Southbound passenger 7 arrived.
Southbound passenger 7 on board.
Boat is full, leaving dock in the southbound direction
Northbound passenger 8 arrived.
Boat is back at dock and switching directions to northbound.
Northbound passenger 5 on  board.
Northbound passenger 8 on  board.
Northbound passenger 9 arrived.
Northbound passenger 9 on  board.
Boat is full, leaving dock in the northbound direction
Northbound passenger 8 getting off the boat.
Northbound passenger 5 getting off the boat.
Northbound passenger 9 getting off the boat.
Boat is back at dock and switching directions to southbound.

[Scenario 3]: Everything finished. The boat made   3 trips transporting   9 passengers.
```
### Test Case 4
```
$ ./boat.bin 4
Running scenario   4: test that some passengers might have to wait two trips before they get a chance to go.

Southbound passenger 1 arrived.
Southbound passenger 2 arrived.
Southbound passenger 3 arrived.
Southbound passenger 4 arrived.
Southbound passenger 5 arrived.
Southbound passenger 6 arrived.
Southbound passenger 7 arrived.
Southbound passenger 8 arrived.
Southbound passenger 9 arrived.
Northbound passenger 10 arrived.
Northbound passenger 10 on  board.
Northbound passenger 11 arrived.
Northbound passenger 11 on  board.
Northbound passenger 12 arrived.
Northbound passenger 12 on  board.
Northbound passenger 13 arrived.
Boat is full, leaving dock in the northbound direction
Northbound passenger 12 getting off the boat.
Northbound passenger 10 getting off the boat.
Northbound passenger 11 getting off the boat.
Boat is back at dock and switching directions to southbound.
Southbound passenger 1 on board.
Southbound passenger 2 on board.
Southbound passenger 5 on board.
Boat is full, leaving dock in the southbound direction
Boat is back at dock and switching directions to northbound.
Northbound passenger 13 on  board.
Northbound passenger 14 arrived.
Northbound passenger 14 on  board.
Northbound passenger 15 arrived.
Northbound passenger 15 on  board.
Boat is full, leaving dock in the northbound direction
Northbound passenger 16 arrived.
Northbound passenger 17 arrived.
Northbound passenger 18 arrived.
Northbound passenger 19 arrived.
Northbound passenger 20 arrived.
Northbound passenger 21 arrived.
Northbound passenger 15 getting off the boat.
Northbound passenger 13 getting off the boat.
Northbound passenger 14 getting off the boat.
Boat is back at dock and switching directions to southbound.
Southbound passenger 4 on board.
Southbound passenger 3 on board.
Southbound passenger 6 on board.
Boat is full, leaving dock in the southbound direction
Boat is back at dock and switching directions to northbound.
Northbound passenger 16 on  board.
Northbound passenger 17 on  board.
Northbound passenger 18 on  board.
Boat is full, leaving dock in the northbound direction
Northbound passenger 18 getting off the boat.
Northbound passenger 16 getting off the boat.
Northbound passenger 17 getting off the boat.
Boat is back at dock and switching directions to southbound.
Southbound passenger 8 on board.
Southbound passenger 9 on board.
Southbound passenger 7 on board.
Boat is full, leaving dock in the southbound direction
Boat is back at dock and switching directions to northbound.
Northbound passenger 20 on  board.
Northbound passenger 19 on  board.
Northbound passenger 21 on  board.
Boat is full, leaving dock in the northbound direction
Northbound passenger 21 getting off the boat.
Northbound passenger 20 getting off the boat.
Northbound passenger 19 getting off the boat.
Boat is back at dock and switching directions to southbound.

[Scenario 4]: Everything finished. The boat made   7 trips transporting  21 passengers.
```

[Back to table of contents](#table-of-contents)








