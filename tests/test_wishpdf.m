%gampdf(2,3,4)
p = 0.018954;
q = wishpdf(2,3,1/4);
if abs(q - p) > 1e-6
	error('wishpdf failed');
end

x = [2 1; 1 3];
a = 5;
p = 0.0038062;
q = wishpdf(x,a);
if abs(q - p) > 1e-6
	error('wishpdf failed');
end
