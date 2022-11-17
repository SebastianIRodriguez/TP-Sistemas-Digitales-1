#include "Petri.h"

// *********************************  Variables de Petri ******************************************
int Ct = 1;
int L = 1;

// Los lugares se simbolizan como Qx
// Las trancisiones se simbolizan como Tx

void petri(int SW1, int SW3, int Ft)
{
    static int Q1 = 1;
    static int Q2;
    static int Q3;
    static int Q4;
    static int Q5;
    static int Q6;
    static int Q7;
    static int Q8;
    static int Q9;

    char TR1 = Q1 && SW1;
    char TR2 = Q2 && !SW1 && !Ft;
    char TR3 = Q3 && SW3 && !Ft;
    char TR4 = Q4 && !SW3 && !Ft;
    char TR5 = Q2 && Ft;
    char TR6 = Q3 && Ft;
    char TR7 = Q4 && Ft;
	char TR8 = Q1 && SW3;
    char TR9 = Q6 && !SW3 && !Ft;
    char TR10 = Q7 && SW1 && !Ft;
    char TR11 = Q8 && !SW1 && !Ft;
    char TR12 = Q5 && SW1 && SW3;
    char TR13 = Q9 && !SW1 && !SW3;
	char TR14 = Q6 && Ft;
    char TR15 = Q7 && Ft;
    char TR16 = Q8 && Ft;


	if(TR1){Q2 = 1; Q1=0;}
    if(TR2){Q3 = 1; Q2=0;}
    if(TR3){Q4 = 1; Q3=0;}
    if(TR4){Q5 = 1; Q4=0;}
    if(TR5){Q1 = 1; Q2=0;}
    if(TR6){Q1 = 1; Q3=0;}
	if(TR7){Q1 = 1; Q4=0;}
    if(TR8){Q6 = 1; Q1=0;}
    if(TR9){Q7 = 1; Q6=0;}
    if(TR10){Q8 = 1; Q7=0;}
    if(TR11){Q5 = 1; Q8=0;}
    if(TR12){Q9 = 1; Q5=0;}
    if(TR13){Q1 = 1; Q9=0;}
    if(TR14){Q1 = 1; Q6=0;}
    if(TR15){Q1 = 1; Q7=0;}
    if(TR16){Q1 = 1; Q8=0;}

	L = Q5;
    Ct = Q2 || Q3 || Q4 || Q6 || Q7 || Q8;
}