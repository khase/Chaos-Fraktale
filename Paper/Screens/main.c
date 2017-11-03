#include <REG515C.H> 

// Laboraufgabe 2
// Tobias Wenzig, Marteyn Weidenbach, Artur Schmidt

// Eingaenge 
sbit wunsch1_i = P1^1; 
sbit wunsch2_i = P1^2; 
sbit in2_i = P1^7; 
bit wunsch1; 
bit wunsch2; 
bit in2; 

// Ausgaenge 
sbit ab_o = P5^1; 
sbit auf_o = P5^2; 
bit ab; 
bit auf; 

// functionen 
static void readInputVars(); 
static void computeOutputVars(); 
static void writeOutputVars(); 

typedef enum {keinW, wOben, wUnten} zustaende_t; 
zustaende_t zustand = keinW; 

void main(){ 
	while(1){ // Endlosschleife 
		// Eingabephase 
		readInputVars(); 
		// Berechnungsphase 
		computeOutputVars(); 
		// Ausgabephase 
		writeOutputVars(); 
	} 
} 

static void readInputVars(){ 
	wunsch1 = wunsch1_i; 
	wunsch2 = wunsch2_i; 
	in2 = in2_i; 
	return; 
} 

static void computeOutputVars(){ 
	// Zustandsuebergangsfunktion 
	switch(zustand) { 
		case keinW: 
			if(wunsch2 && !in2){ 
				zustand = wOben; 
			} 
			if(wunsch1 && in2){ 
				zustand = wUnten; 
			} 
			break; 
		case wOben: 
			if(in2) 
			{ 
			    if(!wunsch1){ 
				zustand = keinW; 
			    } 
			    else{ 
				zustand = wUnten; 
				} 
			} 
			break; 
		case wUnten: 
			if(!in2) 
			{ 
			    if(!wunsch2){ 
				zustand = keinW; 
			    else{ 
				zustand = wOben; 
			    } 
			} 
			break; 
	} 
	// Ausgabefunktion 
	switch(zustand){ 
		case keinW: 
			ab = 0; 
			auf = 0; 
			break; 
		case wOben: 
			ab = 0; 
			auf = 1; 
			break; 
		case wUnten: 
			ab = 1; 
			auf = 0; 
			break; 
	} 
	return; 
} 

static void writeOutputVars(){ 
	ab_o = !ab; 
	auf_o = !auf; 
	return; 
}