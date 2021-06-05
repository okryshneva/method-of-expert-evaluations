//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "expert_evaluation.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;

//in stringgrid cells[cols][rows]

// max rows and cols in stringgrids
const int MAXROW = 8;   // max indicators +1
const int MAXCOL = 10;  // max experts + 1
const int MAXAN = 5;    // max analogues +1

//global variables
float fi, fi0, sigma, v, z;

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
        : TForm(Owner)
{
        int i,j,l;

        // source stringgrids

        // stringgrid of expert evaluations
        for ( i = 1; i < MAXROW; i++) {
                StringGrid1->Cells[0][i] = "w.ind-tor " + IntToStr(i);
        }

        for ( j = 1; j < MAXCOL; j++) {
               StringGrid1->Cells[j][0] = "Expert " + IntToStr(j);
        }

        // stringgrid of analogues
        for ( l = 1; l < MAXAN; l++) {
               StringGrid2->Cells[l][0] = "GBU-" + IntToStr(l-1);
        }
        StringGrid2->Cells[0][1] = "Fc";    //F-combat
        StringGrid2->Cells[0][2] = "Ft";    //F-tactic
        StringGrid2->Cells[0][3] = "Fr";    //F-reliability
        StringGrid2->Cells[0][4] = "Fs";    //F-survival
        StringGrid2->Cells[0][5] = "Fo";    //F-operational
        StringGrid2->Cells[0][6] = "Ftech"; //F-technical
        StringGrid2->Cells[0][7] = "Fe";    //F-economically

        StringGrid3->Cells[0][0] = "Fi ( i )" ;
        StringGrid3->Cells[1][0] = "Fi0 ( i )" ;
        StringGrid3->Cells[2][0] = "G ( i )" ;
        StringGrid3->Cells[3][0] = "V ( i )" ;
        StringGrid3->Cells[4][0] = "Êïi ( i )" ;


        UpDownRow->Position = StringGrid1->RowCount; // cnange quantity
        UpDownAn->Position  = StringGrid2->RowCount;  // rows in stringgrids
        UpDownCol->Position = StringGrid1->ColCount; // rows in stringgrid1
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::UpDownRowClick(TObject *Sender, TUDBtnType Button)
{
        // count of indicators

        if (UpDownRow->Position < 3)
                UpDownRow->Position = 3;
        if (UpDownRow->Position > MAXROW)
                UpDownRow->Position = MAXROW;
        StringGrid1->RowCount = UpDownRow->Position;
        StringGrid2->RowCount = UpDownRow->Position;
        PokCount->Caption = IntToStr(UpDownRow->Position-1);

}
//---------------------------------------------------------------------------
void __fastcall TMainForm::UpDownColClick(TObject *Sender, TUDBtnType Button)
{
        // count of experts

        if (UpDownCol->Position < 3)
                UpDownCol->Position = 3;
        if (UpDownCol->Position > MAXCOL)
                UpDownCol->Position = MAXCOL;
        StringGrid1->ColCount = UpDownCol->Position;
        ExpCount->Caption = IntToStr(UpDownCol->Position-1);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::UpDownAnClick(TObject *Sender,
      TUDBtnType Button)
{
        // count of analogues

        if (UpDownAn->Position < 4)
                UpDownAn->Position = 4;
        if (UpDownAn->Position > MAXAN)
                UpDownAn->Position = MAXAN;
        StringGrid2->ColCount = UpDownAn->Position;
        AnCount->Caption = IntToStr(UpDownAn->Position-1);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CountButtonClick(TObject *Sender)
{

        int i,j;
        bool flag;
        float exp,par;
        float phi,summphi;

        /*//////////////////////////////////////////////////////////////////////
                        CLEAR STRINGGRID3 (final calculation)
        //////////////////////////////////////////////////////////////////////*/
        for (int i=0; i < MAXAN; i++) {
             StringGrid3->Cols[i]->Clear();
        }

        //stringgrid expert evaluations
       for (int i = 1; i < MAXROW; i++) {
                StringGrid1->Cells[0][i] = "w.ind-tor " + IntToStr(i);
        }
        for (int j = 1; j < MAXCOL; j++) {
               StringGrid1->Cells[j][0] = "Expert " + IntToStr(j);
        }

        StringGrid3->Cells[0][0] = "Fi ( i )" ;
        StringGrid3->Cells[1][0] = "Fi0 ( i )" ;
        StringGrid3->Cells[2][0] = "G ( i )" ;
        StringGrid3->Cells[3][0] = "V ( i )" ;
        StringGrid3->Cells[4][0] = "Êïi ( i )" ;

        exp = StrToFloat(UpDownCol->Position-1);
        par = StrToFloat(UpDownRow->Position-1);

        /*//////////////////////////////////////////////////////////////////////
                        checking the fullness of the table
        //////////////////////////////////////////////////////////////////////*/
        for (j = 1; j < UpDownRow->Position; j++) {
          for (i = 1; i < UpDownCol->Position; i++) {
           if (StringGrid1->Cells[i][j] == "")
             flag = 1;
           else
             flag = 0;
          }
        }

        if (flag == 1)
            MessageBox(0,"Fill all empty values","ERROR",MB_OK);
        else {

        // STRINGGRID->CELLS[ ¹ expert ][ ¹ indicator ]

        //dynamic array for fi
        float *fi = new float [par+1];

        //dynamic array for fi0
        float *fi0 = new float [par+1];

        //dynamic array for gi
        float *gi = new float [par+1];

        //dynamic array for V
        float *vi = new float [par+1];

        //dynamic array for Kbi
        float *kbi = new float [par+1];

        //dynamic array for Kpi
        float *kpi = new float [par+1];


        //calculation for phi and summ phi
        for ( i = 1; i <= par; i++ ) {
          phi = 0;
          for ( j = 1; j <= exp; j++ ) {
              if (StringGrid1->Cells[j][i] != "\0")
                  phi = phi + StrToFloat(StringGrid1->Cells[j][i]);
                  fi[i] = (phi*0.1)/exp;
          }
          summphi = summphi + fi[i];
          StringGrid3->Cells[0][i] = FloatToStrF(fi[i],ffFixed,9,3);
        }

        //calcultaion fi0
        for ( i = 1; i <= par; i++){
                fi0[i] = fi[i]/summphi;
                StringGrid3->Cells[1][i] = FloatToStrF(fi0[i],ffFixed,9,3);
        }

        //calculation sigma
        for ( i = 1; i <= par; i++){
        phi = 0;
          for ( j = 1; j <= exp; j++){
           phi = phi + pow(StrToFloat(StringGrid1->Cells[j][i])/10-fi[i],2);
          }
        gi[i] = sqrt(phi/exp);
        StringGrid3->Cells[2][i] = FloatToStrF(gi[i],ffFixed,9,3);
        }

        //Âû÷èñëåíèå âý
        for ( i = 1; i <= par; i++){
                vi[i] = (gi[i]/fi[i])*100;
                StringGrid3->Cells[3][i] = FloatToStrF(vi[i],ffFixed,9,3);
        }

        //determining the relative values of indicators

        float SRAV,an,a;

        //the number of analogs to compare (without the compared variant)

        an = StrToFloat(UpDownAn->Position-2);
        for ( i = 1; i <= par; i++ ) {
          a = 1.0;
          for ( j = 2; j < UpDownAn->Position; j++ ) {
              if (StringGrid2->Cells[j][i] != "")
                  a = a * StrToFloat(StringGrid2->Cells[j][i]);
          }
          kbi[i] = pow(a,(1/an));
        }

        float summkpi;
        for ( i = 1; i <= par; i++){
          if ((i == 7)||(i == 1)) {
            if (StringGrid2->Cells[1][i] != "")
              kpi[i] = kbi[i]/StrToFloat(StringGrid2->Cells[1][i]);
          }
           else {
            if (StringGrid2->Cells[1][i] != "")
              phi = StrToFloat(StringGrid2->Cells[1][i]);
              kpi[i] = phi/kbi[i];
            }
          summkpi = summkpi + kpi[i];
        }

        for ( i = 1; i <= par; i++){
          StringGrid3->Cells[4][i] = FloatToStrF(kpi[i],ffFixed,9,3);
        }

        float fki,b,s;
        fki = 0;
        for ( i = 1; i <= par; i++){
          if (StringGrid3->Cells[0][i] != "")
            a = StrToFloat(StringGrid3->Cells[0][i]);
          if (StringGrid3->Cells[4][i] != "")
            b = StrToFloat(StringGrid3->Cells[4][i]);
          fki = fki + a*b;
        }

        float ktu;
        ktu = fki/summphi;

        summfi->Caption = "fi = " + FloatToStrF(summphi,ffFixed,9,3);
        fik->Caption = "sum = " + FloatToStrF(fki,ffFixed,9,3);
        Kty->Caption = "Kts = " + FloatToStrF(ktu,ffFixed,9,3);

        if ( ktu > 1)
          Panel1->Caption = "The developed GBU is better than analogues";
        else
          Panel1->Caption = "The developed GBU is worse than analogues";

        // delete dynamic massives
        delete [] fi;
        delete [] fi0;
        delete [] gi;
        delete [] vi;
        delete [] kpi;
        delete [] kbi;

        }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Button1Click(TObject *Sender)
{
        fik->Caption = "Amount fi*Kpi";
        Kty->Caption = "Kts";
        summfi->Caption = "Amount fi";

        for (int i=1; i < MAXCOL; i++) {
             StringGrid1->Cols[i]->Clear();
        }
        for (int i=0; i < MAXROW; i++) {
             StringGrid2->Cols[i]->Clear();
        }
        for (int i=0; i < MAXAN; i++) {
             StringGrid3->Cols[i]->Clear();
        }

        //table of expert's evaluations
       for (int i = 1; i < MAXROW; i++) {
                StringGrid1->Cells[0][i] = "w.ind-tor " + IntToStr(i);
        }
        for (int j = 1; j < MAXCOL; j++) {
               StringGrid1->Cells[j][0] = "Expert " + IntToStr(j);
        }

        //table of analogues
        for (int l = 1; l < MAXAN; l++) {
               StringGrid2->Cells[l][0] = "GBU-" + IntToStr(l-1);
        }
        StringGrid2->Cells[0][1] = "Fc";    //F-combat
        StringGrid2->Cells[0][2] = "Ft";    //F-tactic
        StringGrid2->Cells[0][3] = "Fr";    //F-reliability
        StringGrid2->Cells[0][4] = "Fs";    //F-survival
        StringGrid2->Cells[0][5] = "Fo";    //F-operational
        StringGrid2->Cells[0][6] = "Ftech"; //F-technical
        StringGrid2->Cells[0][7] = "Fe";    //F-economically

        StringGrid3->Cells[0][0] = "Fi ( i )" ;
        StringGrid3->Cells[1][0] = "Fi0 ( i )" ;
        StringGrid3->Cells[2][0] = "G ( i )" ;
        StringGrid3->Cells[3][0] = "V ( i )" ;
        StringGrid3->Cells[4][0] = "Êïi ( i )" ;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::Button2Click(TObject *Sender)
{
        //random expert's evaluations
        int i,j;
        for (j = 1; j < UpDownRow->Position; j++) {
          for (i = 1; i < UpDownCol->Position; i++) {
           StringGrid1->Cells[i][j] = rand() % 10 +1;
          }
        }

        for (j = 1; j < MAXAN; j++) {
          for (i = 1; i < MAXROW; i++) {
           StringGrid2->Cells[j][i] = rand() % 10 +1;
          }
        }

}
//---------------------------------------------------------------------------



void __fastcall TMainForm::primerClick(TObject *Sender)
{
        // example

        UpDownRow->Position = 6;
        StringGrid1->RowCount = UpDownRow->Position;
        StringGrid2->RowCount = UpDownRow->Position;
        PokCount->Caption = IntToStr(UpDownRow->Position)-1;

        UpDownCol->Position = 7;
        StringGrid1->ColCount = UpDownCol->Position;
        ExpCount->Caption = IntToStr(UpDownCol->Position)-1;

        UpDownAn->Position = 4;
        StringGrid2->ColCount = UpDownAn->Position;
        AnCount->Caption = IntToStr(UpDownAn->Position)-1;


        // emample expert evaluations
        StringGrid1->Cells[1][1] = 9.5;
        StringGrid1->Cells[2][1] = 8.5;
        StringGrid1->Cells[3][1] = 9;
        StringGrid1->Cells[4][1] = 10;
        StringGrid1->Cells[5][1] = 9;
        StringGrid1->Cells[6][1] = 10;

        StringGrid1->Cells[1][2] = 8;
        StringGrid1->Cells[2][2] = 8.5;
        StringGrid1->Cells[3][2] = 6;
        StringGrid1->Cells[4][2] = 10;
        StringGrid1->Cells[5][2] = 8;
        StringGrid1->Cells[6][2] = 8.5;

        StringGrid1->Cells[1][3] = 8;
        StringGrid1->Cells[2][3] = 7;
        StringGrid1->Cells[3][3] = 8.5;
        StringGrid1->Cells[4][3] = 8;
        StringGrid1->Cells[5][3] = 7;
        StringGrid1->Cells[6][3] = 8;

        StringGrid1->Cells[1][4] = 6;
        StringGrid1->Cells[2][4] = 5;
        StringGrid1->Cells[3][4] = 8;
        StringGrid1->Cells[4][4] = 6;
        StringGrid1->Cells[5][4] = 6;
        StringGrid1->Cells[6][4] = 4;

        StringGrid1->Cells[1][5] = 5;
        StringGrid1->Cells[2][5] = 6;
        StringGrid1->Cells[3][5] = 4;
        StringGrid1->Cells[4][5] = 6;
        StringGrid1->Cells[5][5] = 6;
        StringGrid1->Cells[6][5] = 4;

        StringGrid1->Cells[1][6] = 6;
        StringGrid1->Cells[2][6] = 4;
        StringGrid1->Cells[3][6] = 6;
        StringGrid1->Cells[4][6] = 10;
        StringGrid1->Cells[5][6] = 9;
        StringGrid1->Cells[6][6] = 8;

        StringGrid1->Cells[1][7] = 5;
        StringGrid1->Cells[2][7] = 5;
        StringGrid1->Cells[3][7] = 4;
        StringGrid1->Cells[4][7] = 10;
        StringGrid1->Cells[5][7] = 8;
        StringGrid1->Cells[6][7] = 8;

        // example relative values
        StringGrid2->Cells[1][1] = 3.5;
        StringGrid2->Cells[2][1] = 5;
        StringGrid2->Cells[3][1] = 5;

        StringGrid2->Cells[1][2] = 380;
        StringGrid2->Cells[2][2] = 355;
        StringGrid2->Cells[3][2] = 453;

        StringGrid2->Cells[1][3] = 0.62;
        StringGrid2->Cells[2][3] = 0.62;
        StringGrid2->Cells[3][3] = 0.62;

        StringGrid2->Cells[1][4] = 0.5;
        StringGrid2->Cells[2][4] = 0.5;
        StringGrid2->Cells[3][4] = 0.5;

        StringGrid2->Cells[1][5] = 25;
        StringGrid2->Cells[2][5] = 20;
        StringGrid2->Cells[3][5] = 20;



}
//---------------------------------------------------------------------------



















