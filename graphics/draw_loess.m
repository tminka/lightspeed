function [h,y] = draw_loess(x,y,cutpoints,linestyle)
%DRAW_LOESS   Draw trend lines.
% DRAW_LOESS(X,Y,CUTPOINTS) draws lines indicating the average y-coordinate of points falling into in each interval of x-coordinates.
% CUTPOINTS specifies the number of cuts (where the cuts are computed via cut_quantile), or a vector of cutpoints in x.
% DRAW_LOESS(X,Y,CUTPOINTS,LINESPEC) also specifies the line style and color.

[c,b] = cut_quantile(double(x),cutpoints);
b = [min(x) b max(x)];
% average y's for equal c's
y = meanByGroup(c,y);

h = [];
for i = 1:length(y)
	h(i) = line([b(i) b(i+1)],[y(i) y(i)]);
end
if nargin > 3
	set_linespec(h,linestyle);
end

if 0
[x,order] = sort(double(x));
y = double(y(order));
[s,dummy,loc] = unique(x);
if length(s) < length(x)
end
% The error message "The grid vectors are not strictly monotonic increasing." means you need to increase span.
f = fLOESS([x(:) y(:)],span);
hold on
plot(x,f,linestyle)
hold off
end
