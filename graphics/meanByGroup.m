function [m,v] = meanByGroup(x,y)

if cols(x) > 1
	error('cols(x) > 1')
end
if cols(y) > 1
	error('cols(y) > 1')
end
all1 = ones(rows(x),1);
count = sparse(x, all1, ones(rows(y),1));
m = sparse(x, all1, y)./count;
m = full(m);
if nargout > 1
	m2 = sparse(x, all1, y.*y)./count;
	m2 = full(m2);
	v = m2 - m.*m;
end
