# q1 : collatz_sequence

1) Parent reads numbers from start_numbers.txt and stores into a global array 

2) Parent loops through global array and calls collatz_seq() on each, thereby creating a sequence

3) Parent creates a shared memory object between itself and child process and passes sequence to child through that shared memory object

4) Child opens shared memory object (accesses the sequence) and prints the sequence 
