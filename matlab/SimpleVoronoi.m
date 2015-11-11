numPoints = 5;
xSize = 100;
ySize = 100;
%zSize = 1;

points = randi(xSize,[1,numPoints]);
points = [points; randi(ySize,[1,numPoints])];
%points = [points; randi(zSize,[1,numPoints])];
points = [25 75 25 75 55];
points = [points;25 25 75 75 30];


distMap = zeros(xSize,ySize,numPoints);
for k = 1:numPoints;
    for i = 1:xSize;
        for j = 1:ySize;
            %for l =1:zSize;
                distMap(i,j,k) = (points(1,k) - i)^2 + (points(2,k)-j)^2; % + (points(3,k)-l)^2;
            %end
        end
    end
end

binC = zeros(xSize,ySize);
bin = zeros(xSize,ySize);
p = 0;
for i = 1:xSize;
    for j = 1:ySize;
        count = 0;
        bin(i,j) = 1;
        min = 8000000;
        for k = 1:numPoints;
            if min > distMap(i,j,k);
                min = distMap(i,j,k);
                bin(i,j) = k;
                count = 0;
                binC(i,j) = 0;
            end
            if abs(min - distMap(i,j,k)) < 10;
                count = count + 1;
            end
            if count >=3;
                binC(i,j) = 1;
            end
        end
    end
end


binTest = (bin == 5)

%binTest = bwmorph(binTest,'remove');

[row, column] = find(binTest, 1, 'first')
[allRows,allCols] = find(binTest == 1);

clear P;

startPoint = [row, column];
pointOnHull = startPoint;
figure(2);
imagesc(binTest)
notDone = true;

i = 1;
while(notDone)
    P(:,i) = pointOnHull;
    endPoint = [allRows(1), allCols(1)];
    for j = 1:length(allRows)
        temp = logical(leftOf(pointOnHull,endPoint,allRows(j),allCols(j),startPoint));
        if(endPoint == pointOnHull | temp )
            endPoint = [allRows(j), allCols(j)];
        end
        
    end
    pointOnHull = endPoint
    i = i + 1;
    if(pointOnHull(1) == startPoint(1) & pointOnHull(2) == startPoint(2))
        notDone = false;
    end
    
end

figure(3);
%temp = zeros(xSize,ySize);
%temp = reshape(bin(:,:,1),[xSize ySize]);
imagesc(binTest);
hold on;
for i = 1: length(P);
    plot(P(2,i),P(1,i),'r*');
end
hold off;


%%


figure(3);
temp = binC;
%temp = zeros(xSize,ySize);
%temp = reshape(bin(:,:,1),[xSize ySize]);
subplot(121);
imagesc(binC);
hold on;
for k = 1:numPoints;
    plot(points(2,k),points(1,k),'k*');
end
hold off;
subplot(122);
imagesc(bin);
hold on;
for k = 1:numPoints;
    plot(points(2,k),points(1,k),'k*');
end
hold off;




%%

figure(2);
subplot(321);
temp = zeros(xSize,ySize);
temp = reshape(bin(1,:,:),[xSize ySize]);
imagesc(temp);
%%
subplot(322);
temp = zeros(xSize,ySize);
temp = reshape(bin(10,:,:),[xSize ySize]);
imagesc(temp);

subplot(323);
temp = zeros(xSize,ySize);
temp = reshape(bin(:,1,:),[xSize ySize]);
imagesc(temp);

subplot(324);
temp = zeros(xSize,ySize);
temp = reshape(bin(:,10,:),[xSize ySize]);
imagesc(temp);

subplot(325);
temp = zeros(xSize,ySize);
temp = reshape(bin(:,:,1),[xSize ySize]);
imagesc(temp);


subplot(326);
temp = zeros(xSize,ySize);
temp = reshape(bin(:,:,10),[xSize ySize]);
imagesc(temp);



