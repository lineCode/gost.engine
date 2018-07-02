﻿const int	iSqrt[SQRT_TABLE_SIZE] = {
	3473408,	3440640,	3407872,	3407872,	3375104,	3342336,
	3342336,	3309568,	3276800,	3276800,	3244032,	3211264,
	3211264,	3178496,	3145728,	3145728,	3112960,	3080192,
	3080192,	3047424,	3014656,	3014656,	2981888,	2981888,
	2949120,	2916352,	2916352,	2883584,	2850816,	2850816,
	2818048,	2818048,	2785280,	2752512,	2752512,	2719744,
	2719744,	2686976,	2686976,	2654208,	2621440,	2621440,
	2588672,	2588672,	2555904,	2555904,	2523136,	2523136,
	2490368,	2457600,	2457600,	2424832,	2424832,	2392064,
	2392064,	2359296,	2359296,	2326528,	2326528,	2293760,
	2293760,	2260992,	2260992,	2228224,	2228224,	2195456,
	2195456,	2162688,	2162688,	2129920,	2129920,	2097152,
	2097152,	2064384,	2064384,	2031616,	2031616,	1998848,
	1998848,	1966080,	1966080,	1933312,	1933312,	1900544,
	1900544,	1867776,	1867776,	1867776,	1835008,	1835008,
	1802240,	1802240,	1769472,	1769472,	1736704,	1736704,
	1703936,	1703936,	1703936,	1671168,	1671168,	1638400,
	1638400,	1605632,	1605632,	1605632,	1572864,	1572864,
	1540096,	1540096,	1540096,	1507328,	1507328,	1474560,
	1474560,	1441792,	1441792,	1441792,	1409024,	1409024,
	1376256,	1376256,	1376256,	1343488,	1343488,	1343488,
	1310720,	1310720,	1277952,	1277952,	1277952,	1245184,
	1245184,	1212416,	1212416,	1212416,	1179648,	1179648,
	1179648,	1146880,	1146880,	1114112,	1114112,	1114112,
	1081344,	1081344,	1081344,	1048576,	1048576,	1048576,	
	1015808,	1015808,	1015808,	983040,		983040,		950272,	
	950272,		950272,		917504,		917504,		917504,		884736,
	884736,		884736,		851968,		851968,		851968,		819200,
	819200,		819200,		786432,		786432,		786432,		753664,
	753664,		753664,		720896,		720896,		720896,		688128,
	688128,		688128,		688128,		655360,		655360,		655360,
	622592,		622592,		622592,		589824,		589824,		589824,
	557056,		557056,		557056,		557056,		524288,		524288,
	524288,		491520,		491520,		491520,		458752,		458752,
	458752,		458752,		425984,		425984,		425984,		393216,
	393216,		393216,		393216,		360448,		360448,		360448,
	327680,		327680,		327680,		327680,		294912,		294912,
	294912,		262144,		262144,		262144,		262144,		229376,
	229376,		229376,		229376,		196608,		196608,		196608,
	163840,		163840,		163840,		163840,		131072,		131072,
	131072,		131072,		98304,		98304,		98304,		98304,
	65536,		65536,		65536,		65536,		32768,		32768,
	32768,		32768,		0,			0,			8355840,	8355840,
	8323072,	8290304,	8257536,	8224768,	8192000,	8159232,
	8126464,	8093696,	8060928,	8028160,	7995392,	7962624,
	7929856,	7897088,	7864320,	7864320,	7831552,	7798784,
	7766016,	7733248,	7700480,	7667712,	7634944,	7602176,
	7602176,	7569408,	7536640,	7503872,	7471104,	7438336,
	7405568,	7405568,	7372800,	7340032,	7307264,	7274496,
	7274496,	7241728,	7208960,	7176192,	7143424,	7143424,
	7110656,	7077888,	7045120,	7012352,	7012352,	6979584,
	6946816,	6914048,	6914048,	6881280,	6848512,	6815744,
	6815744,	6782976,	6750208,	6717440,	6717440,	6684672,
	6651904,	6619136,	6619136,	6586368,	6553600,	6553600,
	6520832,	6488064,	6455296,	6455296,	6422528,	6389760,
	6389760,	6356992,	6324224,	6324224,	6291456,	6258688,
	6258688,	6225920,	6193152,	6193152,	6160384,	6127616,
	6127616,	6094848,	6062080,	6062080,	6029312,	6029312,
	5996544,	5963776,	5963776,	5931008,	5898240,	5898240,
	5865472,	5865472,	5832704,	5799936,	5799936,	5767168,
	5767168,	5734400,	5701632,	5701632,	5668864,	5668864,
	5636096,	5603328,	5603328,	5570560,	5570560,	5537792,
	5505024,	5505024,	5472256,	5472256,	5439488,	5439488,
	5406720,	5406720,	5373952,	5341184,	5341184,	5308416,
	5308416,	5275648,	5275648,	5242880,	5242880,	5210112,
	5210112,	5177344,	5177344,	5144576,	5111808,	5111808,
	5079040,	5079040,	5046272,	5046272,	5013504,	5013504,
	4980736,	4980736,	4947968,	4947968,	4915200,	4915200,
	4882432,	4882432,	4849664,	4849664,	4816896,	4816896,
	4784128,	4784128,	4751360,	4751360,	4718592,	4718592,
	4685824,	4685824,	4685824,	4653056,	4653056,	4620288,
	4620288,	4587520,	4587520,	4554752,	4554752,	4521984,
	4521984,	4489216,	4489216,	4489216,	4456448,	4456448,
	4423680,	4423680,	4390912,	4390912,	4358144,	4358144,
	4358144,	4325376,	4325376,	4292608,	4292608,	4259840,
	4259840,	4227072,	4227072,	4227072,	4194304,	4194304,
	4161536,	4161536,	4161536,	4128768,	4128768,	4096000,
	4096000,	4063232,	4063232,	4063232,	4030464,	4030464,
	3997696,	3997696,	3997696,	3964928,	3964928,	3932160,
	3932160,	3932160,	3899392,	3899392,	3866624,	3866624,
	3866624,	3833856,	3833856,	3801088,	3801088,	3801088,
	3768320,	3768320,	3768320,	3735552,	3735552,	3702784,
	3702784,	3702784,	3670016,	3670016,	3670016,	3637248,
	3637248,	3604480,	3604480,	3604480,	3571712,	3571712,
	3571712,	3538944,	3538944,	3538944,	3506176,	3506176,
	3473408,	3473408
};