function array3D = generateArray(startPt, endPt, numPts)
%GENERATEARRAY Generates an array of 3D-points (x,y,z).
%   Given start and end point of a straight line, an array of equi-distant 
%   3D-points will be generated with start and end points bounded at the ends.
%
%   startPt start point coordinate [x1 y1 z1]
%   endPt   end point coordinate [x2 y2 z2]
%   numPts  number of points to generate including start and end points

    array3D = zeros(numPts, 3);
    x = startPt;
    y = endPt;
    array3D(1,:) = x;
    array3D(numPts, :) = y;
    for i=1:numPts-2
        array3D(i+1,:) = x + i*(y-x)/(numPts-1);
    end
end