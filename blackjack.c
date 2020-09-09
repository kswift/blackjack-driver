/*
* Kevin Swift
* Csc352, Spring20  
* blakcjack.c
*
* This program represents a single blackjack game with 5 piles at a time. The goal is to earn points
* by:
* 
* Blackjack (pile total is 21) +21 points
* Natural blackjack (pile total is 21 with 2 cards) +35 points
* Clearing the board (all piles are empty after the game has begun) +45 points
* Cleared the deck (Dealth all 52 cards, repeatable) +60 points
* "Five Card Charlie" (a pile contains 5 cards that are less than 21) +total points in pile
*
* The game ends when all piles would exceed 21 points. If the current high score is higher than the
* previous then it gets written to a file named "highscore.dat" and will indicate in the console if it is.
* If there is no previous file set the high score equal to 0.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
struct Pile;
void debug(struct Pile *, struct Pile *, struct Pile *,
           struct Pile *, struct Pile *, int);

void pile_cards(struct Pile *, int, char);

int events(struct Pile *, int , int *, struct Pile *, struct Pile *, struct Pile *, struct Pile *, struct Pile *);

char drawn_card(int);
int card_value(char);
void display_card(char);
void initialize(int[]);
void shuffle_cards(int[]);

// This represents a single pile in our blackjack game
struct Pile
{
    int sum;
    int max;
    char message[999]; //concatinate at compile time
    int curr;           //number of cards in pile
};

int main(void)
{

    // initializes struct pointers for all piles
    struct Pile pile1;
    struct Pile pile2;
    struct Pile pile3;
    struct Pile pile4;
    struct Pile pile5;

    // sets the sum field of each Pile equal to 0
    pile1.sum = 0;
    pile2.sum = 0;
    pile3.sum = 0;
    pile4.sum = 0;
    pile5.sum = 0;

    // maximum of each pile has not been reached yet (boolean)
    pile1.max = 0;
    pile2.max = 0;
    pile3.max = 0;
    pile4.max = 0;
    pile5.max = 0;

    // curr number of cards in each pile
    pile1.curr = 0;
    pile2.curr = 0;
    pile3.curr = 0;
    pile4.curr = 0;
    pile5.curr = 0;

    // initialize pile strings
    strcpy(pile1.message, "");
    strcpy(pile2.message, "");
    strcpy(pile3.message, "");
    strcpy(pile4.message, "");
    strcpy(pile5.message, "");

    int pile_selection;

    // randomly drawn card
    int drawn_card_int;

    // card
    char card[2];

    // copy
    char copy[2];

    // total score for game
    int score = 0;
    int score_increase;
    int deck[52];

    initialize(deck);
    shuffle_cards(deck);

    int counter = 0;
    int blackjack = 0;

    while (blackjack == 0 && counter < 53)
    {
        score_increase = 0;

        // pass in struct Pile as address
        debug(&pile1, &pile2, &pile3, &pile4, &pile5, score);

        // randomly selected card as an integer (0 - 13)
        drawn_card_int = deck[counter] % 13;

        // char of card
        card[0] = drawn_card(drawn_card_int);

        // display the drawn card to console
        display_card(card[0]);

        // determines the vlaue of the card based on rules
        score_increase = card_value(card[0]);

        printf("Which pile to place card in? ");
        scanf(" %d", &pile_selection);

        // If an A with a value 11 makes pile > 21 then set A to have a value of 1
        if ((pile_selection == 1) && (score_increase == 11) && (score_increase + pile1.sum > 21) ||
            (pile_selection == 2) && (score_increase == 11) && (score_increase + pile2.sum > 21) ||
            (pile_selection == 3) && (score_increase == 11) && (score_increase + pile3.sum > 21) ||
            (pile_selection == 4) && (score_increase == 11) && (score_increase + pile4.sum > 21) ||
            (pile_selection == 5) && (score_increase == 11) && (score_increase + pile5.sum > 21)){

            score_increase = 1;
        }

        if ((pile_selection == 1) && (score_increase + pile1.sum <= 21))
        {
            pile_cards(&pile1, score_increase, card[0]);
            score = events(&pile1, score, &counter, &pile1, &pile2, &pile3, &pile4, &pile5);
        }

        else if ((pile_selection == 2) && (score_increase + pile2.sum <= 21))
        {
            pile_cards(&pile2, score_increase, card[0]);
            score = events(&pile2, score, &counter, &pile1, &pile2, &pile3, &pile4, &pile5);
        }

        else if ((pile_selection == 3) && (score_increase + pile3.sum <= 21))
        {
            pile_cards(&pile3, score_increase, card[0]);
            score = events(&pile3, score, &counter, &pile1, &pile2, &pile3, &pile4, &pile5);
        }

        else if ((pile_selection == 4) && (score_increase + pile4.sum <= 21))
        {
            pile_cards(&pile4, score_increase, card[0]);
            score = events(&pile4, score, &counter, &pile1, &pile2, &pile3, &pile4, &pile5);
        }

        else if ((pile_selection == 5) && (score_increase + pile5.sum <= 21))
        {
            pile_cards(&pile5, score_increase, card[0]);
            score = events(&pile5, score, &counter, &pile1, &pile2, &pile3, &pile4, &pile5);
        }
        
        // verify the pulled card can be placed in a file. Break out if no valid move.
        else if ((score_increase + pile1.sum > 21) && (score_increase + pile2.sum > 21) &&
            (score_increase + pile3.sum > 21) && (score_increase + pile4.sum > 21) &&
            (score_increase + pile5.sum > 21)){
            break;
        }

        else{
            continue;
        }

        // update score here
        counter++;
    }

    // game ending; display final score and indicate if its
    // greater than value in file
    printf("\n\nNo More moves, all piles exceed 21!\n");
    printf("Final Score: %d\n\n", score);

    // update the output file if the current run is a high score
    int all_time_high;
    FILE *fptr = NULL;
    fptr = fopen("highscore.dat", "r");

    if (fptr == NULL){
        all_time_high = 0;
    }

    else{
        fscanf(fptr, "%d", &all_time_high);
        fclose(fptr);
    }
    
    if (score > all_time_high) {
        printf("New High Score!\n");
        FILE *out = fopen("highscore.dat", "w");
        fprintf(out, "%d", score);
    }

    return 0;
}

/*
* This function updates the current score by checking all of the current
* piles based on the win conmditions decribed in the filed header.
*
* Returns the score so that a helper function cann display the (updated) valued
*/
int events(struct Pile *p, int score, int *counter, struct Pile *pile1, struct Pile *pile2, struct Pile *pile3,
    struct Pile *pile4, struct Pile *pile5){

    if (*counter == 52){
        printf("\nCleared the deck! +60 points\n\n");
        score += 60;

        strcpy(p -> message, "");
        p -> curr = 0;
        p -> max = 0;
        p -> sum = 0;
    }
        
    if ((p -> curr == 2) && (p -> sum == 21)){
        printf("\nNatural Blackjack! +35 points\n");
        score += 35;

        strcpy(p -> message, "");
        p -> curr = 0;
        p -> max = 0;
        p -> sum = 0;
    }

    else if (p -> sum == 21){
        printf("\nBlackjack! +21 points\n");
        score += 21;

        strcpy(p -> message, "");
        p -> curr = 0;
        p -> max = 0;
        p -> sum = 0;
    }
            
    else if ((p -> curr == 5) && (p -> sum < 21)){
        printf("\n\"Five Card Charlie\" +total points in pile\n");
        score += p -> sum;

        strcpy(p -> message, "");
        p -> curr = 0;
        p -> max = 0;
        p -> sum = 0;
    }

    else if ((*counter != 0) && (pile1 -> curr == 0) && (pile2 -> curr == 0) 
        && (pile3 -> curr == 0) && (pile4 -> curr == 0) && (pile5 -> curr == 0)){
        printf("\nClearing the board! + 45 points\n");
        score += 45;

        *counter = 0;

        strcpy(pile1 -> message, "");
        strcpy(pile2 -> message, "");
        strcpy(pile3 -> message, "");
        strcpy(pile4 -> message, "");
        strcpy(pile5 -> message, "");

        pile1 -> sum = 0;
        pile2 -> sum = 0;
        pile3 -> sum = 0;
        pile4 -> sum = 0;        
        pile5 -> sum = 0;
    
        pile1 -> curr = 0;
        pile2 -> curr = 0;
        pile3 -> curr = 0;
        pile4 -> curr = 0;        
        pile5 -> curr = 0;

        pile1 -> max = 0;
        pile2 -> max = 0;
        pile3 -> max = 0;
        pile4 -> max = 0;        
        pile5 -> max = 0;
    }
    return score;

}

