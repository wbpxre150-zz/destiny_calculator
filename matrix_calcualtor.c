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
 * add address calcualtion and print. (use sentance structure, and calc sentance) 
 * Read input from and output to a text file in csv format, to load results into spreadsheet to view and use. 
 * add matrix of compatibility charts for iterate_months
 * add pythagoras matrix, to add name and birth numbers together to find missing numbers
*/

// include needed libs. 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 
 hard coded macro to define the alphabet system used. 
 0 is hebrew, does not use 9. 
 1 is normal english gematria.
 2 is the hebrew and english system reduced together, interestingly the diffrence is 9 ;) 
*/
#define system 1

// flags to make code easier to read
#define IGNORE_SPECIAL_NUMBER 0
#define SPECIAL_NUMBER 1

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

// Data Structure for a birthday. 
struct sbday {
    char *bday;
    int day_of_birth;
    int reduced_day_of_birth;
    int final_day_of_birth;
    int month_of_birth;
    int reduced_month_of_birth;
    int year_of_birth;
    int reduced_year_of_birth;
    int final_year_of_birth;
    int date_of_birth_number;
    int reduced_date_of_birth_number;
    int first_impotant_year;
    int second_important_year;
};

// Data structure for name
struct ssentance {
    char *sentance;
    int total_whole_number;
    int reduced_total;
    int total_vowel_number;
    int reduced_vowel;
    int total_consonant_number;
    int reduced_consonant;
};

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
        else if ( bvowel == 0 )  { 
            *consonant_total += j;
        }
        //printf("letter.%c = %d type.%d\n", input_string[len], j, bvowel);
        len++;
    }
    return(y);
}

int reduce_number(int number, int special_number) {
    // 11 and 22 should not be reduced.
    if ( special_number != 0 && (number == 11 || number == 22) )
        return(number);
    // limit is a 32 digit length number
    char snumber[32] = "";
    sprintf(snumber, "%d", number);
    return(calc_word(snumber, &dummy_vowel, &dummy_consonant));
}

int reduce_to_single_number(int number, int special_number) {
    int ret = number;
    while (ret > 9) {
        ret = reduce_number(ret, special_number);
        if ( special_number != 0 && (ret == 11 || ret == 22) ) break; // prevent infinate loop if function called incorrectly. 
    }
    return(ret);
}

void find_compatible_matches(int median_year, int range, int seeking_number) {
    // function to return birthdays reducing to an input number within a range of years. 
    // starting year is first number - second number. ending year is first number + second number. 
    // last number is the one being searched for. use the destiny number and the chart to find applicable numbers.
    // todo: add 3 dimentional matrix holding the business and personal compatibility numbers.
    for (int i=median_year-(range/2); i<median_year+(range/2); i++) {
        for (int y=0; y<12; y++) {
            for (int z=1; z<( i % 4 == 0 && y == 1 ? num_days_in_month[y]+1 : num_days_in_month[y]); z++) {
                char str[16];
                sprintf(str, "%d%d%d", z, y+1, i);
                int reduced_year = reduce_to_single_number(calc_word(str, &dummy_vowel, &dummy_consonant), IGNORE_SPECIAL_NUMBER);
                if ( reduced_year == seeking_number ) {
                    printf("    %2d/%2d/%4d\n", z, y+1, i);
                }
            }
        }
    }
}

void calc_sentance(struct ssentance *sentance) {
    // inputs sentance string, splits into words
    // calculates and fills in variables in structure. 
    // prints each word as it goes, this needs to output to some file format to be readable. 
    const char delim[] = " ";
    // assign the first word of sentance to the pointer
    char *ptr = strtok(sentance->sentance, delim);
    if ( ptr != NULL ) {
        do {
            char word[256]; // limits word to 256 char length. 128 may be enough?
            int whole_number = 0;
            int vowel_number = 0;
            int consonant_number = 0;
            // copy word into temp memory position, and calculate its totals. 
            strcpy(word, ptr);
            whole_number = calc_word(word, &vowel_number, &consonant_number);
            // add this word to the total of all words. 
            sentance->total_whole_number += whole_number;
            sentance->total_vowel_number += vowel_number;
            sentance->total_consonant_number += consonant_number;
            // reduce each word total and print the result for this word. 
            int ireduced_vowel = reduce_to_single_number(vowel_number, SPECIAL_NUMBER);
            int ireduced_consonant = reduce_to_single_number(consonant_number, SPECIAL_NUMBER);
            int ireduced_total = reduce_to_single_number(whole_number, SPECIAL_NUMBER);
            printf("%s = vowel.%d/%d consonant.%d/%d total.%d/%d\n", word, vowel_number, ireduced_vowel, consonant_number, ireduced_consonant, whole_number, ireduced_total);
            // move onto the next word. 
            ptr = strtok(NULL, delim);
        } while ( ptr != NULL) ; // stop when there are no words left.
    }
    // reduce the total numbers to single digits and return the results. 
    sentance->reduced_total = reduce_to_single_number(sentance->total_whole_number,SPECIAL_NUMBER);
    sentance->reduced_vowel = reduce_to_single_number(sentance->total_vowel_number,SPECIAL_NUMBER);
    sentance->reduced_consonant = reduce_to_single_number(sentance->total_consonant_number,SPECIAL_NUMBER);
}

