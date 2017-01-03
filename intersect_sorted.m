function [c,ia,ib] = intersect_sorted(a,b)
%INTERSECT_SORTED     Set intersection between sorted sets.
% INTERSECT_SORTED(A,B) when A and B are vectors returns the values common
% to both A and B.  A and B must be sorted and unique, and the result will be
% sorted and unique.
% [C,IA,IB]=INTERSECT_SORTED(A,B) also returns indices such that C=A(IA) and C=B(IB).

% Written by Tom Minka
% (c) Microsoft Corporation. All rights reserved.

if nargout>1
    [tf,ib]=ismember_sorted(a,b);
    ib=ib(tf);
    c = a(tf);
    [tf,ia]=ismember_sorted(b,a);
    ia=ia(tf);
else
    c = a(ismember_sorted(a,b));
end
