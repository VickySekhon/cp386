# q1 : collatz_sequence

1) Parent reads numbers from start_numbers.txt and stores into a global array 

2) Parent loops through global array and calls collatz_seq() on each, thereby creating a sequence

3) Parent creates a shared memory object between itself and child process and passes sequence to child through that shared memory object

4) Child opens shared memory object (accesses the sequence) and prints the sequence 

# q2 : progress_management

- parent uses command-line arguments to read file name

- parent creates a child process which will read the contents of the file name
(sample_in_process.txt)

- file's contents are written to shared memory by child process, allowing the parent process to read it from there

- copy contents from share memory to array 

- child runs commands and writes them to pipe

- parent reads from pipe and writes to screen using writeOutput()


## logic put simply

- 