/*
* This function displays the current condition of each pile and the total score
* for the current game. We pass 5 struct pointers each representing information about
* each pile. 
*/
void debug(struct Pile *pile1, struct Pile *pile2, struct Pile *pile3,
           struct Pile *pile4, struct Pile *pile5, int score)
{
    printf("\nScore: %d\n\n", score);

    printf("Pile (1): %s           = %d\n", pile1->message, pile1->sum);
    printf("Pile (2): %s           = %d\n", pile2->message, pile2->sum);
    printf("Pile (3): %s           = %d\n", pile3->message, pile3->sum);
    printf("Pile (4): %s           = %d\n", pile4->message, pile4->sum);
    printf("Pile (5): %s           = %d\n\n", pile5->message, pile5->sum);
}

/*
* Assigns a card value to each random intger inside of our deck array of ints.
* returns -1 if an unknown value is pulled (shouldn't happen)
*/  
char drawn_card(int num)
{
    if (num == 0)
    {
        return 'A';
    }

    else if (num == 1)
    {
        return 'J';
    }

    else if (num == 11)
    {
        return 'Q';
    }

    else if (num == 12)
    {
        return 'K';
    }
    else
        return num;

    return -1;
}

/*
* Returns the integer value of each char. Honestly, commenting I realized
* this is very redundant as in C chars are ints and assigned an ASCII value.
*/
int card_value(char x)
{
    if (x == 'A')
    {
        return 11;
    }
    else if (x == 'J' || x == 'Q' || x == 'K')
    {
        return 10;
    }
    else
        return x;
    return -1;
}

