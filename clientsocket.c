#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>  
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define MAX 80

char operation[4];
int sizeOfFirstOperand;
int sizeOfSecondOperand;
int firstNum;
int secondNum;

void removeSpaces(char *str)
{
    // To keep track of non-space character count
    int count = 0;
 
    // Traverse the given string. If current character is not space, then place it at index 'count++'
    for (int i = 0; str[i]; i++)
        if (str[i] != ' ')
            str[count++] = str[i]; // here count is incremented
    str[count] = '\0';
}

const char* getFirstOperand(char *str)
{
    char * firstInteger;
    
    // To keep track of non-space character count
    int count = 0;
 
    // Traverse the given string. If current character is not space, then place it at index 'count++'
    for (int i = 0; str[i]; i++)
    {
    	if (str[i] == '+')
        {
        	strcpy(operation,"add");
        	sizeOfFirstOperand = i;
        }
        if (str[i] == '-')
        {
        	strcpy(operation,"sub");
        	sizeOfFirstOperand = i;
        }
        if (str[i] == '/')
        {
        	strcpy(operation,"div");
        	sizeOfFirstOperand = i;
        }
        if (str[i] == '*')
        {
        	strcpy(operation,"mul");
        	sizeOfFirstOperand = i;
        }
    }
    
    firstInteger = malloc(sizeof(char)*sizeOfFirstOperand);
    strncpy(firstInteger,&str[0],sizeOfFirstOperand);
    
    return firstInteger;
}

const char* getSecondOperand(char *str)
{
	char * secondInteger;
	sizeOfSecondOperand = strlen(str)-(sizeOfFirstOperand+1);
	secondInteger = malloc(sizeof(char)*sizeOfSecondOperand);
    	strncpy(secondInteger,&str[sizeOfFirstOperand+1],sizeOfSecondOperand);
    	return secondInteger;
}

struct packet
{
	int a;
	int b;
	char operator[4];
};

struct result_packet
{
	int successOrFail;
	double resultOrError;
};


int main(int argc, char *argv[])
{
    int sock;

    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) 
        printf("ERROR opening socket."); 
    puts("Socket created.");	

    server.sin_addr.s_addr=inet_addr("127.0.0.1");   
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);

    if (connect(sock,(struct sockaddr *)&server,sizeof(server)) < 0){ 
        printf("ERROR connecting.");
	return 1;
}
    puts("Connected.");

struct packet packet1;
struct result_packet packet2;

while(1){
    
    char message[MAX], server_reply[MAX];
    printf("\nPlease enter the operation: ");
    scanf("%s",message);
    puts(message);
    puts("");
    removeSpaces(message);
    char firstInteger[strlen(getFirstOperand(message))];
    strcpy(firstInteger,getFirstOperand(message));
    char secondInteger[strlen(message)-strlen(firstInteger)-1];
    strcpy(secondInteger,getSecondOperand(message));
    firstNum = atoi(firstInteger);
    secondNum = atoi(secondInteger);
    packet1.a=firstNum;
    packet1.b=secondNum;
    strcpy(packet1.operator,operation);
    
    if (send(sock,&packet1,sizeof(packet1),0)< 0){ 
         printf("ERROR writing to socket.");
	return 1;
	}
     
   if (recv(sock,server_reply,MAX,0)<0){
	puts("Recv failed.");
	break;
	}

puts("Server Reply:");
puts(server_reply);

}
close(sock);
    return 0;
}
