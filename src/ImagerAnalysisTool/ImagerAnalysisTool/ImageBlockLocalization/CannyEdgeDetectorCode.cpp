// CanyEdgeDetectorCode.cpp: implementation of the CCannyEdgeDetectorCode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "math.h"
#include "CannyEdgeDetectorCode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define filter_width 20
#define u_ unsigned

/*gaussian & derivative filter masks*/
double G[filter_width+1],dG[filter_width+1],D2G[filter_width+1]; 
int filter_real_w=filter_width;  

CCannyEdgeDetectorCode::CCannyEdgeDetectorCode()
{
	deri_mag=NULL;
    mag_data=NULL; 
	orient_data = NULL;
	GxImg=NULL;
	GyImg=NULL;
	high=30; 
	low=15;
}

CCannyEdgeDetectorCode::~CCannyEdgeDetectorCode()
{
	if( deri_mag !=NULL ) delete [] deri_mag;
    if( mag_data != NULL) delete [] mag_data; 
	if( orient_data != NULL) delete [] orient_data;
	if( GxImg !=NULL) delete [] GxImg;
	if( GyImg !=NULL) delete [] GyImg;
}

BOOL CCannyEdgeDetectorCode::EdgeDetection(unsigned char *ByteImage,int  column, int  row, double smoothing,double threshold,double min_threshold)
{     double s=1.0;   
      int size;
	  s=smoothing;
	  if( smoothing == 0.0 ) s=1.0;
      high=(int) threshold;
	  if( high == 0 ) high =30;
	  low = (int) min_threshold;
	  if( low == 0 ) low =10;
	  DoGMask(s);
      size = column * row;
	  if( mag_data != NULL) delete [] mag_data;
      mag_data= new unsigned char[size];
      if( orient_data != NULL ) delete [] orient_data;
      orient_data= new  double[ size];
      if( deri_mag != NULL ) delete [] deri_mag;
	  deri_mag = new  double[ size];
	  if( GxImg != NULL ) delete [] GxImg;
      GxImg= new  double[ size];
	  if( GyImg != NULL ) delete [] GyImg;
      GyImg = new  double[ size];

       canny_core(s, column, row, ByteImage);
       non_maximum_supress(column,row);
       thresholding_tracker(column, row,ByteImage); 
	   return TRUE;
}

void CCannyEdgeDetectorCode::non_maximum_supress(int cols, int rows)
{
    register int cindex,nindex,sindex,windex,eindex,nwindex,neindex,swindex,seindex;
    double g0,gc,gn,gs,gw,ge,gnw,gne,gsw,gse;
    int ilimit,jstart,jlimit,j,i;
    double ux,uy;
    double ORIENT_SCALE=1.0;
    double min=10000.0,max=0.0,rate;

    ilimit=cols-filter_real_w;
    jstart=cols*filter_real_w;
    jlimit=cols*(rows-filter_real_w);

	for(i=0; i < cols * rows; i++ ) {
		deri_mag [i] =0.0; mag_data[i]=0; 
	}

    for (i=filter_real_w;i<ilimit;++i)
        for (j=jstart;j<jlimit;j+=cols)
        {  cindex=i+j;
           ux=GxImg[cindex];
           uy=GyImg[cindex];
           rate=deri_mag[cindex]=sqrt(ux*ux+uy*uy);
           if( rate < min )  min =rate;	 
           if( rate > max ) max =rate;
         }
   
    rate= 255.0 /(max-min);

    for (i=filter_real_w;i<ilimit;++i)
    {  
        for (j=jstart;j<jlimit;j+=cols)
        {
           /* calculate current indeces */
            cindex=i+j;
            nindex=cindex-cols;
            sindex=cindex+cols;
            windex=cindex-1;
            eindex=cindex+1;
            nwindex=nindex-1;
            neindex=nindex+1;
            swindex=sindex-1;
            seindex=sindex+1;
            ux=GxImg[cindex];
            uy=GyImg[cindex];
            gc=sqrt(ux*ux+uy*uy);
            /* scale gc to fit into an unsigned char array */
            deri_mag[cindex]=(gc < 255.0  ? gc : 255.0);
            gn=hypotenuse(GxImg[nindex],GyImg[nindex]);
            gs=hypotenuse(GxImg[sindex],GyImg[sindex]);
            gw=hypotenuse(GxImg[windex],GyImg[windex]);
            ge=hypotenuse(GxImg[eindex],GyImg[eindex]);
            gne=hypotenuse(GxImg[neindex],GyImg[neindex]);
            gse=hypotenuse(GxImg[seindex],GyImg[seindex]);
            gsw=hypotenuse(GxImg[swindex],GyImg[swindex]);
            gnw=hypotenuse(GxImg[nwindex],GyImg[nwindex]);
            if (ux*uy>0)
            {
                if(fabs(ux)<fabs(uy))
                {
                    if((g0=fabs(uy*gc))
                     < fabs(ux*gse+(uy-ux)*gs) ||
                     g0<=fabs(ux*gnw+(uy-ux)*gn))
                    continue;
                }
                else
                {
                    if((g0=fabs(ux*gc))
                     < fabs(uy*gse+(ux-uy)*ge) ||
                     g0<=fabs(uy*gnw+(ux-uy)*gw))
                    continue;
                }
            }
            else
            {
                if(fabs(ux)<fabs(uy))
                {
                    if((g0=fabs(uy*gc))
                     < fabs(ux*gne-(uy+ux)*gn) ||
                     g0<=fabs(ux*gsw-(uy+ux)*gs))
                     continue;
                }
                else
                {
                    if((g0=fabs(ux*gc))
                     < fabs(uy*gne-(ux+uy)*ge) ||
                     g0<=fabs(uy*gsw-(ux+uy)*gw))
                        continue;
                }
            }
            /* seems to be a good scale factor */
           { gc=(gc-min)*rate; mag_data[cindex]=(u_ char) gc;}
            /* pi*40 ~= 128 - direction is (thought
               of as) a signed byte */
            if( ux !=0.0 )
            orient_data[cindex]=atan2(uy, ux)*ORIENT_SCALE;
            else
            orient_data[cindex]=0.0;
        }
    }
 }


