/* 
 * Numerlogy calculator written in pure C by wbxpre15
 * LICENSE - whatever license 
 * do what you want with the examples shown. 
 * make what you like with it
 * at some point I will stop posting updates to the code, to make an actual product.
 * please dont beat me too it ;) 
 * Be careful, this predicts your future, use it with caution. 
*/

/*
 * TODO LIST
 * add matrix of compatibility charts for iterate_months
 * add pythagoras matrix, to add name and birth numbers together to find missing numbers
 * add day and month calculators
 * add personal year calculator
 * add personal day calculator
 * 
*/

// include needed libs. 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 
 hard coded macro to define the alphabet system used. 
 0 is hebrew, does not use 9. 
 1 is normal english gematria.
 2 is the hebrew and english system reduced together, interestinly the diffrence is 9 ;) 
*/
#define system 1

// needed conversion tables. 
const char* Upper_Case[21]  = { "B", "C", "D", "F", "G", "H", "J", "K", "L", "M", "N", "P", "Q", "R", "S", "T", "V", "W", "X", "Y", "Z" };
const char* Lower_Case[21]  = { "b", "c", "d", "f", "g", "h", "j", "k", "l", "m", "n", "p", "q", "r", "s", "t", "v", "w", "x", "y", "z" };
const char* Upper_Case_Vowels[5]  = { "A", "E", "I", "O", "U" };
const char* Lower_Case_Vowels[5]  = { "a", "e", "i", "o", "u" };
const char* Numbers[10]     = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
const int   iNumbers[10]    = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
const int   Unit_System[21] = { 2, 3, 4, 8, 3, 5, 1, 2, 3, 4, 5, 8, 1, 2, 3, 4, 6, 6, 6, 1, 7 };
const int   Unit_System_Vowels[5] = { 1, 5, 1, 7, 6 };
const int   Alt_System[21]  = { 2, 3, 4, 6, 7, 8, 1, 2, 3, 4, 5, 7, 8, 9, 1, 2, 4, 5, 6, 7, 8 };
const int   Alt_System_Vowels[5]  = { 1, 5, 9, 6, 3 };
const int   blk_System[21]  = { 2, 1, 5, 1, 1, 2, 3, 4, 5, 7, 8, 1, 2, 3, 4, 6, 6, 6, 6, 1, 7 };
const int   blk_System_Vowels[5]  = { 1, 5, 1, 4, 9 };
const int   num_days_in_month[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

// place holder, because fuck using C++ when we dont need it.
int dummy_consonant, dummy_vowel;

int calc_letter(char* letter, int *len, int *vowel) {
    for (int i=0; i<21; i++ ) {
        // check numbers
        if ( i < 10 && strncmp(&letter[*len], Numbers[i], 1) == 0 ) {
            *vowel = -1;
            return(iNumbers[i]);
        }
        // check vowels
        if ( i < 5 && (strncmp(&letter[*len], Lower_Case_Vowels[i], 1) == 0 || strncmp(&letter[*len], Upper_Case_Vowels[i], 1) == 0) ) {
            *vowel = 1;
            switch (system) {
                case 0:
                    return(Unit_System_Vowels[i]);
                    break;
                case 1: 
                    return(Alt_System_Vowels[i]);
                    break;
                case 2: 
                    return(blk_System_Vowels[i]);
                    break;
            }
        }
        // check consants
        if ( strncmp(&letter[*len], Lower_Case[i], 1) == 0 || strncmp(&letter[*len], Upper_Case[i], 1) == 0 ) {
            *vowel = 0;
            switch (system) {
                case 0:
                    return(Unit_System[i]);
                    break;
                case 1: 
                    return(Alt_System[i]);
                    break;
                case 2: 
                    return(blk_System[i]);
                    break;
            }
        }
    }
    printf("ERROR: input character not found so return 0");
    return(0);
}

int calc_word(char* input_string, int *vowel_total, int *consonant_total) {
    int y = 0;
    int x = strlen(input_string);
    int len = 0;
    for ( int i=0; i<x; i++ ) {
        int bvowel = -1;
        int j = calc_letter(input_string, &len, &bvowel);
        y += j;
        if ( bvowel == 1 ) {
            *vowel_total += j;
        }
        else if ( bvowel == 0)  { 
            *consonant_total += j;
        }
        //printf("letter.%c = %d type.%d\n", input_string[len], j, bvowel);
        len++;
    }
    //printf("vowel_total.%d conesneant_total.%d, word_total.%d\n", *vowel_total, *consonant_total, y);
    return(y);
}

int reduce_number(int number) {
    // 11 and 22 should not be reduced.
    if ( number == 11 || number == 22 )
        return(number);
    // limit is a 32 digit length number
    char snumber[32] = "";
    sprintf(snumber, "%d", number);
    return(calc_word(snumber, &dummy_vowel, &dummy_consonant));
}

void iterate_months (int median_year, int range, int seeking_number) {
    // function to return birthdays reducing to an input number within a range of years. 
    // starting year is first number - second number. ending year is first number + second number. 
    // last number is the one being searched for. use the destiny number and the chart to find applicable numbers.
    // todo: add 3 dimentional matrix holding the business and personal compatibility numbers.
    for (int i=median_year-range; i<median_year+range; i++) {
        for (int y=0; y<12; y++) {
            for (int z=1; z<( i % 4 == 0 && y == 1 ? num_days_in_month[y]+1 : num_days_in_month[y]); z++) {
                char str[16];
                sprintf(str, "%d%d%d", z, y+1, i);
                int total_reduced_year = calc_word(str, &dummy_vowel, &dummy_consonant);
                int reduced_year = reduce_number(total_reduced_year);
                if ( reduced_year == seeking_number ) {
                    printf("%d/%d/%d = %d/%d\n", z, y+1, i, total_reduced_year, reduced_year);
                }
            }
        }
    }
}

int calc_sentance (char *sentance, int *total_vowel_number, int *total_consonant_number, int *reduced_total, int *reduced_vowel, int *reduced_consonant) {
    // inputs sentance string, splits into words
    // returns the total whole number of the sentance (unreduced), and everything else to the pointers in the input. 
    // assign the first word of sentance to the pointer
    const char delim[] = " ";
    int total_whole_number = 0;
    char *ptr = strtok(sentance, delim);
    do {
        char word[256]; // limits word to 256 char length. 128 may be enough?
        int whole_number = 0;
        int vowel_number = 0;
        int consonant_number = 0;
        
        // copy word into temp memory position, and calculate its totals. 
        strcpy(word, ptr);
        whole_number = calc_word(word, &vowel_number, &consonant_number);
        
        // add this word to the total of all words. 
        total_whole_number += whole_number;
        *total_vowel_number += vowel_number;
        *total_consonant_number += consonant_number;
        
        // reduce each word total and print the result for this word. 
        int ireduced_vowel = reduce_number(vowel_number);
        int ireduced_consonant = reduce_number(consonant_number);
        int ireduced_total = reduce_number(total_whole_number);
        printf("%s = vowel.%d/%d consonant.%d/%d total.%d/%d\n", word, vowel_number, ireduced_vowel, consonant_number, ireduced_consonant, whole_number, ireduced_total);
        
        // move onto the next word. 
        ptr = strtok(NULL, delim);
    } while ( ptr != NULL) ; // stop when there are no words left.
    
    // reduce the total numbers to single digits and return the results. 
    *reduced_total = reduce_number(total_whole_number);
    *reduced_vowel = reduce_number(*total_vowel_number);
    *reduced_consonant = reduce_number(*total_consonant_number);
    return(total_whole_number);
}

int calc_date_of_birth_number (char *dob, int *day_of_birth_number, int *month_number, int *year_number, int *unreduced_year_number) {
    // input string in format dd/mm/yyyy
    // returns date of birth number, and day of birth number along with month number and reduced year number
    const char delim[] = "/";
    char *ptr = NULL;
    if ( (ptr = strtok(dob, delim)) != NULL) {
        char input[4];
        strcpy(input, ptr);
        int iday_of_birth_number = calc_word(input, &dummy_vowel, &dummy_consonant);
        *day_of_birth_number = reduce_number(iday_of_birth_number);
        // calculate month number 
        if ( (ptr = strtok(NULL, delim)) != NULL) {
            strcpy(input, ptr);
            int imonth_number = calc_word(input, &dummy_vowel, &dummy_consonant);
            *month_number = reduce_number(imonth_number);
            // calculate the year number 
            if ( (ptr = strtok(NULL, delim)) != NULL) {
                strcpy(input, ptr);
                *unreduced_year_number = atoi(input);
                *year_number = reduce_number(*unreduced_year_number);
            } else printf("null ptr");
            // calculate the total date of birth number
            int date_of_birth_number = *day_of_birth_number + *month_number + *year_number;
            int reduced_date_of_birth_number = reduce_number(date_of_birth_number);
            return(reduced_date_of_birth_number);
        } else printf("null ptr");
    } else printf("null ptr");
    return(0);
}

int main() {
    // inputs to calculate
    char name[] = "Adam Bullock";
    char bday[] = "28/6/1988"; 
    
    // needed variables for calculation of the name.
    int total_whole_number = 0;
    int total_vowel_number = 0;
    int total_consonant_number = 0;
    int reduced_total = 0;
    int reduced_vowel = 0;
    int reduced_consonant = 0;
    
    // calculate name using calc_sentance function. 
    total_whole_number = calc_sentance(name, &total_vowel_number, &total_consonant_number, &reduced_total, &reduced_vowel, &reduced_consonant);
    printf("vowel number.%d/%d\nconesnant number.%d/%d\nwhole name number.%d/%d\n\n", total_vowel_number, reduced_vowel, total_consonant_number, reduced_consonant, total_whole_number, reduced_total);
    
    // calculate day of birth number
    int unreduced_year_number = 0;
    int year_number = 0;
    int month_number = 0;
    int day_of_birth_number = 0;
    int date_of_birth_number = calc_date_of_birth_number(bday, &day_of_birth_number, &month_number, &year_number, &unreduced_year_number);
    printf("day.%d month.%d year.%d/%d date_of_birth_number.%d, \n", day_of_birth_number, month_number, unreduced_year_number, year_number, date_of_birth_number);
    
    // calculate and print the important years of this persons life.
    printf("first important year = %d\nsecond important year = %d\n", reduce_number(year_number)+unreduced_year_number, unreduced_year_number+year_number);
    
    //iterate_months(1988, 2, 9);
    return(0);
}