/*
* Generates and fills an array of ints in consecutive order for each element (0 - 51)
*/
void initialize(int deck[])
{
    int i;
    for (i = 0; i < 52; i++)
    {
        deck[i] = i;
    }
}

/*
* Shuffles the deck in a random ordering
*/
void shuffle_cards(int deck[])
{
    int i;
    for (i = 0; i < 9999; i++)
    {
        int old = rand() % 52;
        int new = rand() % 52;
        int card = deck[old];
        deck[old] = deck[new];
        deck[new] = card;
    }
}

/*
* This function prints the random card pulled onto the console.
*/
void display_card(char x)
{
    if (x == 81)
    {
        printf("Drawn Card: Q\n\n");
    }

    else if (x == 75)
    {
        printf("Drawn Card: K\n\n");
    }

    else if (x == 74)
    {
        printf("Drawn Card: J\n\n");
    }

    else if (x == 65)
    {
        printf("Drawn Card: A\n\n");
    }

    else
    {
        printf("Drawn Card: %d\n\n", x);
    }
}

/*
* This function adds the pulled card to the designated pile. This also updates the
* struct field sum which is the total score for the current pile and number of cards it contains. 
*/
void pile_cards(struct Pile *p, int score_increase, char x)
{
    if (x == 81)
    {
        strcat(p->message, "Q ");
        p->sum += score_increase;
    }

    else if (x == 75)
    {
        strcat(p->message, "K ");
        p->sum += score_increase;
    }

    else if (x == 74)
    {
        strcat(p->message, "J ");
        p->sum += score_increase;
    }

    else if (x == 65)
    {
        strcat(p->message, "A ");
        p->sum += score_increase;
    }

    else if (x == 2)
    {
        strcat(p->message, "2 ");
        p->sum += score_increase;
    }

    else if (x == 3)
    {
        strcat(p->message, "3 ");
        p->sum += score_increase;
    }

    else if (x == 4)
    {
        strcat(p->message, "4 ");
        p->sum += score_increase;
    }

    else if (x == 5)
    {
        strcat(p->message, "5 ");
        p->sum += score_increase;
    }

    else if (x == 6)
    {
        strcat(p->message, "6 ");
        p->sum += score_increase;
    }

    else if (x == 7)
    {
        strcat(p->message, "7 ");
        p->sum += score_increase;
    }

    else if (x == 8)
    {
        strcat(p->message, "8 ");
        p->sum += score_increase;
    }

    else if (x == 9)
    {
        strcat(p->message, "9 ");
        p->sum += score_increase;
    }

    else if (x == 10)
    {
        strcat(p->message, "10 ");
        p->sum += score_increase;
    }

    // update number of cards in current struct
    p -> curr++;
}
