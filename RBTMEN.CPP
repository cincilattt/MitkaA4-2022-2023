#include <stdio.h>
#include <conio.h>
#include <dos.h>
//takt OUT P1 0 (0x01)
//smer otaceni OUT P1 1 (0x02)
//otaceni zakladny OUT P1 2 (0x04)
//hl.rameno OUT P1 3 (0x08)
//rameno OUT P1 4 (0x10)
//chapadlo OUT P1 5 (0x20)
//zavora OTACENI IN P3 0 (0x01)
//zavora HL.RAMENO IN P3 1 (0x02)
//zavora RAMENO IN P3 2 (0x04)
//zavora CHAPADLO IN P3 3 (0x08)
#define RYCHLOST 2
#define _PORT0 0x300
int vystup;
int vstup;
int odpocetZ=0;
int odpocetH=0;
int odpocetR=0;
int odpocetC=0;
int start=0;

//=====================================================================================

void krok(void) // jeden krok libovolneho motoru (takt)
{
 vystup|=0x01; // Zapnuti taktu
 outportb(_PORT0,vystup); // Poslani hodnoty na port
 delay(RYCHLOST); // Zpozdeni (delka trvani impulsu - rozhoduje o rychlosti otaceni motoru)
 vystup&=~(0x01); // Vypnuti taktu
 outportb(_PORT0,vystup); // Vystup na port
 delay(RYCHLOST); // Druha cast zpozdeni
}

//=====================================================================================

//=====================================================================================

void zakladna(int smer,int pocet_kroku) //vytvoreni funkce zakladna pro vyvolani taktu motoru (pro aktivaci motoru) pokud neni aktivni zavora pro motor zakladny
{
 int i;
 if (smer==1) vystup|=0x02; else vystup&=~(0x02); // Pokud smer=1, bit pro smer nastavime, jinak ho shodime
 vystup|=0x3C; // Na vsechny motory jsme nastavili logickou 1 (Aktivni jsou pri log.0) (0x003c = 0b00111100) kdyby byl nejaky z motoru predtim zaply
 vystup&=~(0x04); // Aktivace 3. bitu (motor pro otaceni zakladny)
 for (i=0;i<pocet_kroku;i++)
 {
	krok(); // Pokud neni aktivni zavora, volani funkce taktu (otaceni motoru)
 }
}

//=====================================================================================

void hl_rameno(int smer,int pocet_kroku) //Vytvoreni funkce hlavni rameno pro aktivaci motoru pokud neni aktivni zavora pro hlavni rameno
{
 int i;
 if (smer==1) vystup|=0x02; else vystup&=~(0x02);
 vystup|=0x3C;
 vystup&=~(0x08);
 for (i=0;i<pocet_kroku;i++)
 {
	krok();
 }
}

//=====================================================================================

void rameno(int smer,int pocet_kroku) //Vytvoreni funkce vedlejsi rameno pro aktivaci motoru pokud neni aktivni zavora pro vedlejsi rameno
{
 int i;
 if (smer==1) vystup|=0x02; else vystup&=~(0x02);
 vystup|=0x3C;
 vystup&=~(0x10);
 for (i=0;i<pocet_kroku;i++)
 {
	krok();
 }
}

//=====================================================================================

void chapadlo(int smer,int pocet_kroku) //Vytvoreni funkce chapadlo pro aktivaci motoru pokud neni aktivni zavora pro chapadlo
{
 int i;
 if (smer==1) vystup|=0x02; else vystup&=~(0x02);
 vystup|=0x3C;
 vystup&=~(0x20);
 for (i=0;i<pocet_kroku;i++)
 {
	krok();
 }
}

//=====================================================================================

void vychozi(void)
{
 vstup=inportb(_PORT0);

 while ((vstup & 0x01)==0x01)
 {
	vstup=inportb(_PORT0);
	zakladna(0,1);
 }

 while ((vstup & 0x02)==0x02)
 {
	vstup=inportb(_PORT0);
	hl_rameno(0,1);
 }

 while ((vstup & 0x04)==0x04)
 {
	vstup=inportb(_PORT0);
	rameno(1,1);
 }

 while ((vstup & 0x08)==0x08)
 {
	vstup=inportb(_PORT0);
	chapadlo(0,1);
 }
}

//=====================================================================================

