function [c,b] = cut_quantile(x,n)
% CUT_QUANTILE   Index of quantiles.
% cut_quantile(x,n) returns a matrix of quantile numbers, same size as x, 
% ranging 1..n (n is the desired number of quantiles).  
% The values with number 1 are in the first quantile, etc.
% [c,b] = cut_quantile(x,n) returns the cutpoints (quantile values) selected
%
% Example:
%   cut_quantile(1:10,4)

if length(n) > 1
	% the length of b determines the number of quantiles
	b = n;
else
	probs = (1:(n-1))/n;
	b = unique(quantile(x(:),probs));
end

c = ones(size(x));
for b_iter = 1:length(b)
  c(x >= b(b_iter)) = b_iter+1;
end
