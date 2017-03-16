Lightspeed matlab toolbox
=========================

This library provides:

* highly optimized versions of mathematical functions such as `normcdf`, set intersection, and `gammaln`
* efficient random number generators
* evaluation of common probability densities
* routines for counting floating-point
operations (FLOPS), useful for benchmarking algorithms.
* utilities such as filename globbing and parsing of variable-length argument lists.
* graphics functions such as `axis_pct` and `mobile_text` (in the graphics subdirectory).

See Contents.m for a table of contents.

Also see my general tips on [accelerating matlab](http://tminka.github.io/software/matlab/).

Flop counting
=============

Matlab is frequently used for research, so I have included routines
that faciliate research.
In particular, Lightspeed features a set of routines for accurate
floating-point operation (flop) counting.  These flop counts allow
machine-independent and programmer-independent comparison of numerical
algorithms, because they represent the minimal number of operations that
the algorithm needs.  Consequently, you can compare algorithms based on
their Matlab implementations alone, without having to code them efficiently
in C and report their run time on a particular processor.
Hopefully these routines will allow more informative algorithm 
comparisons in research papers.

The flop count routines in lightspeed are significantly more accurate than
the `flops` function which was included in Matlab up to version 5.
For example, according to Matlab 5, `inv(3)` requires more flops
than `1/3`.  Matlab 5 also returned slightly
incorrect flop counts for matrix multiplies and overly pessimistic flop
counts for solving linear systems.  Lightspeed always returns the minimal
number of flops for matrix operations, as though the best possible
algorithm was used, no matter what method you are actually using.

Perhaps the biggest difference between flop counting in lightspeed versus
Matlab 5 is the handling of special functions like `exp` and
`sqrt`.  Matlab 5 counted these as one flop, which is much too low.
A more accurate count for `exp`, based on the
Pentium 4 processor, is 40 flops.  Similarly, `sqrt` is 8 flops.
Interestingly, the run time in Matlab 6 for `sqrt` is
significantly longer than `exp`, and even longer than the time
for `x^(0.51)` (raising to an arbitrary power other than 0.5).
This emphasizes the importance of measuring the `idealized' run time,
represented by flops, rather than the actual run time, which is subject to
odd inefficiencies in Matlab (or any programming language). 

Flop counting in lightspeed is a more manual process than in Matlab 5.  In Matlab 5, the flop counter is incremented automatically after every operation.  Lightspeed does not increment the flop counter automatically.  Instead, you must specify which operations should have their flops counted.  For example, after performing a matrix multiply you should call `addflops(flops_mul(...))`, and after every Cholesky decomposition you should call `addflops(flops_chol(...))`.  Some operations do not have a dedicated flops routine.  For these you should consult the help for `flops`.

Manual flop counting has two advantages.  First, it can be different from the operation that you actually performed, allowing you to count the flops for an `idealized' algorithm rather than the one you implemented.  Second, since only the operations that you explicitly count get added to the flop counter, unrelated operations (such as debugging code) will not interfere with the result.  

Incrementing the flop counter on every operation can cause your code to run slower.  To avoid this, you can batch up the count for many operations.  For example, to get the flop count for a loop, you can save time by computing the flops for one iteration of the loop and then multiply by the number of iterations.  For examples, see [fastfit/dirichlet_fit_newton.m](https://github.com/tminka/fastfit/blob/master/dirichlet_fit_newton.m) or [logreg/train_cg.m](https://github.com/tminka/logreg/blob/master/train_cg.m).

Installation
============

The toolbox has been tested on all versions of Matlab from 6.5 to 8.4 with
Windows XP, Vista, 7, and 8.  It has been tested on 32-bit and 64-bit machines, with Microsoft Visual Studio 2008-2013 (Professional and Express Editions).  It should work on Macs and Linux as well.  Most (but not all) functions work with Matlab 6.1 and 5. 

You can place the lightspeed directory anywhere.
To make sure lightspeed is always in your path, create a startup.m
file in your matlab directory, if you don't already have one, and add
a line like this:

    addpath(genpath('c:\matlab\lightspeed'))

Replace 'c:\matlab\lightspeed' with the location of the lightspeed directory.

There are some Matlab Extension (MEX) files that need to be compiled.
This can be done in matlab via:

    cd c:\matlab\lightspeed
    install_lightspeed

I recommend using Microsoft Visual C++ as the mex compiler, though this is not required.  You can set the mex compiler by typing 'mex -setup' in matlab.

You can find timing tests in the tests/ subdirectory.  
The test_lightspeed.m script will run all tests, and is a good way to check 
that lightspeed installed properly.

Troubleshooting
===============

If you are having problems compiling the mex files, check your matlab installation by compiling one of the examples that comes with matlab, such as:
    mex([matlabroot '/extern/examples/mex/explore.c'])
If this does not work, then contact MathWorks for help.  You can find some common fixes below.

To use Microsoft Visual C++ 2013 with Matlab 8.1 (R2013a), you will need to download this patch:
http://www.mathworks.com/matlabcentral/fileexchange/45878-setting-microsoft-visual-c++-2013-as-default-mex-compiler

To use Microsoft Visual C++ 2010 with Matlab 7.10 (R2010a), you will need to download this patch:
http://www.mathworks.com/support/solutions/en/data/1-D5W493/?solution=1-D5W493

To use Microsoft Visual C++ 2008 on Windows 64-bit, you will need to follow the instructions at:
http://www.mathworks.com/matlabcentral/answers/98351-how-can-i-set-up-microsoft-visual-studio-2008-express-edition-for-use-with-matlab-7-7-r2008b-on-64

To use Microsoft Visual C++ with Matlab 7.0 (R14), you will need to download 
R14 service pack 2, as described here:
http://www.mathworks.com/support/solutions/en/data/1-UMEKK/?solution=1-UMEKK

To compile mex files on a Snow Leopard upgrade, prior to Matlab R2014a:

1. Go to mexopts.sh in your $HOME/.matlab/ directory, and change the line SDKROOT='/Developer/SDKs/MacOSX10.5.sdk' to SDKROOT='/Developer/SDKs/MacOSX10.6.sdk'. That line is not updated during updating mac OSX, so you need to do manually. This file also exists in the standard matlab bin, so if you run mex with the -v option it will tell you which mexopts.sh file it's looking at.

2. You may also need to change some lines in install_lightspeed.m.  By default, install_lightspeed.m is set up for 64-bit MacOSX 10.6 with gcc-4.0.  If you are using some other version of MacOSX or some other compiler, then you need to edit a few lines (see the comments in that file).

Changelist
==========

### 2.8
Changed to MIT license.  intersect_sorted provides multiple outputs, for compatibility with intersect.  Added support for different number types to randbinom and int_hist.  Added graphics/draw_loess.  Fixes to install_lightspeed, flops_solve, hhist, cut_quantile.

### 2.7
Added the hist2 function.
Updated the installer to handle Matlab versions up to R2014b.  In version R2014a and later, installation on Mac is now much easier.  In version R2013b and later, lightspeed does not replace Matlab's built-in repmat, because the built-in is now faster.  Congratulations to MathWorks---it only took them 11 years to catch up with lightspeed's repmat.  Perhaps now they can try making sum(x,2) run as fast as lightspeed's row_sum.

### 2.6

Updated the installer to handle MacOSX 10.6 and older versions of matlab (all the way back to 7.0.1).

### 2.5

Updated the installer to handle Microsoft Visual Studio 2010 Express with Windows SDK 7.1.  All mex files now use -largeArrayDims.  Fixed a bug in multivariate `gammaln` (used by `wishpdf`).

### 2.4

Lightspeed's normpdf is now called mvnormpdf, for compatibility with the statistics toolbox.  Added invnormcdf.  Updated the installer.

### 2.3

Support for Matlab R2009a and 64-bit architectures.
`install_lightspeed` is more robust to different matlab versions and platforms (pc, mac, unix).
`setnonzeros` and `sparse_nocheck` are new functions for quickly filling in a sparse matrix.  
Flop counting is more accurate in some cases.  Added `flops_lu` and `flops_abs`.
Fixed a bug in `union_rows_sorted`.
Added the `tetragamma` function.

### 2.2

The installer now supports Matlab 7.5.
`randomseed` is a new function that allows you to set the seed for lightspeed's random number generator.
`cholproj` is a new function that provides an approximate cholesky decomposition even when a matrix is not exactly positive definite, for example due to roundoff errors.
The tests have been moved to a subdirectory.
More bug-fixes, thanks to people who have sent in bug reports.

### 2.1

`graphics/hhist` has been greatly improved, and now has the same
interface as `hist`.  You'll never have to use `hist` again!
`normcdflogit` is a new function that provides accurate 
evaluation of the normal CDF in the tails.
A variety of bug-fixes, thanks to people who have sent in bug reports.

### 2.0

Many new functions have been added, including sorted-set functions and
mutation.  Graphics functions have been moved to a
subdirectory.  The formulas for flop counting have been changed, to better
reflect the Pentium 4 architecture.  In particular, note the addition of
`flops_div` (division is 8 flops on a Pentium 4),
`flops_sqrt`, and `flops_log`.

### 1.3

The main change from version 1.2 is that tri_inv_times has been renamed to
solve_triu and solve_tril, and made more efficient.
The tricky part is that these have to be linked against Matlab's
internal BLAS library.

The mex files have also been changed to use the Matlab 5 API.



Tom Minka
