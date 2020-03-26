#include <stdlib.h>
#include <stdio.h>
#include "stats.h"

typedef struct  {
	int FactoryNumber;
	int Made;
	int Eaten;
	float Min_Delay;
	float Avg_Delay;
	float Max_Delay;
}Node;

Node* stats;
int totalnumber = 0;

void stats_init(int num_producers){
	totalnumber = num_producers;
	stats = malloc(sizeof(Node)*num_producers);  // create array of 6 different things
	for(int i = 0; i < num_producers; i++){
		stats[i].FactoryNumber = i; 
		stats[i].Made = 0;
		stats[i].Eaten = 0;
		stats[i].Min_Delay = 0; 
		stats[i].Avg_Delay = 0;
		stats[i].Max_Delay = 0;
	}
}
void stats_cleanup(void){
	free(stats);
}
void stats_record_produced(int factory_number){
	stats[factory_number].Made++;
}
void stats_record_consumed(int factory_number, double delay_in_ms){
	if(stats[factory_number].Min_Delay == 0 && stats[factory_number].Max_Delay == 0){
		stats[factory_number].Min_Delay = delay_in_ms;
		stats[factory_number].Max_Delay = delay_in_ms;
	}
	stats[factory_number].Avg_Delay = (stats[factory_number].Avg_Delay *
	stats[factory_number].Eaten + delay_in_ms)/(stats[factory_number].Eaten + 1);
	stats[factory_number].Eaten++;
	if(delay_in_ms > stats[factory_number].Max_Delay){
		stats[factory_number].Max_Delay = delay_in_ms;
	}
	if(delay_in_ms < stats[factory_number].Min_Delay){
		stats[factory_number].Min_Delay = delay_in_ms;
	}
}
void stats_display(void){
	int ff = 1;
	printf("%s\n", "Statistics:");
	printf("%8s%10s%30s%30s%30s%30s\n","Factory#", "#Made", "#Eaten",
	"Min Delay[ms]", "Avg Delay[ms]", "Max Delay[ms]");
	for(int j = 0; j < totalnumber; j++){
		if(stats[j].Made != stats[j].Eaten){
			ff = 0;
		}
		printf("%8d%10d%30d%30f%30f%30f\n",j, stats[j].Made, stats[j].Eaten,
		stats[j].Min_Delay, stats[j].Avg_Delay, stats[j].Max_Delay);
	}
	if(ff == 0){
		printf("%s\n", "ERROR: Mismatch between number made and eaten.");
	}
}