void calc_date_of_birth_number(struct sbday *bday) {
    // set bday.bday = dd/mm/yyyy then call this function
    // takes structure bday, fills all variables from input string.
    const char delim[] = "/";
    char *ptr = NULL;
    if ( (ptr = strtok(bday->bday, delim)) != NULL) {
        char input[8];
        strcpy(input, ptr);
        // calculate the day of birth number
        bday->day_of_birth = atoi(input);
        bday->reduced_day_of_birth = reduce_number(bday->day_of_birth, IGNORE_SPECIAL_NUMBER);
        bday->final_day_of_birth = reduce_number(bday->reduced_day_of_birth, IGNORE_SPECIAL_NUMBER);
        // calculate month number 
        if ( (ptr = strtok(NULL, delim)) != NULL) {
            strcpy(input, ptr);
            bday->month_of_birth = atoi(input);
            bday->reduced_month_of_birth = reduce_number(bday->month_of_birth, IGNORE_SPECIAL_NUMBER);
            // calculate the year number 
            if ( (ptr = strtok(NULL, delim)) != NULL) {
                strcpy(input, ptr);
                bday->year_of_birth = atoi(input);
                bday->reduced_year_of_birth = reduce_number(bday->year_of_birth, IGNORE_SPECIAL_NUMBER);
                bday->final_year_of_birth = reduce_to_single_number(bday->reduced_year_of_birth, IGNORE_SPECIAL_NUMBER);
                // calculate first and possible second important years of this persons life. 
                bday->first_impotant_year = bday->year_of_birth + bday->final_year_of_birth;
                bday->second_important_year = bday->year_of_birth + bday->reduced_year_of_birth;
                 // calculate the total date of birth number
                char fulldob[16];
                sprintf(fulldob,"%d%d%d", bday->day_of_birth, bday->month_of_birth, bday->year_of_birth);
                bday->date_of_birth_number= calc_word(fulldob, &dummy_vowel, &dummy_consonant);
                bday->reduced_date_of_birth_number = reduce_number(bday->date_of_birth_number, SPECIAL_NUMBER);
            } else printf("null ptr\n");
        } else printf("null ptr\n");
    } else printf("null ptr\n");
}

int moon_phase(int year, int month, int day) {
    // approximation of moon phase, seems accurate within 1-2 days for the year 2020.
    // need to check other years. 
	int r = year % 100;
	r %= 19;
	if (r>9){ r -= 19;}
	r = ((r * 11) % 30) + month + day;
	if ( month != 1 && month < 3 ) {
        r += 2;
    } else { 
        r += 1;
    }
	r -= ((year<2000) ? 4 : 8.3); // small rounding error here due to integer math... 
	r = r % 30;
	return ((r < 0) ? r+30 : r);
}

