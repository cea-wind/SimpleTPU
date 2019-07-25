#include "tpu.h"
#include "stdio.h"
int main(){
	ap_uint<256> *ddr;
	ap_uint<64> *ddr_instr;
	ddr = (ap_uint<256> *)malloc(sizeof(ap_uint<256>)*(16384));
	//512*25+72*25+72+512
	ddr_instr = (ap_uint<64> *)malloc(sizeof(ap_uint<64>)*3300);
	FILE *fid;
	fid = fopen("mlp_img.bin","rb");
	fread(ddr,32,25*512,fid);
	fclose(fid);
	fid = fopen("mlp_param.bin","rb");
	fread(ddr+512*25,32,25*72+72,fid);
	fclose(fid);
	fid = fopen("mlp_instr.bin","rb");
	ap_uint<64> *ddr_instr_r = ddr_instr;
	int cnt = 0;
	while(1==1){
		fread(ddr_instr_r,8,1,fid);
		ap_uint<64> tmp = *ddr_instr_r;
		if(tmp.range(55,55)==1)
			break;
		ddr_instr_r++;
		cnt++;
	}
	fclose(fid);
	tpu(ddr,ddr_instr);
	fid = fopen("golden_result.txt","r");
	int err = 0;
	for(int i=0;i<512;i++){
		ap_uint<256> val = ddr[512*25+72*25+72+i];
		int maxcof = -255;
		int idx = -1;
		int ref = -1;
		for(int j=0;j<16;j++){
			int cof = val(j*8+7,j*8);
			if(cof>127)
				cof = cof-256;
			if(cof>maxcof){
				maxcof = cof;
				idx = j;
			}
		}
		fscanf(fid,"%d",&ref);
		if(idx!=ref)
			err++;
	}
	return err;
}
