/* compile with: 
      Windows: mex randbinom.c util.obj
      Others:  cmex randbinom.c util.o -lm
 */
#include "mexutil.h"
#include "util.h"

#define CrossLoop32(T) \
	{ T *outdata = mxGetData(outArray);							\
		for(j=0;j<nCount;j++) {  \
    unsigned n32 = ((T *)nData)[j];	\
		for(i=0;i<pCount;i++) { \
			*outdata++ = BinoRand32(pdata[i], n32); \
		} } }																			
#define CrossLoop64 \
		for(i=0;i<pCount;i++) { \
			*outdata++ = BinoRand(pdata[i], n); \
		}


void RandBinomCross(mxArray *outArray, double *pdata, void *nData,
										mxClassID class,
										mwSize pCount, mwSize nCount)
{
  mwSize i,j;

	switch(class) {
	case mxDOUBLE_CLASS: {
		double *outdata = mxGetData(outArray);
		for(j=0;j<nCount;j++) {
			double nDouble = ((double*)nData)[j];
			mwSize n = (mwSize)nDouble;
			if((double)n != nDouble)
				mexErrMsgTxt("n is not integer or out of 64-bit range");
			CrossLoop64;
		}
	}	break;
	case mxSINGLE_CLASS: {
		float *outdata = mxGetData(outArray);
		for(j=0;j<nCount;j++) {
			float nSingle = ((float*)nData)[j];
			mwSize n = (mwSize)nSingle;
			if((float)n != nSingle)
				mexErrMsgTxt("n is not integer or out of 64-bit range");
			CrossLoop64;
		}
	}	break;
	case mxUINT64_CLASS: {
		uint64_T *outdata = mxGetData(outArray);
		for(j=0;j<nCount;j++) {
			mwSize n = ((uint64_T*)nData)[j];
			CrossLoop64;
		}
	}	break;
	case mxINT64_CLASS: {
		int64_T *outdata = mxGetData(outArray);
		for(j=0;j<nCount;j++) {
			mwSize n = ((int64_T*)nData)[j];
			CrossLoop64;
		}
	}	break;
	case mxUINT8_CLASS:
		CrossLoop32(uint8_T);
		break;
	case mxUINT16_CLASS:
  	CrossLoop32(uint16_T);
		break;
	case mxUINT32_CLASS:
		CrossLoop32(uint32_T);
		break;
	case mxINT8_CLASS:
		CrossLoop32(int8_T);
		break;
	case mxINT16_CLASS:
		CrossLoop32(int16_T);
		break;
	case mxINT32_CLASS:
		CrossLoop32(int32_T);
		break;
	default:
		mexErrMsgTxt("Second argument is not a supported type");
	}
}

#define Loop32(T) \
	{ T *outdata = mxGetData(outArray);							\
		for(i=0;i<nCount;i++) {  \
    unsigned n32 = ((T *)nData)[i];	\
		outdata[i] = BinoRand32(pdata[i], n32); \
		} }																			

/* pdata and nArray must be the same size
 */
void RandBinom(mxArray *outArray, double *pdata, void *nData, 
							 mxClassID class,
							 mwSize nCount)
{
  mwSize i;

	switch(class) {
	case mxDOUBLE_CLASS: {
		double *outdata = mxGetData(outArray);
		for(i=0;i<nCount;i++) {
			double nDouble = ((double*)nData)[i];
			mwSize n = (mwSize)nDouble;
			if((double)n != nDouble)
				mexErrMsgTxt("n is not integer or out of 64-bit range");
			outdata[i] = BinoRand(pdata[i], n);
		}
	}	break;
	case mxSINGLE_CLASS: {
		float *outdata = mxGetData(outArray);
		for(i=0;i<nCount;i++) {
			float nSingle = ((float*)nData)[i];
			mwSize n = (mwSize)nSingle;
			if((float)n != nSingle)
				mexErrMsgTxt("n is not integer or out of 64-bit range");
			outdata[i] = BinoRand(pdata[i], n);
		}
	}	break;
	case mxUINT64_CLASS: {
		uint64_T *outdata = mxGetData(outArray);
		for(i=0;i<nCount;i++) {
			mwSize n = ((uint64_T*)nData)[i];
			outdata[i] = BinoRand(pdata[i], n);
		}
	}	break;
	case mxINT64_CLASS: {
		int64_T *outdata = mxGetData(outArray);
		for(i=0;i<nCount;i++) {
			mwSize n = ((int64_T*)nData)[i];
			outdata[i] = BinoRand(pdata[i], n);
		}
	}	break;
	case mxUINT8_CLASS:
		Loop32(uint8_T);
		break;
	case mxUINT16_CLASS:
  	Loop32(uint16_T);
		break;
	case mxUINT32_CLASS:
		Loop32(uint32_T);
		break;
	case mxINT8_CLASS:
		Loop32(int8_T);
		break;
	case mxINT16_CLASS:
		Loop32(int16_T);
		break;
	case mxINT32_CLASS:
		Loop32(int32_T);
		break;
	default:
		mexErrMsgTxt("Second argument is not a supported type");
	}
}

void mexFunction(int nlhs, mxArray *plhs[],
		 int nrhs, const mxArray *prhs[])
{
  mwSize ndims, *dims, pCount, nCount, i;
	mwSize ndims2, *dims2, noutdims, *outdims;
  double *pData;
  void *nData;
	const mxArray *nArray, *pArray;
	mxClassID nClass;

  if(nrhs != 2)
    mexErrMsgTxt("Usage: r = bino_sample(p, n)");

  /* prhs[0] is first argument.
   * mxGetPr returns double*  (data, col-major)
   */
  pArray = prhs[0];
  ndims = mxGetNumberOfDimensions(pArray);
  dims = (mwSize*)mxGetDimensions(pArray);
  pCount = mxGetNumberOfElements(pArray);
	pData = mxGetPr(pArray);

	nArray = prhs[1];
  ndims2 = mxGetNumberOfDimensions(nArray);
  dims2 = (mwSize*)mxGetDimensions(nArray);
  nCount = mxGetNumberOfElements(nArray);
	nData = mxGetData(nArray);

  /* plhs[0] is first output */
	if(nCount == 1) {
		noutdims = ndims;
		outdims = dims;
	} else {
		noutdims = ndims2;
    outdims = dims2;
    if(pCount > 1) {
			/* check that dimensions match */
			for(i=0;i<ndims;i++) {
				if(i>=ndims2) {
          /* dims2[i] == 1 */
          if(dims[i] != 1) 
						mexErrMsgTxt("P and N have mismatched array dimensions");
        } else if(dims[i] != dims2[i]) {
					mexErrMsgTxt("P and N have mismatched array dimensions");
        }
      }
      for(;i<ndims2;i++) {
				if(dims2[i] != 1)
					mexErrMsgTxt("P and N have mismatched array dimensions");
      }
		}
  }
	nClass = mxGetClassID(nArray);
  plhs[0] = mxCreateNumericArrayE(noutdims, outdims, nClass, mxREAL);
  if(mxIsSparse(prhs[0]) || mxIsSparse(nArray))
    mexErrMsgTxt("Cannot handle sparse matrices.  Sorry.");
	if(nCount == 1 || pCount == 1)
		RandBinomCross(plhs[0], pData, nData, nClass, pCount, nCount);
  else
		RandBinom(plhs[0], pData, nData, nClass, pCount);
}

