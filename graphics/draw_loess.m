function [h,y] = draw_loess(x,y,span,linestyle)
% span = number of cuts

[c,b] = cut_quantile(double(x),span);
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
