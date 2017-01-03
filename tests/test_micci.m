a=rand(64,64);
%a = a(:);
% faster for k>=64
k = 1;
for i = 1:2
tic;b=xrepmat(a,256*k, 256/k);toc;
tic;b=repmat(a,256*k, 256/k);toc;
end


a=randn(67,54);
for i = 1:2
	tic;b=xrepmat(a,234, 564);toc;
	tic;b=repmat(a,234, 564);toc;
end
