
#include "tpu.h"

void SetWeight(WEIGHTDTYPE weight[512][MXU_COLNUM],WEIGHTDTYPE weightreg[MXU_ROWNUM+4][MXU_COLNUM],
		short weight_raddr, bool enable){
	if(!enable)
		return;
	for(short i=weight_raddr;i<weight_raddr+4+MXU_ROWNUM;i++){
#pragma HLS PIPELINE
		for(int j=0;j<MXU_ROWNUM+4;j++){
			for(int k=0;k<MXU_COLNUM;k++){
				if(j!=MXU_ROWNUM+3)
					weightreg[j][k] = weightreg[j+1][k];
				else
					weightreg[j][k] = weight[i][k];
			}
		}
	}
}

void MacArray(FEATDTYPE ubuf[16384][MXU_ROWNUM],WEIGHTDTYPE weightreg[4+MXU_ROWNUM][MXU_COLNUM],
		PSUMDTYPE psum[512][MXU_COLNUM],MXU_PARAM mxuparam,bool enable){
	if(!enable)
		return;
	FEATDTYPE featreg[MXU_ROWNUM][MXU_COLNUM+MXU_ROWNUM-1];
	PSUMDTYPE psumreg[MXU_ROWNUM][MXU_COLNUM];
    short ubuf_raddr_p1=0;
    short ubuf_raddr_p2=0;
    short ubuf_raddr_p3=0;
    short psum_addr_p1[MXU_COLNUM];
    short psum_addr_p2[MXU_COLNUM];
    for(int i=0;i<MXU_COLNUM;i++){
#pragma HLS UNROLL
    	psum_addr_p1[i] = 0;
    	psum_addr_p2[i] = 0;
    }
	for(short i=0;i<mxuparam.ubuf_raddr_num+MXU_ROWNUM+MXU_COLNUM-2;i++){
#pragma HLS PIPELINE
    short ubuf_raddr = mxuparam.ubuf_raddr_start + ubuf_raddr_p1 + ubuf_raddr_p2 + ubuf_raddr_p3;
    if(ubuf_raddr_p1==mxuparam.ubuf_raddr_end1){
        ubuf_raddr_p1 = 0;
        if(ubuf_raddr_p2==mxuparam.ubuf_raddr_end2){
            ubuf_raddr_p2 = 0;
            ubuf_raddr_p3 = ubuf_raddr_p3 +  mxuparam.ubuf_raddr_step3;
        }
        else{
            ubuf_raddr_p2 = ubuf_raddr_p2 +  mxuparam.ubuf_raddr_step2;
        }
    }
    else{
        ubuf_raddr_p1 = ubuf_raddr_p1 + mxuparam.ubuf_raddr_step1;
    }

		for(int j=0;j<MXU_ROWNUM;j++){
			for(int k=MXU_ROWNUM+MXU_COLNUM-2;k>=0;k--){
				if(k>0)
					featreg[j][k] = featreg[j][k-1];
				else
					if(i<mxuparam.ubuf_raddr_num)
						featreg[j][k] = ubuf[ubuf_raddr][j];
					else
						featreg[j][k] = 0;
			}
		}

		for(int j=MXU_ROWNUM-1;j>=0;j--){
			for(int k=0;k<MXU_COLNUM;k++){
				ap_int<32> biasreg;
				biasreg(31,24)=weightreg[MXU_ROWNUM+0][k];
				biasreg(23,16)=weightreg[MXU_ROWNUM+1][k];
				biasreg(15, 8)=weightreg[MXU_ROWNUM+2][k];
				biasreg( 7, 0)=weightreg[MXU_ROWNUM+3][k];
				if(j==0)
					psumreg[j][k] = featreg[j][k+j]*weightreg[j][k] + biasreg;
				else
					psumreg[j][k] = featreg[j][k+j]*weightreg[j][k] + psumreg[j-1][k];
			}
		}
#pragma HLS DEPENDENCE variable=psum inter false
#pragma HLS DEPENDENCE variable=psum intra false
		for(int j=0;j<MXU_COLNUM;j++){
			if(i>=j+MXU_ROWNUM-1&&i<mxuparam.ubuf_raddr_num+j+MXU_ROWNUM-1){
				short psum_raddr = mxuparam.psum_start%512 + psum_addr_p1[j] + psum_addr_p2[j];
				if(psum_addr_p1[j]==mxuparam.psum_end1){
					psum_addr_p1[j] = 0;
					psum_addr_p2[j] = psum_addr_p2[j] + mxuparam.psum_step2;
				}
				else{
					psum_addr_p1[j] = psum_addr_p1[j] + mxuparam.psum_step1;
				}
				if(mxuparam.isfirstpsum)
					psum[psum_raddr][j] = psumreg[MXU_ROWNUM-1][j];
				else
					psum[psum_raddr][j] = psumreg[MXU_ROWNUM-1][j] + psum[psum_raddr][j];
			}
		}
	}
}

void MXU(FEATDTYPE ubuf[16384][MXU_ROWNUM],WEIGHTDTYPE weight[512][MXU_COLNUM],
		PSUMDTYPE psum[512][MXU_COLNUM],MXU_PARAM mxuparam, bool enable){
//#pragma HLS INTERFACE bram port=ubuf
//#pragma HLS INTERFACE bram port=weight
//#pragma HLS INTERFACE bram port=psum
//#pragma HLS DATA_PACK variable=mxuparam
//#pragma HLS ARRAY_PARTITION variable=ubuf complete dim=2
//#pragma HLS ARRAY_PARTITION variable=weight complete dim=2
//#pragma HLS ARRAY_PARTITION variable=psum complete dim=2

	static WEIGHTDTYPE weightreg1[4+MXU_ROWNUM][MXU_COLNUM];
	static WEIGHTDTYPE weightreg2[4+MXU_ROWNUM][MXU_COLNUM];
#pragma HLS ARRAY_PARTITION variable=weightreg1 complete dim=0
#pragma HLS ARRAY_PARTITION variable=weightreg2 complete dim=0

	if(!enable)
		return;
	if(mxuparam.isping){
		SetWeight(weight,weightreg1,mxuparam.weight_raddr,mxuparam.isload);
		MacArray(ubuf,weightreg2,psum,mxuparam,mxuparam.iscalc);
	}
	else{
		SetWeight(weight,weightreg2,mxuparam.weight_raddr,mxuparam.isload);
		MacArray(ubuf,weightreg1,psum,mxuparam,mxuparam.iscalc);
	}
}
