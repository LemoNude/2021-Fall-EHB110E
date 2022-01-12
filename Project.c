//Esad Ömer Kurt 040190018

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


struct parameter{							//Struct to hold the momentary 
		int i;								//parameters of the game 
		int gained;	
		int score;	
		int bombstatus;
		int k;
		float ypos;
		float dif;
		float difficulty;
	}data;




void drawcity(void);									
int drawplane(int a, int score);
int menu(int a, int scrcur, struct parameter data, char adrs[]);

//body
int body(int xpos, float *yptr, int status, char word[11], char entry, int *const score, int reset); 
//body includes all the bomb mechanics



int main (){
	
	srand(time(NULL));
	
	clock_t t1,t2;		
		
	int  j=0, m=0, p_pos, bm_pos, wordlen = 0, lvl, menureturn, menuv=0;
	int  score = 0;
	char pool[5][11], word[11], c=80, adrs[32];		//c is the variable to store keyboard inputs, its default value is 80
	float *yptr=&data.ypos;					//vertical position and the point gained during
	int *ptrgain = &data.gained;			//the defusal is stored in the form
											//of a pointer to use in other functions
	FILE *codeptr;
	codeptr = fopen("codewords.txt","r");
	FILE *saveptr;
	
	
 // \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\			
 // ||||||||||||||||||||||||||||||||MAIN LOOP|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||MAIN LOOP|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||MAIN LOOP|||||||||||||||||||||||||||||||||||||||
 // ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
	
	for(data.i=5;data.i>=0;){		//data.i is the variable that increases by one every 100ms to progress the game
		
		for(;j<data.k;j++){				//data.k determines the words of the current difficulty levle,
		fscanf(codeptr, "%s", pool[j]);	//takes the next 5 words starting from the top
	}	
		while (_kbhit()) {
		c = _getch();					//taking the keyboard input
		}	
		
		if(c == 27) menuv = 1;			//menu v determines the menu type
										//if the keyboard input is "ESC", Pause Menu gets requested 
										
		if (menuv != 11){				//default value is 11, which means no menu is requested			
			
			c=80;
			
			menureturn=menu(menuv,score,data,adrs);		//menu function takes the menuv, score  
													//and the game paremeters and determines the next action
													//in the form of an integer output
			
			switch (menureturn)		{	//This depends on "menureturn" not "menuv"
			
			case 3:
		//case 3 if for loading from the given file	name	
		
			saveptr = fopen(adrs,"r"); //open and assign gven files address to the pointer
			
			fscanf(saveptr,"%d %d %d %d %d %f %f %f",&data.i,&data.gained,&data.score,&data.bombstatus,&data.k,&data.ypos,&data.dif,&data.difficulty);
			//assign the values from the file to the struct variables
			
			//assign the rest of the values to start the game from the saved state
			score = data.score;		
			data.bombstatus=0;
			fclose(codeptr);			
			codeptr = fopen("codewords.txt","r");
			for(m=-1;m<(int)data.difficulty;m++) {
				for(j=0;j<data.k;j++){
					fscanf(codeptr, "%s", pool[j]);
				} 		
			}
			p_pos=drawplane(data.i%72, score);
			body(bm_pos, yptr, data.bombstatus, word, c, ptrgain, 1);
			
				
			case 0:									//case 0 takes the current parameters, shows a freeze state for 0.6 secs
													//and continues the game so the player does't fail instantly
				system("cls");
				if((int)data.ypos == 0) score += data.gained;										
				p_pos=drawplane(data.i%72, score);													
				data.bombstatus = body(bm_pos, yptr, data.bombstatus, word, c, ptrgain, 0);			
				drawcity();																			
				usleep(600000);																		
				system("cls");
			menuv = 11;								//menuv=11 so the game continues after switch statement
			break;
			
			case -1:
	
			case 1: 								//case 1 is the reset case, it resets all the parameters and restarts the game
				data.i = 5;
				score = 0;
				data.gained = 0;
				data.bombstatus = 0;
																//to reset the word pool
				fclose(codeptr);								//codeword file is closed and
				codeptr = fopen("codewords.txt","r");			//opened again to reset the cursor
				for(j=0, data.k=5;j<data.k;j++){				//first 5 words are taken as before
				fscanf(codeptr, "%s", pool[j]);	}
				data.ypos = 1;
				body(bm_pos, yptr, data.bombstatus, word, c, ptrgain, 1);		//body function has a parameter at the end  
																				//which resets the static variables inside 
																				//the function if it is given as 1
				data.dif = 0.3;
				data.difficulty = 0;
				system("cls");
		
			menuv = 11;	
			if (menureturn != -1) break;												//an if case for game over menu to work
					
								
			case 2:
				t2 = clock() + 1*CLOCKS_PER_SEC;				//arranges the time values so nothing gets printed to the screen until next step
				menuv = 0;										//sets the menuv to 0 so the Start Menu requested
			break;
			
			}
			
			
			
		}
		
		
		t1 = clock();

		if(t1>=t2){																//didn't use a "sleep()" between loops to prevent input lag or even input loss
			t2 = t1 + 0.1 * CLOCKS_PER_SEC;										//instead "i" is increased according to a time constant
			system("cls"); 		
		if((int)data.ypos == 0) {												//data.ypos becomes 0 only if a bomb is defused
		score += data.gained;
		data.score = score;														//so the gained points gets added at that point
		data.gained=0;																	
		}		
		p_pos=drawplane(data.i%72, score);										//p_pos is the horizontal position of the plane which is returned by the
																				//drawplane(), it is used to drop the bomb in the right spot
																				
		if(data.bombstatus == 1)		*yptr += data.dif;								//data.bombstatus = 1 means that there is a bomb in the air
																						//so its distance to the sky increases by data.dif value
																						
		if(data.bombstatus == 0 && (data.i%72 > 6) && (data.i%72 < 60-strlen(word))){	//if the data.bombstatus=0 this means there s not a bomb and body() 
																						//function is waiting for signal to drop the bomb
																						//signal is given if the plane is far enough from 
																						//right and left side of the body, since there are words
																						//on the right, that side's distance is larger accordingly
																						//strlen() is used to find that value
																						
			data.bombstatus = 1;														//bomb drop signal is given if the conditions are right										
			bm_pos = p_pos;																//drop position is assigned
			strcpy(word,pool[rand()%5]);												//word to be printed is randomly choosen from the pool
		}		
		data.bombstatus = body(bm_pos, yptr, data.bombstatus, word, c, ptrgain, 0);		//body() func is called with the necessary parameters
																						//body() func returns the status of the bomb but also 
																						//uses the status value to drop the bomb
																						
																						//body()=0 -- there s no bomb in the air, waiting for signal
																						//body()=1 -- there s a bomb in the air
																						//body()=2 -- 5 consecutive defusals happened
																						//body()=-1 -- bomb hit the buildings
																						
																						
																						
		if (data.bombstatus == 2){														//data.bombstatus=2 indicates a difficulty increase									
			j = 0;																		
			data.difficulty++;															//data.difficulty is increased 
			if (data.difficulty<7) data.dif *= 1.01 ;else data.dif *=1.005;				//data.dif, which is bombs drop speed is increased by 1% until 10 letter words
																						//after that data.dif increases by %0.5 every 5 defuses
																						
			data.bombstatus = 0;														//after necessary operations, data.bombstatus is set to 0
																						//so the game could continue since a bomb is defused
		
		}else if (data.bombstatus == -1){												//data.bombstatus = -1 indicates that the bomb has fallen
		drawcity();																		//buildings are printed for the last time
		system("color 4");																//color is set to red for more dramatized effect												
		sleep(2);																		
		menuv = -1;																		//Game Over menu is requested
		continue;																		//skipped the remaining part of the loop
		} 
			
		c = 80;
		drawcity();																		//Draws the city under the body
		data.i++;
		}
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
}

void drawcity(void){
	//simple function, draws the buildings
	printf("  _____        _____    _________________ ________    _________\n ");
	printf("|o o o|_______|    |__|               | | # # # |____|o o o o|\n ");	
	printf("|o o o|* * *|: ::|. . |               |o| # # # |. . |o o o o|\n ");
	printf("|o o o|* * *|::  |. . | []  []  []  []|o| # # # |. . |o o o o|\n ");	
	printf("|o o o|**** |:  :| . .| []  []  []    |o| # # # |. . |o o o o|\n ");	
	printf("|_[]__|__[]_|_||_|__< |____________;;_|_|___[]__|_.|_|__[]___|\n ");		
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

int drawplane(int a, int score){
	//a function to print the plane and the scoreboard on the top
	//it outputs the position of the plane
	
	int i;
	char line1[73]={"                                                                         "};	//rows are initialized 
	char line2[73]={"                                                                         "};	//blank at the beginning 
	char line3[73]={"                                                                         "};
	char p1[]={"(|__.-. |"};	//plane parts are stored 
	char p2[]={"== ===_]+"};	//row by row
	char p3[]={"        |"};

	strncpy(line1+a-9, p1,9);	//plane characters gets copied ---> strncpy to avoid copying the end 
	strcpy(line2+a-9, p2);		//to the column according to the	character to be able to print scoreboard too
	strcpy(line3+a-9, p3);		//period determined by i%72
	
	//Plane and the scoreboard gets printed
	printf("%.51s Score: %4d\n",line1, score);	
	printf("%.62s\n",line2);					//Only 62 characters are printed so the plane exits 
	printf("%.62s\n",line3);					//the screen instead of disappearing instantly

	return a-5;	//planes position is returned
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

int body(int xpos, float *yptr, int status, char word[11], char entry, int *const score, int reset){
	//main function of the game that controls the bomb, given word, gained points, lose condition etc
	
	int i, point; 
	static int wpos = 0, sccs=0;			//variables needed to stay unchanged from loop to loop are assigned as static 
		
	if(reset == 1){							//the reset condition mentioned above
		wpos = 0;							//these variables are independent from the input
		sccs = 0;							//values so they need to be reset seperately
	}	
		
	int ypos = (int)*yptr;					//distance to the plane and score is 
	point = *score;							//assigned to a variable inside function
	
	char body[81][64];						//body is a 2d char array
	char blank[64] = {"                                                               "};
	for(i=0; i<18; i++){					
	strcpy(body[i],blank);}					//it is initialized as blank at first
	
		if(entry != 80) {					//entry is the last keyboard input
											//wpos is the word's index
											
			if (entry == word[wpos]) {		//it is checked if it is equal to the indexed letter of the word
											//if it is
				point++;					//player gets a potential point 
				wpos++;						//and the word's index increases by one, this number is used to print only the remaining letters
									
			}else  point--;					//if it is not equal to the indexed letter or the default character player gets a potential negative point
			entry = 80;
		}*score = point;					//point is assigned to the pointer so it can be used outside of the function
		
	if (status == 1){								//if the bombstatus is 1, prints the bomb and the word
	strcpy((body[ypos])+xpos-1,"<|>");				//to the column determined by yposcolumn where the plane was at the dropping moment 
	strncpy((body[ypos+1])+xpos,"|",1);				//print the word startig at the wpos index, this makes it so the correct entries disappear
	strcpy((body[ypos+1])+xpos+1,word+wpos);
	strcpy((body[ypos+2])+xpos,"*");
	}	
	
	
	for(i=0; i<18; i++) printf("%s\n",body[i]);		//printing the rows
	
	//return value evalution
	
	if((int)*yptr == 16){							//if bomb hits the buildings return -1 --> triggers the Game Over menu
		return -1;
	}else if(wpos == strlen(word)){					//if a word is entered correctly
		*yptr = 0;									//reset the position
		sccs++;										//increase the consecutive true entry number (or sccs) by one
		
		if (sccs == 5) {							//if sccs is 5 
			sccs=0;									//reset the sccs 
			wpos = 0;								//reset the index
 			return 2;}								//return 2 -->  triggers difficulty increase and new bomb signal
 			
 			
		wpos = 0;									//if a word is cleared but sccs is not 5 just reset the index	
		return  0;									//return 0 --> triggers new bomb signal
	}else return status;							//if none happens, return the status as it was so the loop occurs one more

		
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

int menu(int a, int score, struct parameter data, char adrs[]){
	
	//menu function
	
	system("cls");	//clears the screen
	printf("\n\n");
	
	//"a" indicates the reason of menu being opened
		switch (a){
			
			case -1:																	//game was over
			
				system("color 7");														//reset the color to white from red
				printf("\tGAME\n\tOVER\n\nScore: %d\n\n",score);						//print the "Game Over" to top
				system("pause");														//wait for a keyboard input
				return -1;																//return -1 so the game resets
				break;
			case 0:																		//game was started
																					
				printf("\tMENU\n\n\tEnter - Start\n\tL - Load\n\tQ - Quit\n\t");		//present the choices	
				while (1) {																//take the keyboard input in real time
				char c = _getch();	
				if(c == 13) return 1;													//if  ENTER is pressed reset the initial values and start the game
				
				else if (c=='l'){														//if 'l' was pressed, load save from file
					
				auto FILE *saveptr;	
				auto char name[32];	
					
				printf("\n\tName of the save file:  ");
				scanf("%s",name);														//ask for a file name
				sprintf(name,"%s.txt",name);											
				if( !(saveptr = fopen(name,"r")) ){										//check if file exists by trying to open it in read mode
					system("cls");														//if it doens't
					printf("\n\t%s",strerror(2));										//print an error message
					sleep(2);
					return 2;															//return 2 so start menu is requested after
				}else{
					fclose(saveptr);													//it the file exists, close the file
					sprintf(adrs,"%s",name);											//assign the files name to a pointer to extract it into the main func
						
				}
				return 3;																//return 3 to initiate loading process in the main function
				} 
				else if (c=='q'){														//if 'q' was pressed
				exit(0);		}														//exit the game
				}
				
			case 1:
			//if 'ESC' was pressed, open the Pause Menu		
				
				//Show the possible actions 
				printf("\tPAUSE\n\tMENU\n\n\tScore = %d\n\n\tESC - Continue\n\tS - Save\n\tM - Back to menu\n\tR - Restart\n\tQ - Quit\n\t",data.score);
							
				char c = _getch();											//take the input in real time using _getch()
				
				switch (c){													
				case 27:													//ESC for continue
					return 0;												
				case 'r':													//r for restart
					return 1;												
				case 'm': 													//m for reset and main menu
					return 2;												
				case 's':													//s for save
					system("cls");
					auto FILE *saveptr;										//create a temp char array and a FILE pointer
					auto char name[32];
					int i = 0;
					system("cls");
					while(i==0){
						printf("\n\tName of the save file:  ");				
						scanf("%s",name);									//ask for the file name
						sprintf(name,"%s.txt",name);						//add the ".txt" extension
						
						if(fopen(name,"r")){								//check if file exists by trying to open it in read mode, if it does ask for the next action
							printf("\tThis file already exists.\n\tDo you want to overwrite?   y/n\tType \"c\" to cancel\n\n\t"); 	
							while(1){
								c = getchar();								//get the input
								if(c=='y'){									//if y, overwrite the file by deleting and creating it again
								remove(name);								
								i=1;										//break both loops
								break;							
								} 
								else if(c=='n') {							//if n, break the inner loop to ask for another file name
								system("cls");	
								break;	
								}else if(c=='c') return 0;					//if c, cancel the operation and continue the game		
								
								}
							}else {
							i=1;											//if the filename is available
							printf("\n\tName available\n\n\t");				//indicate at it
							}
						} 
						saveptr = fopen(name,"w");							//create the file by trying to open it
						
						//print the parameters of the game into it					
						fprintf(saveptr,"%d\n%d\n%d\n%d\n%d\n%f\n%f\n%f",data.i,data.gained,data.score,data.bombstatus,data.k,data.ypos,data.dif,data.difficulty);
						fclose(saveptr);									//close the file
						printf("\n\tSave succesful");						//give indication for the success
						sleep(2);
							
				return 2;													//return 2 to open the Main Menu
				
				case 'q':													//if 'q' was pressed, exit the game
					system("cls");
					exit(0);
				
			}
		}	
}









