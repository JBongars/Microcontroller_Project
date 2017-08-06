#include <f330lib.h>
#include <stdio.h>
#include <stdlib.h>

/*
Misc Systems functions

*/




/*
//ceil Function

unsigned char ceil (unsigned char myNum, unsigned char myNum2){
    if(myNum > myNum2){return myNum;}
    else {return myNum2;}
}

signed char ceil (signed char myNum, signed char myNum2){
    if(myNum > myNum2){return myNum;}
    else {return myNum2;}
}

unsigned int ceil (unsigned int myNum, unsigned int myNum2){
    if(myNum > myNum2){return myNum;}
    else {return myNum2;}
}

signed int ceil (signed int myNum, signed int myNum2){
    if(myNum > myNum2){return myNum;}
    else {return myNum2;}
}
*/


/*
Assignment 1

- Write a simple Number Guessing Game.
- You are to first create a 8 bit variable, myNum and assigned it with a ASCII number. (For example '5')
- On the first row of the LCD, display "Guess the number".
- If the user guess the number correctly, display on the second row of the LCD, the message "Correct".
  Else display "Try Again".

- Bonus: Count the number of guess before the user gets it correct and display "Correct (n)", 
  where n is the number of guess before getting it correct.
*/

void numberGuessingGame(void){

    unsigned int myNum = rand(); //figure out random number
    
    unsigned char input_char; //to be used as initial input to test for bad inputs
    unsigned int input = 0; //input which is comprised of several input_char
    unsigned int input_previous = 0; //previous input, to be used to compare results

    unsigned char n = 1; //n counter initially set to 1
    unsigned char i; //generic counter(s)

    while(1){

        //initial screen
        lcdClrScr();
        lcdSetXY(0, 0);
        lcdPutStr("Guess my number!");

        while(1) {

            //get key from numpad with filter for hex values
            while(1){
                input = keyGet() - '0';
                
                //clears bottom row only
                lcdSetXY(0, 1);
                lcdPutStr("              ");
                lcdSetXY(0, 1);
                
                if (input < 10) break;

                //writes error message
                lcdSetXY(0, 1);
                lcdPutStr("Invalid Input!");
            }

            //reset screen
            lcdSetXY(0, 0);
            lcdClrScr();

            //display first number
            lcdPutDecU16(input);
            
            //get subsequent input_char to build input
            for(i = 0; i < 4; i++){
                
                //error checking for overflowed values
                if(input > 6553 && i == 2) {

                    //error is disguised as false answer
                    input = 60000;
                    break;
                }
                //move to the next digit
                input *= 10;
                
                //get key from numpad with filter for hex values (again)
                while(1){
                    input_char = keyGet() - '0';
                    
                    lcdSetXY(0, 1);
                    lcdPutStr("              ");
                    lcdSetXY(i + 1, 0);
                    
                    if (input_char < 10) break;

                    lcdSetXY(0, 1);
                    lcdPutStr("Invalid Input!");
                }
                
                lcdPutDecU16(input_char);
                input += input_char;
            }

            //if correct answer is given, while loop will break here
            if(input == myNum){
                break;

            } else {
                
                //else statement is given for appropriate reaction
                n += 1;

                lcdSetXY(0, 1);

                //first if and else if are for the initial reaction
                if(input > myNum && n == 2) {
                    lcdPutStr("Too much!");

                } else if(n == 2) {
                    lcdPutStr("Too little!");

                //all other reactions are given by comparing the previous input with the current
                } else if(abs(input - myNum) < abs(input_previous - myNum)) {
                        lcdPutStr("Warmer!!");

                } else {
                        lcdPutStr("Colder!!");
                }

                //save previous input once all checks are complete
                input_previous = input;
            }
        }

        //reset state to display win state
        lcdClrScr();
        lcdSetXY(0,0);

        //win state is given based on number of turns it took the player to guess the correct answer
        if(n == 1) {
            lcdPutStr("You got it on");
            lcdSetXY(0, 1);
            lcdPutStr("the first try!");

        } else if(n < 10){
            lcdPutStr("Wowe only "); lcdSetXY(0,1); lcdPutDecU8(n); lcdPutStr(" tries!!");
        } else if (n < 20){
            lcdPutStr("Ok.. only"); lcdSetXY(0,1); lcdPutDecU8(n); lcdPutStr(" tries.");
        } else {
            lcdPutStr("Got it in"); lcdSetXY(0,1); lcdPutDecU8(n); lcdPutStr(" tries...");
        }

        //pause to allow player to read win state, switch to end state
        delayms(1000);
        //reset input
        input = 0;

        //end state
        lcdClrScr();
        lcdSetXY(0, 0);
        lcdPutStr("Try again?");

        //force player to enable correct option
        while(1) {

            lcdSetXY(0, 1);
            lcdPutStr("1 - yes, 2 - no");
            
            input = keyGet() - '0';

            if (input > 2) {
                lcdSetXY(0, 1);
                lcdPutStr("Invalid Input..");
                delayms(1000);
                lcdSetXY(0, 1);
                lcdPutStr("1 - yes, 2 - no");;
            } else {
                break; //break out of while loop once acceptable answer is given
            }
        }
        
        if (input == 2){
            break; //terminate program if option 2 is given
        }
    }
}

