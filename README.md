# Microcontroller_Project

Project files for my microcontroller  final project for DEEC, Ngee Ann Polytechnic Diploma in Engineering (Electronic). 
  1=Number Guessing game; 
  2=Five number calc; 
  3=Mastermind
  
# License

This program is subject to an Educational license of entitlement may be used solely for purposes directly related to learning, training, research or development at the discretion of Ngee Ann Polytechnic in Singapore. This program cannot be used for commercial, professional or any other for-profit purposes.

# System Info and Commands

This program was configured on a DEMPA F330 Kit using a Toolstick + C2 Board. Compiled on Silicon Laboratories IDE.

Commands
=====================
unsigned int rand(void) = outputs random number
unsigned int abs(unsigned int input) = outputs absolute value of input

void lcdInit(void) = initiates device
void lcdClrScr(void) = clears LCD screen
void lcdPutCh(unsigned cahr ch) = Prints ASCII equivalent of ch on LCD screen and moves placeholder one space to right
void lcdPutStr(char *str) *= Prints string on screen and moves placeholder to the following space.
void lcdSetXY(unsigned char x, unsigned char y) = moves placeholder to (x, y) position on screen

void lcdPutDecU8(unsigned char num) 
void lcdPutDecS8(signed char num)
void lcdPutDecU16(unsigned int num)
void lcdPutDecS16(signed int num) = = prints char/int value on screen

void delayms(unsigned int n) = pauses the controller for n milliseconds

Electronic Diagram
====================
![Alt text](/pictures/Schematic.jpg?raw=true "Optional Title")


# Number Guessing game

User has to guess a random number between 1 and 65552. On the first round, the microcontroller will either output "Too high" or "Too low" depending on the input. After the first round, the microcontroller will output "Warmer" or "Colder" depending on how close the current guess is compared to the previous. Once the correct number has been guessed, the number of attempts used to guess the correct number will be read back to the user.

# Number Calculator

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

Take care of invalid entry such as 'A', 'B', 'C', 'D', '*' and '#'

# Mastermind

User will have to guess random sequence of 8 numbers where each number can be either a 0, 1, 2 or 3.
If the user guesses the correct number in the correct place, it is counted as a hit;
else if the number is the correct number but not in the correct place, it is counted as a miss
At the end of every round, the total number of hits and misses is presented to the player.
The number of attempts is presented to the user once they have guessed the correct number sequence.

# Limitations

There are two limitations with this program which I am trying to work on. The first is that the program cannot be intterupted at anytime to return to the main menu. The second is that random numbers are delivered via the rand() function in #stdlib. This causes the exact same random numbers to be returned upon program launch.

# Improvements

Alas, the microcontroller had to be returned shortly after completion of this first prototype. I am planning to write follow up programs using javascript once I have some time. If it were to be rewritten using C++, a visual interface would have to be built around the current programs which I am conflicted as to whether this would be worth the time doing. One idea that could make this work however would be future development in a chess application (still under consideration)

# Special Thanks and additional Comments

I want to thank my teacher, Mr Seng Puay Hong and Mr Tey Ching Sze for their continued help, support and also understanding in my current work requirements. Thanks to all

