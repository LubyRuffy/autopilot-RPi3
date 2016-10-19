
#include<math.h>

double dev[361];


double deviation(double bearing) {
	return bearing - dev[(int)rint(bearing)];
}


void initDev(void) {
	dev[0]=-15.30;
	dev[1]=-14.81;
	dev[2]=-14.32;
	dev[3]=-13.84;
	dev[4]=-13.36;
	dev[5]=-12.89;
	dev[6]=-12.42;
	dev[7]=-11.96;
	dev[8]=-11.50;
	dev[9]=-11.05;
	dev[10]=-10.60;
	dev[11]=-10.16;
	dev[12]=-9.72;
	dev[13]=-9.29;
	dev[14]=-8.87;
	dev[15]=-8.45;
	dev[16]=-8.03;
	dev[17]=-7.62;
	dev[18]=-7.22;
	dev[19]=-6.82;
	dev[20]=-6.42;
	dev[21]=-6.04;
	dev[22]=-5.65;
	dev[23]=-5.27;
	dev[24]=-4.90;
	dev[25]=-4.53;
	dev[26]=-4.17;
	dev[27]=-3.81;
	dev[28]=-3.46;
	dev[29]=-3.11;
	dev[30]=-2.77;
	dev[31]=-2.43;
	dev[32]=-2.10;
	dev[33]=-1.77;
	dev[34]=-1.45;
	dev[35]=-1.13;
	dev[36]=-0.82;
	dev[37]=-0.52;
	dev[38]=-0.22;
	dev[39]=0.08;
	dev[40]=0.37;
	dev[41]=0.66;
	dev[42]=0.94;
	dev[43]=1.21;
	dev[44]=1.48;
	dev[45]=1.75;
	dev[46]=2.01;
	dev[47]=2.26;
	dev[48]=2.51;
	dev[49]=2.76;
	dev[50]=3.00;
	dev[51]=3.23;
	dev[52]=3.46;
	dev[53]=3.69;
	dev[54]=3.91;
	dev[55]=4.12;
	dev[56]=4.33;
	dev[57]=4.54;
	dev[58]=4.74;
	dev[59]=4.93;
	dev[60]=5.12;
	dev[61]=5.31;
	dev[62]=5.49;
	dev[63]=5.66;
	dev[64]=5.83;
	dev[65]=6.00;
	dev[66]=6.16;
	dev[67]=6.32;
	dev[68]=6.47;
	dev[69]=6.62;
	dev[70]=6.76;
	dev[71]=6.90;
	dev[72]=7.03;
	dev[73]=7.16;
	dev[74]=7.28;
	dev[75]=7.40;
	dev[76]=7.51;
	dev[77]=7.62;
	dev[78]=7.72;
	dev[79]=7.82;
	dev[80]=7.92;
	dev[81]=8.01;
	dev[82]=8.09;
	dev[83]=8.18;
	dev[84]=8.25;
	dev[85]=8.32;
	dev[86]=8.39;
	dev[87]=8.46;
	dev[88]=8.51;
	dev[89]=8.57;
	dev[90]=8.62;
	dev[91]=8.66;
	dev[92]=8.71;
	dev[93]=8.74;
	dev[94]=8.78;
	dev[95]=8.80;
	dev[96]=8.83;
	dev[97]=8.85;
	dev[98]=8.86;
	dev[99]=8.87;
	dev[100]=8.88;
	dev[101]=8.88;
	dev[102]=8.88;
	dev[103]=8.88;
	dev[104]=8.87;
	dev[105]=8.86;
	dev[106]=8.84;
	dev[107]=8.82;
	dev[108]=8.79;
	dev[109]=8.76;
	dev[110]=8.73;
	dev[111]=8.69;
	dev[112]=8.65;
	dev[113]=8.60;
	dev[114]=8.56;
	dev[115]=8.50;
	dev[116]=8.45;
	dev[117]=8.39;
	dev[118]=8.32;
	dev[119]=8.26;
	dev[120]=8.18;
	dev[121]=8.11;
	dev[122]=8.03;
	dev[123]=7.95;
	dev[124]=7.86;
	dev[125]=7.77;
	dev[126]=7.68;
	dev[127]=7.58;
	dev[128]=7.48;
	dev[129]=7.38;
	dev[130]=7.28;
	dev[131]=7.17;
	dev[132]=7.05;
	dev[133]=6.94;
	dev[134]=6.82;
	dev[135]=6.69;
	dev[136]=6.57;
	dev[137]=6.44;
	dev[138]=6.31;
	dev[139]=6.17;
	dev[140]=6.03;
	dev[141]=5.89;
	dev[142]=5.75;
	dev[143]=5.60;
	dev[144]=5.45;
	dev[145]=5.30;
	dev[146]=5.14;
	dev[147]=4.99;
	dev[148]=4.82;
	dev[149]=4.66;
	dev[150]=4.49;
	dev[151]=4.32;
	dev[152]=4.15;
	dev[153]=3.98;
	dev[154]=3.80;
	dev[155]=3.62;
	dev[156]=3.44;
	dev[157]=3.26;
	dev[158]=3.07;
	dev[159]=2.88;
	dev[160]=2.69;
	dev[161]=2.50;
	dev[162]=2.30;
	dev[163]=2.10;
	dev[164]=1.90;
	dev[165]=1.70;
	dev[166]=1.49;
	dev[167]=1.29;
	dev[168]=1.08;
	dev[169]=0.87;
	dev[170]=0.66;
	dev[171]=0.44;
	dev[172]=0.23;
	dev[173]=0.01;
	dev[174]=-0.21;
	dev[175]=-0.43;
	dev[176]=-0.65;
	dev[177]=-0.88;
	dev[178]=-1.10;
	dev[179]=-1.33;
	dev[180]=-1.56;
	dev[181]=-1.79;
	dev[182]=-2.02;
	dev[183]=-2.25;
	dev[184]=-2.49;
	dev[185]=-2.72;
	dev[186]=-2.96;
	dev[187]=-3.19;
	dev[188]=-3.43;
	dev[189]=-3.67;
	dev[190]=-3.91;
	dev[191]=-4.16;
	dev[192]=-4.40;
	dev[193]=-4.64;
	dev[194]=-4.89;
	dev[195]=-5.13;
	dev[196]=-5.38;
	dev[197]=-5.62;
	dev[198]=-5.87;
	dev[199]=-6.12;
	dev[200]=-6.36;
	dev[201]=-6.61;
	dev[202]=-6.86;
	dev[203]=-7.11;
	dev[204]=-7.36;
	dev[205]=-7.61;
	dev[206]=-7.86;
	dev[207]=-8.11;
	dev[208]=-8.36;
	dev[209]=-8.61;
	dev[210]=-8.86;
	dev[211]=-9.11;
	dev[212]=-9.36;
	dev[213]=-9.61;
	dev[214]=-9.86;
	dev[215]=-10.11;
	dev[216]=-10.36;
	dev[217]=-10.61;
	dev[218]=-10.85;
	dev[219]=-11.10;
	dev[220]=-11.35;
	dev[221]=-11.60;
	dev[222]=-11.84;
	dev[223]=-12.09;
	dev[224]=-12.33;
	dev[225]=-12.57;
	dev[226]=-12.82;
	dev[227]=-13.06;
	dev[228]=-13.30;
	dev[229]=-13.54;
	dev[230]=-13.78;
	dev[231]=-14.02;
	dev[232]=-14.25;
	dev[233]=-14.49;
	dev[234]=-14.72;
	dev[235]=-14.95;
	dev[236]=-15.18;
	dev[237]=-15.41;
	dev[238]=-15.64;
	dev[239]=-15.86;
	dev[240]=-16.09;
	dev[241]=-16.31;
	dev[242]=-16.53;
	dev[243]=-16.75;
	dev[244]=-16.97;
	dev[245]=-17.18;
	dev[246]=-17.39;
	dev[247]=-17.60;
	dev[248]=-17.81;
	dev[249]=-18.02;
	dev[250]=-18.22;
	dev[251]=-18.42;
	dev[252]=-18.62;
	dev[253]=-18.82;
	dev[254]=-19.01;
	dev[255]=-19.20;
	dev[256]=-19.39;
	dev[257]=-19.58;
	dev[258]=-19.76;
	dev[259]=-19.94;
	dev[260]=-20.11;
	dev[261]=-20.29;
	dev[262]=-20.46;
	dev[263]=-20.63;
	dev[264]=-20.79;
	dev[265]=-20.95;
	dev[266]=-21.11;
	dev[267]=-21.26;
	dev[268]=-21.41;
	dev[269]=-21.56;
	dev[270]=-21.70;
	dev[271]=-21.84;
	dev[272]=-21.98;
	dev[273]=-22.11;
	dev[274]=-22.24;
	dev[275]=-22.36;
	dev[276]=-22.48;
	dev[277]=-22.60;
	dev[278]=-22.71;
	dev[279]=-22.82;
	dev[280]=-22.92;
	dev[281]=-23.02;
	dev[282]=-23.11;
	dev[283]=-23.20;
	dev[284]=-23.28;
	dev[285]=-23.36;
	dev[286]=-23.44;
	dev[287]=-23.51;
	dev[288]=-23.57;
	dev[289]=-23.64;
	dev[290]=-23.69;
	dev[291]=-23.74;
	dev[292]=-23.79;
	dev[293]=-23.82;
	dev[294]=-23.86;
	dev[295]=-23.89;
	dev[296]=-23.91;
	dev[297]=-23.93;
	dev[298]=-23.94;
	dev[299]=-23.95;
	dev[300]=-23.95;
	dev[301]=-23.94;
	dev[302]=-23.93;
	dev[303]=-23.91;
	dev[304]=-23.89;
	dev[305]=-23.86;
	dev[306]=-23.82;
	dev[307]=-23.78;
	dev[308]=-23.73;
	dev[309]=-23.67;
	dev[310]=-23.61;
	dev[311]=-23.54;
	dev[312]=-23.47;
	dev[313]=-23.40;
	dev[314]=-23.32;
	dev[315]=-23.25;
	dev[316]=-23.16;
	dev[317]=-23.08;
	dev[318]=-22.99;
	dev[319]=-22.90;
	dev[320]=-22.80;
	dev[321]=-22.70;
	dev[322]=-22.60;
	dev[323]=-22.50;
	dev[324]=-22.39;
	dev[325]=-22.27;
	dev[326]=-22.16;
	dev[327]=-22.03;
	dev[328]=-21.91;
	dev[329]=-21.78;
	dev[330]=-21.65;
	dev[331]=-21.51;
	dev[332]=-21.36;
	dev[333]=-21.22;
	dev[334]=-21.06;
	dev[335]=-20.91;
	dev[336]=-20.75;
	dev[337]=-20.58;
	dev[338]=-20.41;
	dev[339]=-20.23;
	dev[340]=-20.05;
	dev[341]=-19.87;
	dev[342]=-19.67;
	dev[343]=-19.48;
	dev[344]=-19.27;
	dev[345]=-19.07;
	dev[346]=-18.85;
	dev[347]=-18.63;
	dev[348]=-18.41;
	dev[349]=-18.17;
	dev[350]=-17.94;
	dev[351]=-17.69;
	dev[352]=-17.44;
	dev[353]=-17.19;
	dev[354]=-16.93;
	dev[355]=-16.66;
	dev[356]=-16.38;
	dev[357]=-16.10;
	dev[358]=-15.81;
	dev[359]=-15.51;
	dev[360]=-15.30;

}
