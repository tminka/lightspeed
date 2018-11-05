/* C implementation of repmat.
 * by Tom Minka
 */
/*
mex -c mexutil.c
mex repmat.c mexutil.obj
to check for warnings:
gcc -Wall -I/cygdrive/c/Program\ Files/MATLAB/R2008a/extern/include -c repmat.c
*/
#include "mexutil.h"
#include <string.h>

/* ALWAYS_2D == 1 gives the usual matlab behavior where repmat(0,3) is
 * a 3x3 matrix.
 * ALWAYS_2D == 0 gives a 3x1 matrix instead.
 * repmat(x,sizes,1) is another way to get this behavior.
 */
#define ALWAYS_2D 1

/* Repeat a block of memory rep times.
 * The characters from dest[0] .. dest[chunk-1] are copied to
 * dest[chunk] .. dest[2*chunk-1] as well as
 * dest[2*chunk] .. dest[3*chunk-1] and so on up to
 * dest[(rep-1)*chunk] .. dest[(rep-1)*chunk + chunk-1]
 */
void memrep(char *dest, mwSize chunk, mwSize rep)
{
  /*
  printf("chunk = %ld, rep = %ld\n", chunk, rep);
  */
  if(chunk >= 1024) {
    /* big chunks */
    mwSize i;
    char *p = dest;
    for(i=1;i<rep;i++) {
      p += chunk;
      memcpy(p, dest, chunk);
				/* memcpy(p, p-chunk, chunk); */
    }
  } else {
    /* small chunks */
    if(rep == 1) return;
    memcpy(dest + chunk, dest, chunk); 
    if(rep & 1) {  /* odd number of repetitions */
      dest += chunk;
      memcpy(dest + chunk, dest, chunk);
    }
    /* now repeat using a block twice as big */
    memrep(dest, chunk<<1, rep>>1);
  }
}

/*
 * ndim = 1 + the dimension to repeat
 * dims = dimensions of the source array
 * dimsize = stride of the source array
 * destdimsize = stride of the destination array
 * rep[i] = number of times to repeat dimension i
 */
void repmat(char *dest, const char *src, mwSize ndim, mwSize *destdimsize, 
	    mwSize *dimsize, const mwSize *dims, mwSize *rep) 
{
  mwSize d = ndim-1;
  mwSize i;
  mwSize chunk;
  /*
    printf("repmat(ndim=%ld,dims=%ld,%ld)\n", ndim,dims[0],dims[1]);
	*/
  /* copy the first repetition into dest */
  if(d == 0) {
    chunk = dimsize[0];
    memcpy(dest,src,chunk);
  }
  else {
    /* recursively repeat each slice of src */
    for(i=0;i<dims[d];i++) {
      repmat(dest + i*destdimsize[d-1], src + i*dimsize[d-1], 
	     ndim-1, destdimsize, dimsize, dims, rep);
    }
    chunk = destdimsize[d-1]*dims[d];
  }
  /* copy the result rep-1 times */
  memrep(dest,chunk,rep[d]);
}

/* Experimental version for 2D arrays that copies each column repeatedly.
 */
void repmat2(char *dest, const char *src, mwSize ndim, mwSize *destdimsize, 
	    mwSize *dimsize, const mwSize *dims, mwSize *rep) 
{
  mwSize i,j,k;
  mwSize chunk = dimsize[0];
  char *p;
  for(i=0;i<dims[1];i++) {
    p = dest + i*destdimsize[0];
    for(j=0;j<rep[1];j++) {
      for(k=0;k<rep[0];k++) {
        memcpy(p,src,chunk);
        p += chunk;
      }
      p += destdimsize[0]*(dims[1]-1);
    }
    src += chunk;
  }
}

void readsizearray(mwSize *dest, const mxArray *ar, mwSize len)
{
    mwSize j;
    
    switch (mxGetClassID(ar)) {
        case mxDOUBLE_CLASS:
        {
            double *sp = mxGetPr(ar);
            for (j = 0; j < len; j++)
                dest[j] = sp[j];
        } break;
        case mxSINGLE_CLASS:
        {
            float *sp = mxGetData(ar);
            for (j = 0; j < len; j++)
                dest[j] = sp[j];
        } break;
        case mxUINT8_CLASS:
        {
            uint8_T *sp = mxGetData(ar);
            for (j = 0; j < len; j++)
                dest[j] = sp[j];
        } break;
        case mxUINT16_CLASS:
        {
            uint16_T *sp = mxGetData(ar);
            for (j = 0; j < len; j++)
                dest[j] = sp[j];
        } break;
        case mxUINT32_CLASS:
        {
            uint32_T *sp = mxGetData(ar);
            for (j = 0; j < len; j++)
                dest[j] = sp[j];
        } break;
        case mxINT8_CLASS:
        {
            int8_T *sp = mxGetData(ar);
            for (j = 0; j < len; j++)
                dest[j] = sp[j];
        } break;
        case mxINT16_CLASS:
        {
            int16_T *sp = mxGetData(ar);
            for (j = 0; j < len; j++)
                dest[j] = sp[j];
        } break;
        case mxINT32_CLASS:
        {
            int32_T *sp = mxGetData(ar);
            for (j = 0; j < len; j++)
                dest[j] = sp[j];
        } break;
        default:
            mexErrMsgTxt("Unknown type in size argument(s) to repmat.");
    }
}


