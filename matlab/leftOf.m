function [chooseQuery] = leftOf(pointOnHull,endPoint,queryRow,queryCol,startPoint)
%LEFTOF Summary of this function A            B        M
%   Detailed explanation goes here'

chooseQuery = 0;

temp = (endPoint(1) - pointOnHull(1))*(queryCol -  pointOnHull(2))- ...
    (endPoint(2) - pointOnHull(2))*(queryRow - pointOnHull(1));

if(temp == 0)
    if(queryRow == startPoint(1) & queryCol == startPoint)
        chooseQuery = 1;
        return;
    end
    dist1 = (endPoint(1) - pointOnHull(1))^2 + (endPoint(2) - pointOnHull(2))^2 ;
    dist2 = (queryRow - pointOnHull(1))^2 + (queryCol - pointOnHull(2))^2 ;
    chooseQuery = (dist2 > dist1);
    return;
end
if(temp > 0)
    chooseQuery = 1;
    return;
end
end

