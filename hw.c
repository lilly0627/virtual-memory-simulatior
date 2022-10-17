//Hanting Li hantinl1 23253132

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MaxLine 80


struct Memory {
	int allocation;
	char value;
};

struct Memory main_memory[64];

void malloc1(int size){
    int i;
    int count = 0; // payload
    int check = 0; // if can allocate
    int index = 0; // index
    
    while(index < 64){
        if(main_memory[index].allocation < 0){ // unallocated
            count = main_memory[index].allocation; // payload header
            count = -count; // positive number of payload header
            if(count >= size){
                check = 1;
                break;
            }else{
                index -= main_memory[index].allocation; // add payload
                index += 1; // add header index
                index += 1; // add footer index
            }
        }else{ // allocated
            index += main_memory[index].allocation; // add payload
            index += 1; // add header index
            index += 1; // add footer index
        }
    }
    if(check == 1){ // allocating
        int left_free = count - size - 2;
        if(left_free < 1){ // use the whole section of free block
            main_memory[index].allocation = count;
            for(i = 0; i < count; i++){
                main_memory[index+1+i].allocation = 1;
            }
            main_memory[index+1+i].allocation = count;
            index++;
            printf("%d\n", index);          
        }else{
            main_memory[index].allocation = size;
            for(i = 0; i < size; i++){
                main_memory[index+1+i].allocation = 1;
            }
            main_memory[index+1+i].allocation = size;
            main_memory[index+1+i+1].allocation = -left_free;
            main_memory[index+count+1].allocation = -left_free;
            index++;
            printf("%d\n", index);
        }
    }
}


void free1(int index){
    int payload = main_memory[index-1].allocation; // 10
    int free_payload = -main_memory[index-1].allocation; // -10
    main_memory[index-1].allocation = free_payload; // -10
    int i;
    for(i = 0; i < payload; i++){
        main_memory[index+i].allocation = 0;
    }
    main_memory[index+i].allocation = free_payload;  
    if(index-1-1 >= 0){
        if(main_memory[index-1-1].allocation < 0){ // previous
            int together = -main_memory[index-1-1].allocation-main_memory[index-1].allocation+2;
            int prev_index = index-1+main_memory[index-1-1].allocation-1;
            main_memory[prev_index-1].allocation = -together;           
            for(i = 0; i < together; i++){
                main_memory[prev_index+i].allocation = 0;
            }
            main_memory[prev_index+i].allocation = -together;
            index = prev_index;
            payload = together; // positive number
        }
    }
    if(index+payload+1 < 64){
        if(main_memory[index+payload+1].allocation < 0){
            int together = -main_memory[index+payload+1].allocation - main_memory[index-1].allocation+2;
            main_memory[index-1].allocation = -together;  
            for(i = 0; i < together; i++){
                main_memory[index+i].allocation = 0;
            }
            main_memory[index+i].allocation = -together;
        }
    }
}


void blocklist(){
    int i;
    int count = 0;
    for(i = 0; i < 64; i++){
        if(main_memory[i].allocation < 0){
            if(count % 2 == 0){
                printf("%d, %d, free.\n", i+1, -main_memory[i].allocation);
                count++;
            }else{
                count++;
            }
         }
        if(main_memory[i].allocation > 1){
            if(count % 2 == 0){
                printf("%d, %d, allocated.\n", i+1, main_memory[i].allocation);
                count++;
            }else{
                count++;
            }
        }
    }
}


void writemem(int index, char* str){
    int i;
    int length = strlen(str);
    for(i = 0; i < length-1; i++){
        main_memory[i+index].value = *str;
        *str++;
    }
}


void printmem(int index, int numChar){
    int i;
    for(i = 0; i < numChar; i++){
        if(i != numChar-1){
            printf("%X ", main_memory[index+i].value);
        }else{
            printf("%X\n", main_memory[index+i].value);
        }
    }
}


void init(){
    int i;
    for(i = 0; i < 64; i++){
        main_memory[i].allocation = 0; // initialize all to 0
        main_memory[i].value = 0;
    }
    main_memory[0].allocation = -62;
    main_memory[63].allocation = -62;
}


int main(){
    init();
    char userInput[MaxLine];
	int childNum = 0;
	while (1) {
		printf(">");
		fgets(userInput, MaxLine, stdin);
		char* token = strtok(userInput, " ");
		if (!strncmp(token, "quit", 4)){
			break;
		}else if (!strncmp(token, "malloc", 6)){
			token = strtok(NULL, " ");
			int size = atoi(token);
			malloc1(size);
		}else if (!strncmp(token, "free", 4)){
			token = strtok(NULL, " ");
			int index = atoi(token);
			free1(index);
		}else if (!strncmp(token, "blocklist", 9)){
			blocklist();
		}else if (!strncmp(token, "writemem", 8)){
            token = strtok(NULL, " ");
			int index = atoi(token);
            token = strtok(NULL, " ");
            char* str = token;
			writemem(index, str);
		}else if (!strncmp(token, "printmem", 8)){
            token = strtok(NULL, " ");
			int index = atoi(token);
            token = strtok(NULL, " ");
            int numChar = atoi(token);
            printmem(index, numChar);
        }else{
        }
    }
    return 0;
}