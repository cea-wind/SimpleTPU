
rng(0);
feature = randi([-128,127],14,14,32);
weight = randi([-128,127],32,3,3,32);
bias = randi([-1024,1023],1,32);
output = zeros(14,14,32);

saveparam(feature,weight,bias)

out1 = convmxu(weight,feature,bias,2,2);
out2 = convmxu(weight,feature,zeros(1,32),1,1);
out3 = convmxu(weight,feature,zeros(1,32),1,2);
out4 = convmxu(weight,feature,zeros(1,32),1,3);
out5 = convmxu(weight,feature,zeros(1,32),2,1);
out6 = convmxu(weight,feature,zeros(1,32),2,3);
out7 = convmxu(weight,feature,zeros(1,32),3,1);
out8 = convmxu(weight,feature,zeros(1,32),3,2);
out9 = convmxu(weight,feature,zeros(1,32),3,3);

output = out1;
output(2:end,2:end,:) = output(2:end,2:end,:) + out2(1:end-1,1:end-1,:);
output(2:end,:,:) = output(2:end,:,:) + out3(1:end-1,:,:);
output(2:end,1:end-1,:) = output(2:end,1:end-1,:) + out4(1:end-1,2:end,:);
output(:,2:end,:) = output(:,2:end,:) + out5(:,1:end-1,:);
output(:,1:end-1,:) = output(:,1:end-1,:) + out6(:,2:end,:);
output(1:end-1,2:end,:) = output(1:end-1,2:end,:) + out7(2:end,1:end-1,:);
output(1:end-1,:,:) = output(1:end-1,:,:) + out8(2:end,:,:);
output(1:end-1,1:end-1,:) = output(1:end-1,1:end-1,:) + out9(2:end,2:end,:);

golden = zeros(14,14,32);
for k = 1:32
   wk = reshape(weight(k,:,:,:),3,3,32);
   wk = wk(end:-1:1,end:-1:1,end:-1:1);
   tmp = convn(feature,wk,'same');
   golden(:,:,k) = tmp(:,:,16)+bias(k);
end
golden = int32(golden);
fid = fopen('golden.dat','wb');
for i=1:14
    for j=1:14
        fwrite(fid,golden(i,j,:),'int32');
    end
end
fclose(fid);