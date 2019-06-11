#include "tpu.h"
#include "stdio.h"
#include "stdlib.h"

int main(){
	PSUMDTYPE psum_buffer[512][MXU_COLNUM];
	FEATDTYPE unified_buffer[16384][MXU_ROWNUM];
	int norm_coef[MXU_COLNUM];
	RELPOOL_PARAM param;
	for(int i=0;i<14;i++){
		for(int j=0;j<14;j++){
			for(int c=0;c<32;c++){
				psum_buffer[i*14+j][c] = (i*14+j+c)*512;
			}
		}
	}
	for(int c=0;c<32;c++)
		norm_coef[c] = 1<<23;

	// no pooling
	param.isrelu = true;
	param.psum_raddr_start = 0;
	param.maxpool = true;
	param.pool_kw = 0;
	param.pool_kh = 0;
	param.pool_w = 14-1;
	param.pool_sw = 1;
	param.pool_sh = 1;
	param.pool_cnt = 14*14;
	param.pool_h_step = 14;
	param.ubuf_waddr_start = 0;
	param.ubuf_waddr_step1 = 1;
	param.ubuf_waddr_end1 = 14*14-1;
	relu_norm_pool(psum_buffer,unified_buffer,norm_coef,param);

	FEATDTYPE golden[14*14][MXU_ROWNUM];
	for(int i=0;i<14;i++){
		for(int j=0;j<14;j++){
			for (int k=0;k<32;k++){
				int tmp = psum_buffer[i*14+j][k]/512;
				tmp = tmp>127?127:tmp;
				tmp = tmp<-128?-128:tmp;
				golden[i*14+j][k] = tmp;
			}
		}
	}
	int err=0;
	for(int i=0;i<14*14;i++){
		for(int k=0;k<32;k++){
			if(golden[i][k]!=unified_buffer[i][k])
				err ++;
		}
	}

	// max pooling 2,2
	for(int c=0;c<32;c++)
		norm_coef[c] = 1<<23;
	param.isrelu = true;
	param.psum_raddr_start = 0;
	param.maxpool = true;
	param.pool_kw = 1;
	param.pool_kh = 1;
	param.pool_w = 12;
	param.pool_sw = 2;
	param.pool_sh = 2;
	param.pool_cnt = 14*14;
	param.pool_h_step = 14;
	param.ubuf_waddr_start = 0;
	param.ubuf_waddr_step1 = 1;
	param.ubuf_waddr_end1 = 7*7-1;
	relu_norm_pool(psum_buffer,unified_buffer,norm_coef,param);

	for(int i=0;i<7;i++){
		for(int j=0;j<7;j++){
			for (int k=0;k<32;k++){
				int tmp = -128;
				for(int i1=0;i1<2;i1++){
					for(int j1=0;j1<2;j1++){
						if(tmp<psum_buffer[(2*i+i1)*14+2*j+j1][k]/512)
							tmp = psum_buffer[(2*i+i1)*14+2*j+j1][k]/512;
					}
				}
				tmp = tmp>127?127:tmp;
				tmp = tmp<-128?-128:tmp;
				golden[i*7+j][k] = tmp;
			}
		}
	}
	for(int i=0;i<7*7;i++){
		for(int k=0;k<32;k++){
			if(golden[i][k]!=unified_buffer[i][k])
				err ++;
		}
	}

	for(int c=0;c<32;c++)
		norm_coef[c] = 171196;
	// avg pooling 7,7
	param.isrelu = true;
	param.psum_raddr_start = 0;
	param.maxpool = false;
	param.pool_kw = 6;
	param.pool_kh = 6;
	param.pool_w = 7;
	param.pool_sw = 7;
	param.pool_sh = 7;
	param.pool_cnt = 14*14;
	param.pool_h_step = 14;
	param.ubuf_waddr_start = 0;
	param.ubuf_waddr_step1 = 1;
	param.ubuf_waddr_end1 = 14*14-1;
	relu_norm_pool(psum_buffer,unified_buffer,norm_coef,param);

	for(int i=0;i<2;i++){
		for(int j=0;j<2;j++){
			for (int k=0;k<32;k++){
				int tmp = 0;
				for(int i1=0;i1<7;i1++){
					for(int j1=0;j1<7;j1++){
						tmp += psum_buffer[(i*7+i1)*14+7*j+j1][k];
					}
				}
				tmp = (long(tmp)*long(171196))>>32;
				tmp = tmp>127?127:tmp;
				tmp = tmp<-128?-128:tmp;
				golden[i*2+j][k] = tmp;
			}
		}
	}
	for(int i=0;i<2*2;i++){
		for(int k=0;k<32;k++){
			if(golden[i][k]!=unified_buffer[i][k])
				err ++;
		}
	}
	return err;
}
