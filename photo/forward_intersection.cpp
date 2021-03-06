#include "forward_intersection.h"

#include <cmath>
#include "..//basics//matrix//matrix.h"
#include "..//basics//rotation_matrix.h"
//#include "..//basics//rot_matrix.h"//old!
#include "..//basics//straight_line.h"



//###################################################################################
double Vorwaertsschnitt::Zaehler_x(Cam K, Point OP_0, Matrix& R)
{
	double Z;
  	Z =   R(0,0) * (OP_0.get_X() - K.get_OX())
		+ R(1,0) * (OP_0.get_Y() - K.get_OY())
		+ R(2,0) * (OP_0.get_Z() - K.get_OZ());
	return Z;
}

double Vorwaertsschnitt::Zaehler_y(Cam K, Point OP_0, Matrix& R)
{
	double Z;
	Z =   R(0,1) * (OP_0.get_X() - K.get_OX())
		+ R(1,1) * (OP_0.get_Y() - K.get_OY())
		+ R(2,1) * (OP_0.get_Z() - K.get_OZ());
	return Z;
}

double Vorwaertsschnitt::Nenner(Cam K, Point OP_0, Matrix& R)
{
	double N;
	N =   R(0,2) * (OP_0.get_X() - K.get_OX())
		+ R(1,2) * (OP_0.get_Y() - K.get_OY())
		+ R(2,2) * (OP_0.get_Z() - K.get_OZ());
	return N;
}

double Vorwaertsschnitt::dxdX(Cam K, double Zx, double N, Matrix& R)
{
	double A;
    A = (K.get_c() * (R(0,2) * Zx - R(0,0) * N)) / (N * N);
	return A;
}

double Vorwaertsschnitt::dxdY(Cam K, double Zx, double N, Matrix& R)
{
	double A;
	A = (K.get_c() * (R(1,2) * Zx - R(1,0) * N)) / (N * N);
	return A;
}

double Vorwaertsschnitt::dxdZ(Cam K, double Zx, double N, Matrix& R)
{
	double A;
	A = (K.get_c() * (R(2,2) * Zx - R(2,0) * N)) / (N * N);
	return A;
}

double Vorwaertsschnitt::dydX(Cam K, double Zy, double N, Matrix& R)
{
	double A;
	A = (K.get_c() * (R(0,2) * Zy - R(0,1) * N)) / (N * N);
	return A;
}

double Vorwaertsschnitt::dydY(Cam K, double Zy, double N, Matrix& R)
{
	double A;
	A = (K.get_c() * (R(1,2) * Zy - R(1,1) * N)) / (N * N);
	return A;
}

double Vorwaertsschnitt::dydZ(Cam K, double Zy, double N, Matrix& R)
{
	double A;
	A = (K.get_c() * (R(2,2) * Zy - R(2,1) * N)) / (N * N);
	return A;
}

double Vorwaertsschnitt::lx(Cam K, Point OP_0, double BP_x, Matrix& R)
{
	double l;
	double BP_x0;
	BP_x0 =  K.get_c() * (Zaehler_x(K,OP_0,R) / Nenner(K, OP_0,R)); // ohne xh + dx
	l = (-1.0)*(BP_x - BP_x0);
	return l;
}

double Vorwaertsschnitt::ly(Cam K, Point OP_0, double BP_y, Matrix& R)
{
	double l;
	double BP_y0;
	BP_y0 =  K.get_c() * (Zaehler_y(K,OP_0,R) / Nenner(K, OP_0,R)); // ohne yh + dy
	l = (-1.0)*(BP_y - BP_y0);
	return l;
}

double Vorwaertsschnitt::x(Cam K, Point OP_0, Matrix& R)
{
	double BP_x0;
	BP_x0 =  K.get_c() * (Zaehler_x(K,OP_0,R) / Nenner(K, OP_0,R)); // ohne xh + dx
	return BP_x0;
}

