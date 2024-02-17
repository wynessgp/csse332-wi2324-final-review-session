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

I'd recommend leaving this README page up on GitHub and tabbing back and forth for instructions. Like before, I have provided you with some nice navigability links to jump around.

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
Reminder that these links will only work if you're viewing this in a markdown renderer, or you're on GitHub. I'd highly suggest using one of those. <br>

[Exam 1 Content Notes](#exam_one_notes) <br>
[Exam 2 Content Notes](#exam_two_notes) <br>

Note that the content note sections are copied directly from my previous review session README files - they should be used in tandem with your own notes if you cannot remember something.

<a id="admin_notes"></a>

## *Administrative* Notes
This GitHub repository, once again, will only contain the questions for a practice exam - the solutions will NOT be posted here. I've been explicitly asked not to post them here, as it will make the sample solutions public. You are more than welcome to ask me for hints, but please keep in mind I am also a student, and have finals. The best time to ask me questions is DURING the review session - that's time I've set aside for you to explicitly practice and ask me questions!<br><br>
***Please respect my time off, especially as we go into finals week. If I do not respond to your first teams message, I will not respond if you send a billion more.***<br><br>
That being said, let's talk about your final exam. <br><br>
For this exam, there is a **2** hour time limit, longer if you have accomodations. It will have **two** questions, with **two** parts for each. <br><br>
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
- Remember that calling fork() splits the current process into two processes like so:
    - If pid < 0, it's an error.
    - The child process has pid = 0
    - The original parent process has pid > 0.
- Once you get into the parent's code, the pid of the CHILD process will be stored into your pid variable.
- Remember to do error handling on fork & execlp calls! (see example below)
```
int pid = fork();
if (pid < 0) {
    perror("fork error");
    exit(1); // make sure you exit!
} else if (pid == 0) {
    execlp(...);
    // you will ONLY reach this line IF execlp fails!
    perror("execlp error"); 
    exit(1);
}
```
- execlp should automatically throw the process away once it's done, but we error handle just incase it doesn't throw away the process.
- A good parent process should always wait for its child processes, whether it happens in an else block on PID, or you use waitpid at some point later on. Either way, just prevent zombies (unless if you are told it is OK for a problem)
- Remember to close pipes! Mohammad says you're a bad person if you don't close your pipes. I don't think you want to be remembered for forgetting to close a pipe end while programming, so make sure you don't forget!

[Back to table of contents](#table-of-contents)

<a id="exam_two_notes"></a>

## Exam Two *Content* Notes
Threads, unlike processes, have **SHARED** global memory... more specifically, they share a code section, data section, and some OS resources (opened files, signals, etc). However, threads do happen to have unique stacks, so local variables are largely unaffected.

### On Global Variables
This is why we have to be so cautious about how we utilize global variables. If there's a chance that another thread may need to access what your current thread is looking at, you'll need to use a lock to prevent an inconsistent state between the two separate threads. You don't want to have a thread actively updating a variable's value while another is doing a comparison with it - one should be resolved before another, otherwise you'll get incorrect behavior. So unless if you can guarantee that your threads will not overlap in **ANY** way, make sure you **LOCK!** <br><br>
Most of the time, you will need locks. The only situation I can possibly think of where you won't need one is if you can guarantee that threads access only their own specified index within an array. The original thread can then come back and clean up the data after the child threads are done.

[Back to table of contents](#table-of-contents)

### On Critical Sections...
In order to have threads really run anything in parallel, it is imperative that you **UNLOCK** as soon as you are done with any thread overhead code - that way it gives other threads an opportunity to actually run the relevant code in the **CRITICAL SECTION**. If you do not unlock, your threads will run in a serial manner as opposed to a parallel one, which is WAY slower. I have written a small example to show this off, not unlike what you guys did for max in conditional variable basics: <br><br>
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

### On Signal vs. Broadcast
Sometimes we will ask you to implement things that expect multiple threads to be running a specific way all at once. Think tunnel, three jobs, and the like. If you cannot guarantee exactly how many threads will be waiting in a queue, and you'd like for **MULTIPLE** of the waiting ones to start running at the same time, it is in your best interest to use *broadcast*. Even if you have multiple threads currently running, if there's any chance you might have *MORE* after this one, you should *broadcast*.<br> <br>
In the case of ```tunnel```, think about an ambulance interrupting normal traffic flow - cars might start piling up at the EW entrance, so when that ambulance leaves, it should *broadcast* to those EW cars. (This is a bit of an oversimplifiction, but you get the point) If it alerts only one car with signal, then suddenly the EW side of the tunnel shrinks to one lane! <br><br>
As far as signal goes, this is for a *targeted* number of threads. *Signal* itself only wakes up one thread in a queue - so if you have multiple threads running a *signal* call, you'll end up waking up however many corresponding threads. You can also have just one thread call it multiple times. <br><br>
It is important to note that these can both generate viable solutions to a problem depending on how you make threads wait, but switching between the two can sometimes vastly reduce the amount of code you have for a problem. <br><br>
An important thing to note is that these will **NOT** error if there are no threads waiting in a queue, it just simply won't wake anyone up. <br><br> 
Additionally, you should aim to use both of these calls while you still have a lock. Why? Most of the time a program will still have correct behavior regardless if you signal while locked or unlocked, but if you want to 100% guarantee correctness, it is **safer** to use broadcast and signal while you are holding a lock, that way you know which thread is actually doing the signal or broadcast. I'm fairly certain this is what Mohammad and Hays expect from you, so you might as well do it!

### On Waiting
To make a long paragraph short: use while loops whenever you anticipate having a wait. It prevents any cases where the thread gets woken up and suddenly goes rogue, ruining every other part of your code. <br><br>
The more elaborate reason why we use while loops - whenever that thread gets woken up from that wait statement, it'll have to re-evaluate the condition of the while loop. So if the world still isn't a better place, the thread goes back to waiting in the queue, no harm done. It provides us with a lot more flexibility when it comes to signal and broadcast, since any threads that we still don't want to run still won't run. If you don't use while loops, we will find you. 

[Back to table of contents](#table-of-contents)