/*
Assignment 2

Ask the user to enter 5 numbers using keypad.

  1. On the first row of the LCD, show "Enter 5 numbers"
  2. On the second row of the LCD, show '*' as the user enter the number

After the user has entered the 5 number

  1. Calculate the sum and the product of the 5 number
  2. Clear the LCD screen
  3. Display the 5 numbers the user has entered on the first row, each number separated by a comma.
     for e.g.  1, 4, 7, 2, 1
  4. On the second row display the sum and product separated by a comma.
     for e.g. 15, 56

Bonus: Take care of invalid entry such as 'A', 'B', 'C', 'D', '*' and '#'
*/

void fiveNumbers(void) {

    unsigned char myNum[] = {0, 0, 0, 0, 0}; //five inputs given by user
    unsigned char input; //defaut input variable
    unsigned char i; //counters

    unsigned char sumAnswer = 0; //Summation for above five inputs
    unsigned int productAnswer = 1; //Product for above five inputs

    lcdClrScr();
    i = 0;

    //get five inputs with validation
    while(i < 5){ 

        lcdSetXY(0, 0); 
        lcdPutStr("Pick Five Num!"); //prompt
        lcdSetXY(i, 1); //go to nth character
        
        input = keyGet() - '0'; 
        
        if (input < 10){ //validation
            myNum[i] = input;

            lcdPutDecU8(myNum[i]); //debugging
            //lcdPutChr('*'); //default
            i++;

        } else {
            lcdSetXY(0, 0);
            lcdPutStr("Invalid Input!"); //error checking
            delayms(1000);
        }
    } //get five numbers 

    lcdClrScr(); //reset to next state
    lcdSetXY(0, 0); 

    lcdPutDecU8(myNum[0]);

    for(i = 1; i < 5; i++){
        lcdPutStr(",");
        lcdPutDecU8(myNum[i]);
    }


    //calculate actual values
    for(i = 0; i < 5; i++){ 
        sumAnswer += myNum[i];
        productAnswer *= myNum[i];
    }

    lcdSetXY(0, 1); 

    //display summation one digit at a time (base 10)
    lcdPutStr("(");
    lcdPutDecU8(sumAnswer);

    //display product one digit at a time (base 10)
    lcdPutStr(",");
    lcdPutDecU16(productAnswer);

    lcdPutStr(")"); //Done message, return to main menu
    input = keyGet();

}

/*
Extra Credits


*/
void battleShip(void){
}


