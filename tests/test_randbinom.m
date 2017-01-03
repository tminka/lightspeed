% Computes the speed difference between Matlab's binornd and 
% Lightspeed's randbinom.

if ~exist('binornd')
  disp('binornd not found.  skipping test_randbinom.');
else

	for type = 1:7
		if type == 1
			fprintf('double\n');
			n = 123;
		elseif type == 2
			fprintf('single\n');
			n = single(123);
		elseif type == 3
			fprintf('uint64\n');
			n = uint64(123);
		elseif type == 4
			fprintf('int64\n');
			n = int64(123);
		elseif type == 5
			fprintf('uint32\n');
			n = uint32(123);
		elseif type == 6
			fprintf('uint16\n');
			n = uint16(123);
		elseif type == 7
			fprintf('uint8\n');
			n = uint8(123);			
		end
		% test multiple p, single n
		p = [0.9 0.8];
		y = randbinom(p,n);
		if any(y == 0)
			error('randbinom failed')
		end
		% test multiple p, multiple n
		n2 = repmat(n,1,2);
		y = randbinom(p,n2);
		if any(size(y) ~= [1 2])
			error('randbinom returned wrong size')
		end
		tim = [];
		% test single p, multiple n
		p = 0.9;
		n = repmat(n,1,1000);
		nsamples = 1e2;
		y = zeros(nsamples,length(n));
		tic
		for i = 1:nsamples
			y(i,:) = randbinom(p,n);
		end
		disp(y(1,1:4))
		if any(any(y == 0))
			error('randbinom failed')
		end
		tim(1) = toc;
		%g = int_hist(y+1,n+1)/nsamples;
		nd = double(n);
		tic
		for i = 1:nsamples
			y(i,:) = binornd(nd,p);
		end
		tim(2) = toc;
		fprintf('Time for binornd: %g\n', tim(2));
		fprintf('Time for randbinom: %g (%g times faster)\n', tim(1), tim(2)/tim(1));
	end

  if 0
    % test validity of the sampler (use nsamples = 1e5)
    x = 0:n;
    f = binopdf(x,n,p);
    plot(x,f,x,g)
    legend('true','estimated')
  end
end
