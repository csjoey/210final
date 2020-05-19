#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sense/sense.h>
// Joseph Loporto file containing game code
// Orientation is the ports on the left side of the pi with 0,0 in the bottom left of the screen

// Globals

int ENTER_PRESSED = 0;

// Structs
struct game_state{
	int stack_x, stack_y, game_over;
}; // Strunct to contain all necessary informarion about the game to be used to recreate it

// Functions

// Function to push buffer to pi framebuffer
void pushToFrameBuffer(pi_framebuffer_t* FBD, int buffer[8][8]){
	for(int x=0;x<8;x++){
		for(int y=0;y<8;y++){
			FBD->bitmap->pixel[y][7-x] = buffer[x][y]; // Draw flipping to orient 0,0 in bottom left
		}
	}
}
// Joystick Callback
void joyCall(unsigned int code){
	if(code == 28){ // 28 = KEY_ENTER
			ENTER_PRESSED = 1;
	}
}
// function to clear screen capable of being used outisde of game loop
void outsideClear(){
	pi_framebuffer_t* FBD = getFBDevice();
	clearBitmap(FBD->bitmap,0);
	freeFrameBuffer(FBD);
}

struct game_state game(struct game_state current){
	// Constants
	int colorBlank = 0;
	int colorGreen = getColor(0,255,0);
	
	// Setup Devices
	pi_framebuffer_t* FBD = getFBDevice();
	pi_joystick_t* STICK = getJoystickDevice();
	
	// Env Vars
	int buffer[8][8] = {{0}};

	// Check to see if game is over and safety to make sure you cant seg fault
	if((current.stack_y >= 8) || current.game_over){
		current.game_over = 1;
		return current;
	}

	//Draw Current Screen to buffer
	for(int y=0;y<current.stack_y;y++){
		buffer[current.stack_x][y] = colorGreen;
	}
	pushToFrameBuffer(FBD,buffer);
	
	// Setup this turns physics variables
	int turnDone = 0;
	int xVel = 1;
	int xPos = current.stack_x;
	int yPos = current.stack_y;
	while(!turnDone){
		// Reverse velocity if at the edge
		if(xPos >= 7){
			xVel = -1;
		}

		if(xPos <= 0){
			xVel = 1;
		}
		// Update position based on xVel
		xPos += xVel;
		// Display change and then undo last dot
		buffer[xPos][yPos] = colorGreen;
		pushToFrameBuffer(FBD,buffer);
		buffer[xPos][yPos] = 0;
		// Poll joystick and do it faster the higher we are
		pollJoystick(STICK,joyCall,200-(yPos*20));
		// Check if the button was pressed
		if(ENTER_PRESSED){
			turnDone = 1;
			ENTER_PRESSED = 0;
		}
	}
	// If the game is on the first block then set the stack location
	if(current.stack_y == 0){
		current.stack_x = xPos;
	}
	// If the block was stacked in the right spot then increment the y height else end the game
	if(xPos == current.stack_x){
		current.stack_y += 1;
	}else{
		current.game_over = 1;
	}
	// Cleanup display session and return game state
	freeJoystick(STICK);
	freeFrameBuffer(FBD);
	return current;
}