void masterMind(void){
    
    unsigned char computerPoints[8];

    unsigned char hits;
    unsigned char misses;

    unsigned char playerMove[8];
    
    signed char combinations_comp[4];
    signed char combinations_player[4];

    unsigned char turns;
    unsigned char input;

    unsigned char i;

    while(1){

        lcdClrScr();

        turns = 1;
        input = 0; //reset values before initialised

        lcdSetXY(0, 0);
        lcdPutStr("MASTER MIND!");

        lcdSetXY(0, 1);

        //generate random values between 0 to 3
        for(i = 0; i < 8; i++){
            computerPoints[i] = rand() >> 13; //set random points for computer
        }

        //player turn
        while(1){

            //reset all values
            hits = 0;
            misses = 0;
            for(i = 0; i < 4; i++){
                combinations_player[i] = 0;
                combinations_comp[i] = 0;
            }

            //get player turn playerMove
            for(i = 0; i < 8; i++){
                
                //error checking
                while(1){
                    playerMove[i] = keyGet() - '0';
                    if (playerMove[i] <= 3){
                        break;
                    }
                }
                
                //check for hit
                if(playerMove[i] == computerPoints[i]){
                    hits++;
                } 

                lcdSetXY(i, 1);
                lcdPutDecU8(playerMove[i]);
            }
            
            //calculate combinations
            for (i = 0; i < 8; i++){
                combinations_player[playerMove[i]]++;
                combinations_comp[computerPoints[i]]++;
            }

            /*
            //debugging
            lcdSetXY(0, 1);
            lcdPutStr("            ");
            for (i = 0; i < 4; i++){
                lcdSetXY(i, 1);
                lcdPutDecU8(combinations_player[i]);
                lcdSetXY(i+5, 1);
                lcdPutDecU8(combinations_comp[i]);
            }
            keyGet();
            */

            //tally combinations
            for (i = 0; i < 4; i++){
                if(combinations_comp[i] < combinations_player[i]){
                    misses = misses + combinations_comp[i];
                } else {
                    misses = misses + combinations_player[i];
                }
            }

            //final calculation
            //misses = 9 - misses - hits;

            misses = misses - hits;

            //win condition
            if(hits == 8){
                break;
            }

            lcdSetXY(0, 0);
            lcdPutStr("H = ");
            lcdPutDecU8(hits);

            lcdPutStr(" / M = ");
            lcdPutDecU8(misses);

            turns++;

            keyGet();
            lcdClrScr();
        }

        lcdClrScr();
        lcdSetXY(0, 0);

        if (turns < 21){
            lcdPutStr("Wow, only ");
            lcdPutDecU8(turns);
            lcdSetXY(0, 1);
            lcdPutStr("turns!!!");
        } else {
            lcdPutStr("Finished in ");
            lcdPutDecU8(turns);
            lcdSetXY(0, 1);
            lcdPutStr("turns!");
        }

        do {
            
            keyGet();

            lcdSetXY(0, 1);
            lcdPutStr("Again? 1=Y 2=N  ");

            input = keyGet() - '0';

            if(input > 2){
                lcdSetXY(0, 1);
                lcdPutStr("Invalid Input...");
                delayms(500);
            }

        } while(input > 2);

        if(input == 2) break;
    }

}

/*
main subroutine


*/

void main(void) {

    unsigned char option; //declared variable as handle for input

    Init_Device(); //initiate device

    while(1){

        lcdClrScr();  

        lcdSetXY(0, 0);
        lcdPutStr("Press 1 - Game"); //display options
        
        lcdSetXY(0, 1);
        lcdPutStr("Press 2 - Calc");

        option = keyGet() - '0';

        if(option == 1) {numberGuessingGame();} //option 1
        else if(option == 2) {fiveNumbers();} //option 2
        else if(option == 3) {masterMind();} //option 3
        
        //wrong input error handling
        else {
            lcdClrScr();
            lcdSetXY(0, 0);
            lcdPutStr("Wrong Input");
            delayms(1000);
        }
    }
}

