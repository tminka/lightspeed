function [h,c,edges] = hist2(z,b)
% HIST2     Two-dimensional histogram.
%
% h = hist2(z) bins the elements of z into 20 equally spaced intervals (in each columns) and returns the number of rows in each rectangle.
% h = hist2(z,b) where b is a scalar, uses b bins in each dimension.
% h = hist2(z,b) where b is a matrix, uses b to define the cutpoints.
% [h,c,e] = hist2(...) also returns the center of each rectangle in C and the cutpoints in E.  E can be passed as the second argument to hist2 to get another histogram over the same bins.
% hist2(...) without output arguments produces a contour plot of the histogram.

if nargin < 2
	b = 20;
end
z1 = z(:,1);
z2 = z(:,2);
if rows(b) > 1
	edges1 = b(:,1);
	edges2 = b(:,2);
else
	edges1 = linspace(min(z1),max(z1),b)';
	edges2 = linspace(min(z2),max(z2),b)';
end
h = zeros(length(edges1),length(edges2));
for i = 1:(cols(h)-1)
	j = (edges2(i) <= z2) & (z2 < edges2(i+1));
	h(:,i) = histc(z1(j),edges1);
end
c1 = (edges1 + [edges1(2:end); edges1(end)])/2;
c2 = (edges2 + [edges2(2:end); edges2(end)])/2;
if nargout == 0
	contour(c1,c2,h)
end
c = [c1 c2];
edges = [edges1 edges2];
