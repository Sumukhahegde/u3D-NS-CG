#include"velocity.h"

//calculates the value of initial w velocity
void wEqn()
{
	for (i=2;i<=nxm;i++)
	 {
	 	for (j=2;j<=nym;j++)
		 {
		 	for(k=2;k<=nzm;k++)
		 	{
		 		ap[i][j][k]  = (- ae[i][j][k] - aw[i][j][k] - an[i][j][k] - as[i][j][k] - at[i][j][k] - ab[i][j][k] + apw[i][j][k])*urecrw;
      			sw[i][j][k]  = sw[i][j][k] + (1 - wUnderRelaxCoeff) * ap[i][j][k] * w[i][j][k][l] ;
      			apw[i][j][k] = 1 / ap[i][j][k];	/* For implementation in the pressure correction approach */
		 		
		 	}
 	     }
  	 }
  
   for(nonLinear=0;nonLinear<=wVelocityLoops;nonLinear++)
  {
  
//  Creating matrices for ADI-TDMA algorithm	 
  	 for (i=2;i<=nxm;i++)
	{
		for (j=2;j<=nym;j++)
		{
			for(k=2;k<=nzm;k++)
			{
				ieast  = i + 1;
	  			iwest  = i - 1;
	  			jnorth = j + 1;
	  			jsouth = j - 1;
	  			ktop   = k + 1;
	  			kbottom= k - 1;
	  			
				c[k] 	= (sw[i][j][k] - (ae[i][j][k]*w[ieast][j][k][l] + aw[i][j][k]*w[iwest][j][k][l] + an[i][j][k]*w[i][jnorth][k][l] + as[i][j][k]*w[i][jsouth][k][l]))*apw[i][j][k];
				lower[k]= ab[i][j][k]*apw[i][j][k];
				upper[k]= at[i][j][k]*apw[i][j][k];
				
				Thomas(nzm);
				
				//Equating solved value back to velocity
				for(thomasI=2;thomasI<=nzm;thomasI++)
	  				w[i][j][thomasI][l]=x[thomasI];				
			}
    	}
  }
  
  wMaxRes = 0;
  for (i=2;i<=nxm;i++)
	{
		for (j=2;j<=nym;j++)
		{
			for(k=2;k<=nzm;k++)
			{
				ieast  = i + 1;
	  			iwest  = i - 1;
	  			jnorth = j + 1;
	  			jsouth = j - 1;
	  			ktop   = k + 1;
	  			kbottom= k - 1;
	  			
	  			wRes[i][j][k] 	= sw[i][j][k] - (an[i][j][k]*w[i][jnorth][k][l] + as[i][j][k]*w[i][jsouth][k][l] + at[i][j][k]*w[i][j][ktop][l] + ab[i][j][k]*w[i][j][kbottom][l] + ae[i][j][k]*w[ieast][j][k][l] + aw[i][j][k]*w[iwest][j][k][l] + ap[i][j][k]*w[i][j][k][l]);
				wMaxRes	= wMaxRes + fabs(wRes[i][j][k]);
	  			
			}
    	}
   }
   
   
//   printf(" w Res = %e\n",wMaxRes);
   if(fabs(wMaxRes)>accuracy)		wVelocityLoops = wVelocityLoops + 1;
 }
  

}

