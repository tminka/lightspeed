function colors = YlGnBu_colors(n)
% YlGnBu_colors    A good sequential colormap.
% YlGnBu_colors(n) returns an RGB matrix of n colors (default 4).
% These colors form a sequential colormap (colors easily perceived to lie
% on a sequence).
%
% Example:
%   colormap(YlGnBu_colors(9))
%
% See also YR_colors

% Written by Tom Minka

if nargin < 1
  n = 4;
end

if n == 4
  colors = [1 1 0.8; 0.63 0.855 0.706; 0.255 0.714 0.765; 0.22 0.42 0.69];
elseif n == 9
	colors = [1 1 0.85; 0.93 0.97 0.85; 0.78 0.91 0.71; 0.5 0.8 0.73; 0.25 0.71 0.77; 0.11 0.57 0.75; 0.13 0.37 0.66; 0.15 0.2 0.58; 0.03 0.11 0.35];
else
  error('can''t make that many levels');
end
