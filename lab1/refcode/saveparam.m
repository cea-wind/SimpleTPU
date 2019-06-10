function [] = saveparam(feature,weight,bias)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

feature = int8(feature);
weight = int8(weight);
bias = int32(bias);
bias4 = bitand(bitshift(bias,-24),int32(255));
bias3 = bitand(bitshift(bias,-16),int32(255));
bias2 = bitand(bitshift(bias,-8),int32(255));
bias1 = bitand(bias,int32(255));
fid = fopen('feature.dat','wb');
for i=1:14
    for j=1:14
        fwrite(fid,feature(i,j,:),'int8');
    end
end
fclose(fid);

fid = fopen('weight.dat','wb');
for k=1:32
    fwrite(fid,weight(:,2,2,k),'int8');
end
fwrite(fid,uint8(bias4),'uint8');
fwrite(fid,uint8(bias3),'uint8');
fwrite(fid,uint8(bias2),'uint8');
fwrite(fid,uint8(bias1),'uint8');
for i=1:3
    for j=1:3
        for k=1:32
            if(~(i==2&&j==2))
                fwrite(fid,weight(:,i,j,k),'int8');
            end
        end
        if(~(i==2&&j==2))
            for k=1:32
                fwrite(fid,0,'int32');
            end
        end
    end
end
fclose(fid);

end