void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
  const mxArray *srcmat;
  mwSize ndim, eltsize;
  mwSize *dimsize;
  const mwSize *dims;
  mwSize ndimdest;
  mwSize *destdims, *destdimsize;
  char *src, *dest;
  mwSize *rep;
  mwSize i,nrep;
  mwSize extra_rep = 1;
  int empty;
	double *outp, *inp;
	mwSize m,n,numel;

  if(nrhs < 2) mexErrMsgTxt("Usage: repmat(A, [M N ...])");
  srcmat = prhs[0];
	if(0) {
		/* testing code, please ignore */
		/*
		plhs[0] = mxCreateNumericArrayE(ndim, dims, mxGetClassID(srcmat), 
																		mxIsComplex(srcmat)?mxCOMPLEX:mxREAL);
																		*/
		m = mxGetM(srcmat);
		n = mxGetN(srcmat);		
		plhs[0] = mxCreateDoubleMatrixE(m, n, mxREAL);
		outp = mxGetPr(plhs[0]);
		inp = mxGetPr(srcmat);
		numel = mxGetNumberOfElements(srcmat);
		memcpy(outp, inp, numel*sizeof(double));
		/*
		plhs[0] = mxCreateNumericMatrixE(dims[0], dims[1], mxGetClassID(srcmat), 
																		mxIsComplex(srcmat)?mxCOMPLEX:mxREAL);
		plhs[0] = mxCreateNumericMatrix(0, 0, mxGetClassID(srcmat), 
																		mxIsComplex(srcmat)?mxCOMPLEX:mxREAL);
		*/
		return;
	}

  if(!mxIsNumeric(srcmat) || mxIsSparse(srcmat) || mxIsCell(srcmat) || mxIsStruct(srcmat)) {
    /* call Matlab's repmat */
    mexCallMATLAB(nlhs,plhs,nrhs,(mxArray**)prhs,"xrepmat");return;
    /* mexErrMsgTxt("Sorry, can't handle sparse matrices yet."); */
  }
  ndim = mxGetNumberOfDimensions(srcmat);
  dims = mxGetDimensions(srcmat);
  eltsize = mxGetElementSize(srcmat);
	
  /* compute dimension sizes */
  dimsize = (mwSize*)mxCalloc(ndim, sizeof(mwSize));
  dimsize[0] = eltsize*dims[0];
  for(i=1;i<ndim;i++) dimsize[i] = dimsize[i-1]*dims[i];

  /* determine repetition vector */
  ndimdest = ndim;
  if(nrhs == 2) {
    /* prhs[1] is a vector of reps */
    nrep = mxGetN(prhs[1]);
    if(nrep > ndimdest) ndimdest = nrep;
    rep = (mwSize*)mxCalloc(ndimdest, sizeof(mwSize));
    readsizearray(rep, prhs[1], nrep);
/*     for(i=0;i<nrep;i++) {
 *       double repv = mxGetPr(prhs[1])[i];
 *       rep[i] = (mwSize)repv;
       }   */
#if ALWAYS_2D
    if(nrep == 1) {
      /* special behavior */
      nrep = 2;
      rep[1] = rep[0];
    }
#endif
  }
  else {
    /* concatenate all prhs's */
    mwSize ri=0;
    nrep = 0;
    for(i=0;i<nrhs-1;i++) {
      nrep += mxGetNumberOfElements(prhs[i+1]);
    }
    if(nrep > ndimdest) ndimdest = nrep;
    rep = (mwSize*)mxCalloc(ndimdest, sizeof(mwSize));
    for(i=0;i<nrhs-1;i++) {
      mwSize sz = mxGetNumberOfElements(prhs[i+1]);
      readsizearray(rep+ri, prhs[i+1], sz);
      ri += sz;
      /* for(j=0;j<sz;j++) rep[ri++] = (mwSize)p[j]; */
    }
  }
  for(i=nrep;i<ndimdest;i++) rep[i] = 1;

  /* compute output size */
  destdims = (mwSize*)mxCalloc(ndimdest, sizeof(mwSize));
  for(i=0;i<ndim;i++) destdims[i] = dims[i]*rep[i];
  for(;i<ndimdest;i++) { 
    destdims[i] = rep[i];
    extra_rep *= rep[i];
  }
  destdimsize = (mwSize*)mxCalloc(ndim, sizeof(mwSize));
  destdimsize[0] = eltsize*destdims[0];
  for(i=1;i<ndim;i++) destdimsize[i] = destdimsize[i-1]*destdims[i];

  /* for speed, array should be uninitialized */
  plhs[0] = mxCreateNumericArrayE(ndimdest, destdims, mxGetClassID(srcmat), 
				  mxIsComplex(srcmat)?mxCOMPLEX:mxREAL);

  /* if any rep[i] == 0, output should be empty array.
     Added by KPM 11/13/02.
  */
  empty = 0;
  for (i=0; i < nrep; i++) {
    if (rep[i]==0) 
      empty = 1;
  }
  if (empty) 
    return;

  src = (char*)mxGetData(srcmat);
  dest = (char*)mxGetData(plhs[0]);
  repmat(dest,src,ndim,destdimsize,dimsize,dims,rep);
  if(ndimdest > ndim) memrep(dest,destdimsize[ndim-1],extra_rep);
  if(mxIsComplex(srcmat)) {
    src = (char*)mxGetPi(srcmat);
    dest = (char*)mxGetPi(plhs[0]);
    repmat(dest,src,ndim,destdimsize,dimsize,dims,rep);
    if(ndimdest > ndim) memrep(dest,destdimsize[ndim-1],extra_rep);
  }
}
