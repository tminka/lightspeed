/*
 * INT_HIST(x, n) is a histogram of all integer values 1..n in x.
 * If n is not given, max(x) is used.
 */
#include "mex.h"
#include "util.h"

#define GetMax(T) \
	{ T *indata = mxGetData(nArray); \
  	bins = indata[0]; \
  	for(i=0;i<len;i++) { \
			if(indata[i] > bins) bins = indata[i]; \
		} \
	}	

#define Loop(T) \
	{ T *indata = mxGetData(nArray); \
		for(i=0;i<len;i++) { \
			mwSize v = (mwSize)(*indata++) - 1; \
			if((v < 0) || (v >= bins)) \
				mexErrMsgTxt("value out of bounds"); \
			outdata[v]++; \
		} \
	} 

void mexFunction(int nlhs, mxArray *plhs[],
		 int nrhs, const mxArray *prhs[])
{
  mwSize len, i, bins;
	double *outdata;
	mxClassID class;
	const mxArray *nArray;

  if((nrhs < 1) || (nrhs > 2))
    mexErrMsgTxt("Usage: h = int_hist(x, n)");

  /* prhs[0] is first argument.
   * mxGetPr returns double*  (data, col-major)
   * mxGetM returns int  (rows)
   * mxGetN returns int  (cols)
   */
	nArray = prhs[0];
	class = mxGetClassID(nArray);
  len = mxGetNumberOfElements(nArray);

  if(mxIsSparse(nArray))
    mexErrMsgTxt("Cannot handle sparse matrices.  Sorry.");

	if(len == 0) {
		plhs[0] = mxCreateDoubleMatrix(1, 0, mxREAL);
		return;
	}
	
  if(nrhs == 2) {
    if(mxGetNumberOfElements(prhs[1]) != 1) mexErrMsgTxt("n is not scalar.");
    bins = *mxGetPr(prhs[1]);
  } else {
		switch(class) {
		case mxDOUBLE_CLASS: 
			GetMax(double); 
			break;
		case mxSINGLE_CLASS:
			GetMax(float);
			break;
		case mxUINT8_CLASS:
			GetMax(uint8_T);
			break;
		case mxUINT16_CLASS:
			GetMax(uint16_T);
			break;
		case mxUINT32_CLASS:
			GetMax(uint32_T);
			break;
		case mxUINT64_CLASS:
			GetMax(uint64_T);
			break;
		case mxINT8_CLASS:
			GetMax(int8_T);
			break;
		case mxINT16_CLASS:
			GetMax(int16_T);
			break;
		case mxINT32_CLASS:
			GetMax(int32_T);
			break;
		case mxINT64_CLASS:
			GetMax(int64_T);
			break;
		default:
			mexErrMsgTxt("First argument is not a supported type");
			return;
		}
  }

  /* plhs[0] is first output */
  plhs[0] = mxCreateDoubleMatrix(1, bins, mxREAL);
  outdata = mxGetPr(plhs[0]);

	switch(class) {
	case mxDOUBLE_CLASS: 
		Loop(double);
		break;
	case mxSINGLE_CLASS:
		Loop(float);
		break;
	case mxUINT8_CLASS:
		Loop(uint8_T);
		break;
	case mxUINT16_CLASS:
  	Loop(uint16_T);
		break;
	case mxUINT32_CLASS:
		Loop(uint32_T);
		break;
	case mxUINT64_CLASS:
		Loop(uint64_T);
		break;
	case mxINT8_CLASS:
		Loop(int8_T);
		break;
	case mxINT16_CLASS:
		Loop(int16_T);
		break;
	case mxINT32_CLASS:
		Loop(int32_T);
		break;
	case mxINT64_CLASS:
		Loop(int64_T);
		break;
	default:
		mexErrMsgTxt("First argument is not a supported type");
	}
}

