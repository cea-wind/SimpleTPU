function [out1] = convmxu(weight,feature,bias,index1,index2)
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here

out1 = zeros(14,14,32);
for i = 1:14
    for j = 1:14
        for k = 1:32
            for c = 1:32
                if(c==1)
                    out1(i,j,k) = bias(k) + weight(k,index1,index2,c)*feature(i,j,c);
                else
                    out1(i,j,k) = out1(i,j,k) + weight(k,index1,index2,c)*feature(i,j,c);
                end
            end
        end
    end
end

end