void calc_personal_years (int day_of_birth_number, int month_of_birth, int year_of_birth, int dob_number, int vowel_number, int name_number, int ending_year) {
    /* 
    * personal years run from birthday to birthday, day of birth + month of birth + year of last birthday
    * split into 3 4 month periods, age+ year, dob_num + year, vowel_number + year. 
    * calc days, middle number (dob + name) + reduced date of the day. 
    * 
    * TODO: maybe this can actually start the personal year exactly on the birthday, rather than an approximate start at the next month. 
    * example, if birthday is 1st december, the current algorithim will start personal year at 1st january, leaving 31 days in december with the wrong personal year number. 
    */
    month_of_birth = month_of_birth == 12 ? 0 : month_of_birth; // if december start at january. 
    int middle_number = dob_number + name_number;
    int k = year_of_birth;
    for (int i=year_of_birth; i<ending_year; i++) {
        int j = 0;
        // calcualtions for each year
        char snumber[16];
        sprintf(snumber, "%d%d%d", day_of_birth_number, month_of_birth, i);
        int personal_year_number = calc_word(snumber, &dummy_vowel, &dummy_consonant);
        int reduced_personal_year_number = reduce_to_single_number(personal_year_number,IGNORE_SPECIAL_NUMBER);
        printf("New Year.%d = %d/%d\n", i, personal_year_number, reduced_personal_year_number);
        for (int y=month_of_birth; y<12; y++) {
            // increase k by one each january, to display the actual date in the prints, to allow searching for a particular day. 
            if ( y == 0 ) k++;
            // calculations for each 4 month period. 
            if ( j == 0 ) {
                // calculate age, add age to year. 
                int age = i-year_of_birth;
                printf("    First 4 month period = %d\n", reduce_to_single_number(age+i, IGNORE_SPECIAL_NUMBER));
            } else if ( j == 4) {
                // add date of birth number to year
                printf("    Second 4 month period = %d\n", reduce_to_single_number(dob_number+i, IGNORE_SPECIAL_NUMBER));
            } else if ( j == 8) {
                // add vowel number to year
                printf("    Third 4 month period = %d\n", reduce_to_single_number(vowel_number+i, IGNORE_SPECIAL_NUMBER));
            }
            // calculations for each month 
            int personal_month_number = personal_year_number+y+1;
            int reduced_personal_month_number = reduce_to_single_number(personal_month_number,IGNORE_SPECIAL_NUMBER);
            printf("        New Month.%d = %d/%d\n", y+1, personal_month_number, reduced_personal_month_number);
            for (int z=0; z<( i % 4 == 0 && y == 1 ? num_days_in_month[y]+1 : num_days_in_month[y]); z++) {
                // calculations for each day
                sprintf(snumber, "%d%d%d", z+1, y+1, i);
                int day_number = calc_word(snumber, &dummy_vowel, &dummy_consonant);
                int actual_day_number = reduce_to_single_number(day_number+middle_number,IGNORE_SPECIAL_NUMBER);
                int phase = moon_phase(k,y+1,z+1);
                char moonph[4] = {0};
                if (phase == 15)
                    sprintf(moonph, "[F]");
                else if (phase == 0)
                    sprintf(moonph, "[N]");
                printf("            %d/%d/%d = %d + %d = %d/%d %s\n", z+1, y+1 ,k, day_number, middle_number, day_number+middle_number, actual_day_number, moonph);
            }
            y = y == 11 ? -1 : y;
            if ( ++j > 11 ) break;
        }
    }
}

int main() {
    // inputs to calculate
    char strname[] = "Adam Bullock";
    char strbday[] = "28/6/1988";
    char straddress[] = "1 Crab Creek Road Broome Western Australia";
    int  max_age_gap = 4;
    
    // structures for name and birthday, set all variables to 0.
    struct sbday bday;
    memset(&bday, 0, sizeof(bday));
    struct ssentance name;
    memset(&name, 0, sizeof(name));
    struct ssentance address;
    memset(&address, 0, sizeof(address));
    // assign birthday string and name string to structures. 
    name.sentance = strname;
    bday.bday = strbday;
    address.sentance = straddress;
    
    // calculate and print name 
    calc_sentance(&name);
    printf("vowel number.%d/%d\nconesnant number.%d/%d\nwhole name number.%d/%d\n\n", name.total_vowel_number, name.reduced_vowel, name.total_consonant_number, name.reduced_consonant, name.total_whole_number, name.reduced_total);
    
    // calculate and print the date of birth
    calc_date_of_birth_number(&bday);
    printf("day.%d/%d/%d month.%d/%d year.%d/%d/%d\n  date_of_birth_number.%d/%d\n  first important year.%d\n  second important year.%d\n\n", bday.day_of_birth, bday.reduced_day_of_birth,bday.final_day_of_birth,  bday.month_of_birth, bday.reduced_month_of_birth, bday.year_of_birth, bday.reduced_year_of_birth, bday.final_year_of_birth, bday.date_of_birth_number, bday.reduced_date_of_birth_number, bday.first_impotant_year, bday.second_important_year);
    
    // calcuate addrress numbers. 
    calc_sentance(&address);
    printf("vowel number.%d/%d\nconesnant number.%d/%d\nwhole name number.%d/%d\n\n", address.total_vowel_number, address.reduced_vowel, address.total_consonant_number, address.reduced_consonant, address.total_whole_number, address.reduced_total);
    
    // find and print personal year, 4 month periods, months and days. 
    printf("Personal Year, thirds of year, month and day calcultions:\n");
    calc_personal_years (bday.final_day_of_birth, bday.month_of_birth, bday.year_of_birth, bday.reduced_date_of_birth_number, name.reduced_vowel, name.reduced_total, 2021);
    
    // find compatible business or personal matches based on date of birth only. You need a chart to determine what number to seek for until I add it to be done automatically. 
    printf("\nCompatible dates of birth for business or personal relationships:\n");
    find_compatible_matches(bday.year_of_birth, max_age_gap, 9);
    
    return(0);
}
