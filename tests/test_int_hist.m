if ~isempty(int_hist([]))
	error('int_hist([]) failed');
end

M = [repmat(1:10,1,10) 7*ones(1,17) 57];

niter = 1000;
tic; for i = 1:niter int_hist(M); end; toc
tic; for i = 1:niter int_hist(int32(M)); end; toc
tic; for i = 1:niter int_hist(int64(M)); end; toc
