#include <netinet/in.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include "game.c"
#define CLIENT_C 720
#define SERVER_C 370
#define BUFF_SIZE 2048
#define SA struct sockaddr 
// Joseph Loporto final project server/client to play stacker with a friend
// Joystick to start and then place blocks
int MODE;
int GAME = 1;
int RECV_FLAG = 0;
int PORT;
void serv_game(int sockfd){ 
	//Servers handshake
	char buff[BUFF_SIZE]; 
	bzero(buff, BUFF_SIZE); 
	read(sockfd, buff, sizeof(buff)); 
	if(strcmp(buff,"GAMESTART") != 0){
		bzero(buff, BUFF_SIZE); 
		strcpy(buff,"CONFIRM");
    	}
	write(sockfd, buff, sizeof(buff)); // write start confirmation
	bzero(buff,sizeof(buff)); // clear buffer
	struct game_state gameS = {0,0,0};

	while(!gameS.game_over){
		read(sockfd,buff,sizeof(buff)); // read first game state
		memcpy(&gameS,buff,sizeof(buff));
		bzero(buff,sizeof(buff));
		gameS = game(gameS); // play game
		memcpy(buff,&gameS,sizeof(gameS));
		write(sockfd,buff,sizeof(buff));
		bzero(buff,sizeof(buff));
	}
	outsideClear();
	printf("Game over...Exiting\n");
}  

void client_game(int sockfd){ 
    	char buff[BUFF_SIZE]; 
	//Clients handshake
        bzero(buff, sizeof(buff));
       	strcpy(buff,"GAMESTART");	 
        write(sockfd, buff, sizeof(buff)); 
        bzero(buff, sizeof(buff)); 
        read(sockfd, buff, sizeof(buff));
	if(strcmp(buff,"CONFIRM") != 0){
		printf("Handshake confirmed \nStarting game \n");
	}
	struct game_state gameS = {0,0,0}; // Setup game state
	while(!gameS.game_over){
		gameS = game(gameS); // Play round
		memcpy(buff,&gameS,sizeof(gameS)); // Copy state into buffer
		write(sockfd,buff,sizeof(buff)); // Write buffer to socket
		bzero(buff,sizeof(buff)); // Clear buffer
		read(sockfd,buff,sizeof(buff)); // Wait for game state as response
		memcpy(&gameS,buff,sizeof(buff)); // Copy received game state into ours
		bzero(buff,sizeof(buff));
	}
	printf("Game over...Exiting \n");
	outsideClear();
} 
 
 

int main(int argc, char const *argv[]){
	if(argc == 3){
        	PORT = atoi(argv[1]);
		MODE = CLIENT_C;
	}else if(argc == 2){
        	PORT = atoi(argv[1]);
		MODE = SERVER_C;
	}else{
		printf("Invalid argument count \"port\" for server or \"port server\" for server \n");
		return -1;
	}
	// BEGIN CODE FOR CLIENT
	if(MODE == CLIENT_C){
    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else{
        printf("Socket successfully created..\n"); 
    }
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
  
    // connect the client socket to server socket 
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else{
        printf("connected to the server..\n"); 
    }
    // function for chat 
    client_game(sockfd); 
  
    // close the socket 
    close(sockfd); 

	}
	// BEGIN CODE FOR SERVER
	if(MODE == SERVER_C){

    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    }else{
        printf("Socket successfully created..\n"); 
    }    
    bzero(&servaddr, sizeof(servaddr)); 
    
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    }else{
        printf("Socket successfully binded..\n"); 
    }
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    }else{
        printf("Server listening..\n"); 
    }
    len = sizeof(cli); 
    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0){ 
        printf("server acccept failed...\n"); 
        exit(0); 
    }else{
        printf("server acccept the client...\n"); 
    }
    // Function for chatting between client and server 
    serv_game(connfd); 
  
    // After chatting close the socket 
    close(sockfd);
    }
}