double Vorwaertsschnitt::y(Cam K, Point OP_0, Matrix& R)
{
	double BP_y0;
	BP_y0 =  K.get_c() * (Zaehler_y(K,OP_0,R) / Nenner(K, OP_0,R)); // ohne yh + dy
	return BP_y0;
}


//###################################################################################

Vorwaertsschnitt::Vorwaertsschnitt(vector<BPoint> &BP)//,Point Org
{
    m_BP = &BP;
    calc();
}

bool Vorwaertsschnitt::set_new_bpoint_list(vector<BPoint> &BP)
{
    m_BP = &BP;
    calc();

    return get_is_error();
}

void Vorwaertsschnitt::calc()
{
    if(m_BP->size()>1)
    {
     m_is_error=false;

    //###############################################
    //Nherungsberechnung des Schnittpunktes
    //2 Geraden die sich schneiden (sollten)

    //old
    //BPoint BP1 = BP[0];
    //BPoint BP2 = BP[1];

    //new ->problem when change the orientation center the Point into the BP1 is not acutely
    //BPoint BP1(m_BP->begin()->get_Cam(),m_BP->begin()->get_m(),m_BP->begin()->get_n());
    //BPoint BP2((++m_BP->begin())->get_Cam(),(++m_BP->begin())->get_m(),(++m_BP->begin())->get_n());

    //update BPoint class outside //steffen 20101013
    BPoint BP1 ( *(  m_BP->begin()) );
    BPoint BP2 ( *(++m_BP->begin()) );


    //cout <<endl<<"m: "<< BP1.get_m()<<" n: "<<BP1.get_n()<< flush;
    //cout <<endl<<"m: "<< BP2.get_m()<<" n: "<<BP2.get_n()<< flush;

    Gerade G_l(BP1.get_Point(),BP1.get_Cam().get_O());
    Gerade G_r(BP2.get_Point(),BP2.get_Cam().get_O());

    m_schnittpunkt=G_l.Schnitt(G_r);

    //cout <<endl<<"schnittp: "<< m_schnittpunkt;
    //###############################################
    //Vorwrtsschnitt

    Point OP_0;
    OP_0=m_schnittpunkt; // Zuweisen des Nherungswertes

    //Anzahl der verfgbaren Bildpunkte:
    int anzahl = static_cast<int>( m_BP->size() );

    /*
    //###############################################
    // Kontrollrechnung
      double x_st_l,y_st_l,x_st_r,y_st_r,x_korr_l,x_unkorr_l,y_korr_l,y_unkorr_l,x_korr_r,x_unkorr_r,y_korr_r,y_unkorr_r;
      double dx_l,dy_l,dx_r,dy_r;

       x_st_l = x(K1,OP_0,R1);
       x_korr_l=BP1.get_xyBKooKorr().get_X();
       x_unkorr_l=BP1.get_xyBKoo().get_X();
       dx_l=x_st_l-x_korr_l;

       y_st_l = y(K1,OP_0,R1);
       y_korr_l=BP1.get_xyBKooKorr().get_Y();
       y_unkorr_l=BP1.get_xyBKoo().get_Y();
       dy_l=y_st_l-y_korr_l;

       x_st_r = x(K2,OP_0,R2);
       x_korr_r=BP2.get_xyBKooKorr().get_X();
       x_unkorr_r=BP2.get_xyBKoo().get_X();
       dx_r=x_st_r-x_korr_r;

       y_st_r = y(K2,OP_0,R2);
       y_korr_r=BP2.get_xyBKooKorr().get_Y();
       y_unkorr_r=BP2.get_xyBKoo().get_Y();
       dy_r=y_st_r-y_korr_r;
    //#################################################
   */

    //Punkt OP -> Neu berechneter(ausgeglichener) ObjektPunkt;
    Point OP;

    int i=0,k,j; //Laufvariable fr Abbruch
    Point dOP(100,100,100); //Differenz zwischen dem NherungsObjektpunkt und neu berechnetem Objektpunkt

    while(!( fabs(dOP.get_X())<0.00000001 && fabs(dOP.get_Y())<0.00000001 && fabs(dOP.get_Z())<0.00000001 ) )
    {
       Matrix A(anzahl*2,3,Null);
       Matrix l(anzahl*2,1,Null);
       for(k=0,j=0;k<anzahl*2;k=k+2,j++)
       {
           BPoint BB  = m_BP->operator [](j);//[j];
           Cam KK     = BB.get_Cam();
           Rotation_matrix R_hilf(Rotation_matrix::math,KK.get_rotX(),KK.get_rotY(),KK.get_rotZ());
           Matrix RR;
           RR = R_hilf;//.get_Matrix();
           A(k  ,0) = dxdX(KK, Zaehler_x(KK,OP_0,RR), Nenner(KK,OP_0,RR), RR);
           A(k  ,1) = dxdY(KK, Zaehler_x(KK,OP_0,RR), Nenner(KK,OP_0,RR), RR);
           A(k  ,2) = dxdZ(KK, Zaehler_x(KK,OP_0,RR), Nenner(KK,OP_0,RR), RR);

           A(k+1,0) = dydX(KK, Zaehler_y(KK,OP_0,RR), Nenner(KK,OP_0,RR), RR);
           A(k+1,1) = dydY(KK, Zaehler_y(KK,OP_0,RR), Nenner(KK,OP_0,RR), RR);
           A(k+1,2) = dydZ(KK, Zaehler_y(KK,OP_0,RR), Nenner(KK,OP_0,RR), RR);

           l(k  ,0) = lx(KK,OP_0,BB.get_xyBKooKorr().get_X(),RR);
           l(k+1,0) = ly(KK,OP_0,BB.get_xyBKooKorr().get_Y(),RR);
       }

       //A.MatShow();
       //l.MatShow();

       Matrix N(3,3,Null);
       N = A.MatTrans().MatMult(A);
       //N.MatShow();

       Matrix Q(3,3,Null);
       Q = N.MatInvert();
       //Q.MatShow();

       Matrix tx;
       tx=A.MatTrans().MatMult(l);

       Matrix x;//(3,1,Null);
       x = Q.MatMult( tx );
       //x.MatShow();


       // Anbringen der "dX Verbesserung" x an den Nherungsobjektpunkt
       OP.set_X( OP_0.get_X() + x(0,0));
       OP.set_Y( OP_0.get_Y() + x(1,0));
       OP.set_Z( OP_0.get_Z() + x(2,0));

       //setzen der neuen Nherung und der Differenz zwischen altem und neuen Objektpunkt
       dOP=OP_0.Subt(OP);
       //Point dS3=Org.Subt(OP);//Kontrolle
       OP_0=OP;

       // Berechnung der Verbesserungen
       Matrix v(anzahl*2,1,Null);
       v = A.MatMult(x).MatSub( l );
       //v.MatShow();

       //Berechnung der Standardabweichung a posteriori
       Matrix vv;
       vv=v.MatTrans().MatMult(v);
       double s0 = sqrt(vv(0,0)/(anzahl*2-3));

       //Berechnung der Standardabweichung der ausgeglichenen Objektkoordinaten
       double sx, sy, sz;
       sx = s0 * sqrt( Q(0,0) ) *3;
       sy = s0 * sqrt( Q(1,1) ) *3;
       sz = s0 * sqrt( Q(2,2) ) *3;

       //setzen der Standardabweichnug
       OP.set_dX( sx );
       OP.set_dY( sy );
       OP.set_dZ( sz );

        if(i>1000)
        {
            m_is_error=true;
            cerr << endl << "(VWS) ERROR more than 1000 iterations !!!\n" <<flush;
            break;
        }

        ++i;
    }//Ende der while-Schleife

    m_schnittpunkt=OP;

    }
    else
    {m_is_error=true;
    }

}