int CCannyEdgeDetectorCode::canny_core(double s, int cols, int rows, unsigned char *data)
{ 
  double *gsmooth_x,*gsmooth_y;
  double t1,t2;
  int ilimit,jstart,jlimit,j,i,cindex,k;
  int kfactor1,kfactor2;
  int kfactor;

  gsmooth_x = new double[cols * rows];
  gsmooth_y = new double[cols * rows];
 for( i=0; i< cols* rows; i++)  {  gsmooth_y[i]=0.0; gsmooth_x[i]=0.0;}

 /* produce x- and y- convolutions with gaussian */

    ilimit=cols-(filter_real_w-1);
    jstart=cols*(filter_real_w-1);
    jlimit=cols*(rows-(filter_real_w-1));
    for (i=filter_real_w-1;i<ilimit;++i)
       for(j=jstart;j<jlimit;j+=cols)
        {
            cindex=i+j;

            t2=t1=G[0]*data[cindex];
            for(k=1,
                kfactor1=cindex-cols,
                kfactor2=cindex+cols;
                k<filter_real_w;k++,
                kfactor1-=cols,
                kfactor2+=cols)
            {
                t1+=G[k]*(data[kfactor1]+
                      data[kfactor2]);
                t2+=G[k]*(data[cindex-k]+
                      data[cindex+k]);
            }
            gsmooth_x[cindex]=(float) t1;
            gsmooth_y[cindex]=(float) t2;
        }

   /* produce x and y convolutions with derivative of
       gaussian */

   for (i=filter_real_w-1;i<ilimit;i++)
       for(j=jstart;j<jlimit;j+=cols)
        {
            t1=0;
            cindex=i+j;
            for (k=1;k<filter_real_w;++k)
			{  t2 = gsmooth_x[cindex-k]- gsmooth_x[cindex+k];
                t1+=( dG[k]*t2 );
			}
            GxImg[cindex]=t1;
        }

  for (i=filter_real_w-1;i<cols-(filter_real_w-1);i++)
       for(j=jstart;j<jlimit;j+=cols)
        {
            t2=0.0;
            cindex=i+j;
            for (k=1,kfactor=cols;
                 k<filter_real_w;
                 k++,kfactor+=cols)
                t2+=dG[k]*(gsmooth_y[cindex-kfactor]-gsmooth_y[cindex+kfactor]);

            GyImg[cindex]=t2;
        }


  delete [] gsmooth_y;
  delete [] gsmooth_x;
   return(1);
}



/* hypot can't cope when both it's args are zero, hence this hack.... */
double CCannyEdgeDetectorCode::hypotenuse(double x, double y)
{double r;
 r=sqrt(x*x+y*y);
    if (x==0.0 && y==0.0) return(0.0);
    else return(r);
}

void CCannyEdgeDetectorCode::thresholding_tracker(int cols, int rows,unsigned char *data)
{
   	 int i,j;    /* counters */
    /* clear data array before tracking */
    for (i=0;i<cols*rows;++i) data[i]=0;

    /* threshold image with hysteresis: follow from
       each strong edge point */
    for (i=0;i<cols;++i)
        for (j=0;j<rows;++j)
            if (mag_data[i+cols*j]> high)
                 follow(i,j,cols,rows,data);
}

/* follow a connected edge */
int CCannyEdgeDetectorCode::follow(int i,int j, int cols, int rows, unsigned char * data)
{
    int k,l;        /* counters */
    int i_plus_1,i_minus_1,j_plus_1,j_minus_1;
    long index,kindex;
    char break_flag;

    i_plus_1=i+1;
    i_minus_1=i-1;
    j_plus_1=j+1;
    j_minus_1=j-1;
    index=i+j*cols;
    if (j_plus_1>=rows) j_plus_1=rows-1;
    if (j_minus_1<0) j_minus_1=0;
    if (i_plus_1>=cols) i_plus_1=cols-1;
    if (i_minus_1<0) i_minus_1=0;

    if (!data[index])
    {
        /* current point must be added to the list of tracked points */
        data[index]=255; /* magnitude[index]; */
        /* now we can check immediately adjacent points to see if
           they too could be added to the track */
        break_flag=0;
        for (k=i_minus_1;k<=i_plus_1;k++)
        {
            for(l=j_minus_1;l<=j_plus_1;++l)
            {
                kindex=k+l*cols;
                if (!(l==j && k==i) &&mag_data[kindex]>=low )
                {
                    if (follow(k,l,cols,rows,data))
                    {
                        break_flag=1;
                        break;
                    }
                }
            }
            if (break_flag) break;
        }
        return(1);
    }
    else return(0);
}

void CCannyEdgeDetectorCode::DoGMask( double s)
{int n;
 double a,b,c,d;
 for(n=0; n<20; ++n)
    {
        a=gaussian(((double) n),s);
        if (a>0.005 || n<2)
        {
            b=gaussian((double)n-0.5,s);
            c=gaussian((double)n+0.5,s);
            d=gaussian((double)n,s*0.5);
            G[n]=(a+b+c)/3/(6.283185*s*s);
            dG[n]=c-b;
            D2G[n]=1.6*d-a;      
        }
                else break;
    }
   filter_real_w=n;
}


double CCannyEdgeDetectorCode::gaussian(double x,double s)
{
    return(exp((-x*x)/(2*s*s)));
}
