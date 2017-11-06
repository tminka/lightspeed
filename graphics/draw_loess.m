function [h,y] = draw_loess(x,y,cutpoints,linestyle,f)
%DRAW_LOESS   Draw trend lines.
% DRAW_LOESS(X,Y,CUTPOINTS) draws lines indicating the average y-coordinate of points falling into in each interval of x-coordinates.
% CUTPOINTS specifies the number of cuts (where the cuts are computed via cut_quantile), or a vector of cutpoints in x.
% DRAW_LOESS(X,Y,CUTPOINTS,LINESPEC) also specifies the line style and color.
% DRAW_LOESS(X,Y,CUTPOINTS,LINESPEC,F) combines the y values in each group using function F instead of @mean.

bad = isnan(x) | isnan(y);
if any(bad)
	good = ~bad;
	x = x(good);
	y = y(good);
end
[c,b] = cut_quantile(double(x),cutpoints);
b = [min(x) b max(x)];
if nargin<5
	% average y's for equal c's
	m = meanByGroup(c,y);
else
	m = [];
	for i = 1:max(c)
		m(i) = feval(f,y(c==i));
	end
end

connected = 1;
if connected
	b2 = (b(1:end-1) + b(2:end))/2;
	h = line(b2, m);
else
	h = [];
	for i = 1:length(m)
		h(i) = line([b(i) b(i+1)],[m(i) m(i)]);
	end
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
