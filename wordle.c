#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define WORD_SIZE 5
#define MAX_ATTEMPTS 6

const char *words[] = {"cable", "flame", "brave", "shiny", "globe", "dream", "crisp", "plush"};

void get_feedback(const char *secret, const char *guess, char *feedback) {
    int secret_letter_count[26] = {0};
    int guess_letter_count[26] = {0};

    for (int i = 0; i < WORD_SIZE; i++) {
        if (guess[i] == secret[i]) {
            feedback[i] = 'G';  // Green: correct letter in the correct place
        } else {
            feedback[i] = 'B';  // Black: incorrect letter
            secret_letter_count[secret[i] - 'a']++;
            guess_letter_count[guess[i] - 'a']++;
        }
    }

    for (int i = 0; i < WORD_SIZE; i++) {
        if (feedback[i] == 'B' && guess_letter_count[guess[i] - 'a'] > 0 && secret_letter_count[guess[i] - 'a'] > 0) {
            feedback[i] = 'Y';  // Yellow: correct letter in the wrong place
            secret_letter_count[guess[i] - 'a']--;
            guess_letter_count[guess[i] - 'a']--;
        }
    }

    feedback[WORD_SIZE] = '\0';
}

void print_feedback(const char *feedback, const char *guess) {
    for (int i = 0; i < WORD_SIZE; i++) {
        if (feedback[i] == 'G') {
            printf("\033[1;32m%c\033[0m", guess[i]);  // Green for correct letter in correct place
        } else if (feedback[i] == 'Y') {
            printf("\033[1;33m%c\033[0m", guess[i]);  // Yellow for correct letter in wrong place
        } else {
            printf("\033[1;30m%c\033[0m", guess[i]);  // Black for incorrect letter
        }
    }
    printf("\n");
}

int is_valid_guess(const char *guess) {
    if (strlen(guess) != WORD_SIZE) {
        return 0;
    }
    for (int i = 0; i < WORD_SIZE; i++) {
        if (!isalpha(guess[i])) {
            return 0;
        }
    }
    return 1;
}

int main() {
    srand(time(0));
    const char *secret_word = words[rand() % (sizeof(words) / sizeof(words[0]))];
    char guess[WORD_SIZE + 1];
    char feedback[WORD_SIZE + 1];
    int attempts = 0;

    printf("Welcome to Wordle!\n");
    printf("You have %d attempts to guess the %d-letter word.\n", MAX_ATTEMPTS, WORD_SIZE);

    while (attempts < MAX_ATTEMPTS) {
        printf("Attempt %d: Enter your guess: ", attempts + 1);
        scanf("%s", guess);

        // Convert guess to lowercase
        for (int i = 0; guess[i]; i++) {
            guess[i] = tolower(guess[i]);
        }

        if (!is_valid_guess(guess)) {
            printf("Invalid guess. The guess must be %d letters and only contain alphabetic characters. Try again.\n", WORD_SIZE);
            continue;
        }

        get_feedback(secret_word, guess, feedback);
        print_feedback(feedback, guess);

        if (strcmp(secret_word, guess) == 0) {
            printf("Congratulations! You've guessed the word correctly.\n");
            break;
        }

        attempts++;
    }

    if (attempts == MAX_ATTEMPTS) {
        printf("Sorry, you've used all your attempts. The secret word was: %s\n", secret_word);
    }

    return 0;
}