void main(void) //Vytvoreni prazdne funkce void
{
 if (start==0)
 {
  vychozi();
  start=1;
 }

 char klavesa;
 while(klavesa!='x') //Klavesou X prerusime ovladani
 {
  klavesa=getch(); //Zjisteni stisknuteho tlacitka (kontrola tlacitka)
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if (klavesa=='d')
  {
   if (odpocetZ<2400)
   {
    clrscr();                       
    printf("Zakladna doprava\r\n"); //pokud stisknute tlacitko == a, zakladna se toci doleva
    zakladna(1,1); //nastaveni hodnot do funkce zakladna (smer a pocet taktu) spolu s jejim vyvolanim
    odpocetZ++;
    printf("Z: %d / 2400\nHR: %d / 1400\nRC: %d / 3800\nCh: %d / 550\n",odpocetZ,odpocetH,odpocetR,odpocetC);
   }
   else
   {
    clrscr();
    printf("SW ZAVORA ZAKLADNY AKTIVNI!\nZ: %d / 2400\nHR: %d / 1400\nRC: %d / 3800\nCh: %d / 550\n",odpocetZ,odpocetH,odpocetR,odpocetC); //vypis polohy motoru a stavu zavory
   }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if (klavesa=='a')
  {
   vstup=inportb(_PORT0);
   if ((vstup & 0x01)==0)
   {
    clrscr();
    odpocetZ=0;
    printf("IR ZAVORA ZAKLADNY AKTIVNI!\nZ: %d / 2400\nHR: %d / 1400\nRC: %d / 3800\nCh: %d / 550\n",odpocetZ,odpocetH,odpocetR,odpocetC);
   }
   else
   {
    clrscr();
    printf("Zakladna doleva\r\n");
    zakladna(0,1);
    odpocetZ--;
    printf("Z: %d / 2400\nHR: %d / 1400\nRC: %d / 3800\nCh: %d / 550\n",odpocetZ,odpocetH,odpocetR,odpocetC);
   }
  }
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if (klavesa=='s')
  {
   if(odpocetH<1400)
   {
    clrscr();
    printf("Hlavni rameno dolu\r\n");
    hl_rameno(1,1);
    odpocetH++;
    printf("Z: %d / 2400\nHR: %d / 1400\nRC: %d / 3800\nCh: %d / 550\n",odpocetZ,odpocetH,odpocetR,odpocetC);
   }
   else
   {
    clrscr();
    printf("SW ZAVORA HL.RAMENE AKTIVNI!\nZ: %d / 2400\nHR: %d / 1400\nRC: %d / 3800\nCh: %d / 550\n",odpocetZ,odpocetH,odpocetR,odpocetC);
   }
  }
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if (klavesa=='w')
  {
   vstup=inportb(_PORT0);
   if ((vstup & 0x02)==0)
   {
    clrscr();
    odpocetH=0;
    printf("IR ZAVORA HLAVNIHO RAMENE AKTIVNI!\nZ: %d / 2400\nHR: %d / 1400\nRC: %d / 3800\nCh: %d / 550\n",odpocetZ,odpocetH,odpocetR,odpocetC);
   }
   else
   {
    clrscr();
    printf("Hlavni rameno nahoru\r\n");
    hl_rameno(0,1);
    odpocetH--;
    printf("Z: %d / 2400\nHR: %d / 1400\nRC: %d / 3800\nCh: %d / 550\n",odpocetZ,odpocetH,odpocetR,odpocetC);
   }
  }
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if (klavesa=='i')
  {
   vstup=inportb(_PORT0);
   if ((vstup & 0x04)==0)
   {
    clrscr();
    odpocetR=0;
    printf("IR ZAVORA RAMENE CHAPADLA AKTIVNI!\nZ: %d / 2400\nHR: %d / 1400\nRC: %d / 3800\nCh: %d / 550\n",odpocetZ,odpocetH,odpocetR,odpocetC);
   }
   else
   {
    clrscr();
    printf("Vedlejsi rameno nahoru\r\n");
    rameno(1,1);
    odpocetR--;
    printf("Z: %d / 2400\nHR: %d / 1400\nRC: %d / 3800\nCh: %d / 550\n",odpocetZ,odpocetH,odpocetR,odpocetC);
   }
  }
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if (klavesa=='k')
  {
   if(odpocetR<3800)
   {
    clrscr();
    printf("Rameno chapadla dolu\r\n");
    rameno(0,1);
    odpocetR++;
    printf("Z: %d / 2400\nHR: %d / 1400\nRC: %d / 3800\nCh: %d / 550\n",odpocetZ,odpocetH,odpocetR,odpocetC);
   }
   else
   {
    clrscr();
    printf("SW ZAVORA RAMENE CHAPADLA AKTIVNI\NZ: %d / 2400\nHR: %d / 1400\nRC: %d / 3800\nCh: %d / 550\n",odpocetZ,odpocetH,odpocetR,odpocetC);
   }
  }
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if (klavesa=='j')
  {
   vstup=inportb(_PORT0);
   if ((vstup & 0x08)==0)
   {
		clrscr();
		odpocetC=0;
		printf("IR ZAVORA CHAPADLA AKTIVNI!\nZ: %d / 2400\nHR: %d / 1400\nRC: %d / 3800\nCh: %d / 550\n",odpocetZ,odpocetH,odpocetR,odpocetC);
	 }
	 else
	 {
		clrscr();
		printf("Chapadlo otevrit\r\n");
		chapadlo(0,1);
		odpocetC--;
		printf("Z: %d / 2400\nHR: %d / 1400\nRC: %d / 3800\nCh: %d / 550\n",odpocetZ,odpocetH,odpocetR,odpocetC);
	 }
	}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if(klavesa=='l')
	{
   if(odpocetC<550)
   {
    clrscr();
    printf("Chapadlo zavrit\r\n");
    chapadlo(1,1);
    odpocetC++;
    printf("Z: %d / 2400\nHR: %d / 1400\nRC: %d / 3800\nCh: %d / 550\n",odpocetZ,odpocetH,odpocetR,odpocetC);
   }
   else
   {
    clrscr();
    printf("SW ZAVORA CHAPADLA AKTIVNI!Z: %d / 2400\nHR: %d / 1400\nRC: %d / 3800\nCh: %d / 550\n",odpocetZ,odpocetH,odpocetR,odpocetC);
   }
  }
 }
}