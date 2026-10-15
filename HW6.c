#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main(void)
{
    int voters;
    int maxNumberOfRounds;
    pid_t pid;
    int voter_id = -1;

    // Get the number of voters from the user
    printf("Enter the number of voters: ");
    fflush(stdout);

    // Check if the input is a valid integer
    if (scanf("%d", &voters) != 1) {
        fprintf(stderr, "Error: expected a int for the first input.\n");
        return EXIT_FAILURE;
    }

    // Get the number of rounds from the user
    printf("Enter the number of rounds: ");

    fflush(stdout);

    // Check if the input is a valid integer
    if (scanf("%d", &maxNumberOfRounds) != 1) {
        fprintf(stderr, "Error: expected a int for the second input.\n");
        return EXIT_FAILURE;
    }

    // Judge process creates a pipe for each voter
    int pipes[voters][2];

    // Check if the pipe was created successfully
    for (int i = 0; i < voters; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("Pipe creation failed");
            return EXIT_FAILURE;
        }
    }

    // Judge process creates a child process for each voter
    for (int i = 0; i < voters; i++) {
        pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            return EXIT_FAILURE;
        } else if (pid == 0) {
            // Child process (voter)
            voter_id = i;

            // Give each voter a different random seed
            srand((unsigned int)(time(NULL) ^ getpid()));
            
            // Close the read end of the pipe in the child process so that it can only write to the pipe
            close(pipes[i][0]);

            // exit the loop so that the child process does not create more child processes
            break;
        } else {
            // Parent process (judge)
            // Close the write end of the pipe in the parent process so that it can only read from the pipe
            close(pipes[i][1]);
        }
    }

    // Now we go into the voting rounds. The judge will read from the pipes and declar the majority, and the voters will write to the pipes with their votes.
    for (int i = 0; i < maxNumberOfRounds; i++) {
        // Judge process reads from the pipes and declares the majority
        if (voter_id == -1) {
            printf("------ Round %d ------\n", i + 1);

            int votes[voters];
            int count = 0.0;
            for (int j = 0; j < voters; j++) {
                read(pipes[j][0], &votes[j], sizeof(int));

                // Print the vote of each voter
                printf("Round %d: Voter %d voted: %s\n", i + 1, j + 1, (votes[j] == 1) ? "YES" : "NO");
            }

            // Count the votes and declare the majority
            for (int j = 0; j < voters; j++) {
                if (votes[j] == 1) {
                    count++;
                }
            }
            printf("--- Round %d: The voting result is a ", i + 1);

            // print results of the majority or if it is a tie
            if (count * 2 == voters) {
                printf("TIE.\n");
            } else if (count * 2 < voters) {
                printf("NO.\n");
            } else {
                printf("YES.\n");
            }

            printf("\n\n");
        } else {
            // Voter process writes to the pipe with their vote
            int vote = rand() % 2; // Randomly vote YES (1) or NO (0)
            write(pipes[voter_id][1], &vote, sizeof(int));
        }
    }

    return EXIT_SUCCESS;
}
