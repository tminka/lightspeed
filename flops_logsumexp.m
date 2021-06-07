function f = flops_logsumexp(a, dim)
% flops_logsumexp     Flops for logsumexp.
% flops_logsumexp(a, dim) returns the number of flops for logsumexp(a, dim).

[n,m] = size(a);
f = flops_exp*n*m;
if dim == 1
	f = f + flops_col_sum(a) + flops_log*m;
else
	f = f + flops_row_sum(a) + flops_log*n;
end
