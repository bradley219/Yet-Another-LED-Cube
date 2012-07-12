#ifndef _WINDOW_H_
#define _WINDOW_H_

.global tbl_window
tbl_window:	
#if FFT_N == 1024
	.dc.w 2621, 2621, 2622, 2623, 2625, 2628, 2631, 2635, 2639, 2644, 2649, 2655, 2662, 2669, 2677
	.dc.w 2685, 2694, 2703, 2713, 2723, 2734, 2746, 2758, 2771, 2784, 2798, 2813, 2828, 2843, 2859
	.dc.w 2876, 2893, 2911, 2929, 2948, 2968, 2988, 3008, 3030, 3051, 3073, 3096, 3120, 3144, 3168
	.dc.w 3193, 3218, 3245, 3271, 3298, 3326, 3354, 3383, 3412, 3442, 3473, 3504, 3535, 3567, 3600
	.dc.w 3633, 3666, 3701, 3735, 3770, 3806, 3842, 3879, 3917, 3954, 3993, 4031, 4071, 4111, 4151
	.dc.w 4192, 4233, 4275, 4318, 4361, 4404, 4448, 4492, 4537, 4583, 4629, 4675, 4722, 4769, 4817
	.dc.w 4866, 4915, 4964, 5014, 5064, 5115, 5166, 5218, 5270, 5323, 5376, 5429, 5483, 5538, 5593
	.dc.w 5648, 5704, 5760, 5817, 5874, 5932, 5990, 6049, 6108, 6167, 6227, 6287, 6348, 6409, 6471
	.dc.w 6533, 6595, 6658, 6721, 6785, 6849, 6914, 6979, 7044, 7110, 7176, 7242, 7309, 7376, 7444
	.dc.w 7512, 7581, 7649, 7719, 7788, 7858, 7929, 7999, 8070, 8142, 8214, 8286, 8358, 8431, 8504
	.dc.w 8578, 8652, 8726, 8801, 8875, 8951, 9026, 9102, 9178, 9255, 9332, 9409, 9487, 9564, 9642
	.dc.w 9721, 9800, 9879, 9958, 10037, 10117, 10198, 10278, 10359, 10440, 10521, 10603, 10684, 10767, 10849
	.dc.w 10931, 11014, 11097, 11181, 11264, 11348, 11432, 11517, 11601, 11686, 11771, 11856, 11942, 12027, 12113
	.dc.w 12199, 12286, 12372, 12459, 12546, 12633, 12720, 12808, 12895, 12983, 13071, 13160, 13248, 13336, 13425
	.dc.w 13514, 13603, 13692, 13782, 13871, 13961, 14050, 14140, 14230, 14321, 14411, 14501, 14592, 14682, 14773
	.dc.w 14864, 14955, 15046, 15137, 15229, 15320, 15411, 15503, 15595, 15686, 15778, 15870, 15962, 16054, 16146
	.dc.w 16238, 16330, 16423, 16515, 16607, 16699, 16792, 16884, 16977, 17069, 17162, 17254, 17347, 17439, 17532
	.dc.w 17625, 17717, 17810, 17902, 17995, 18087, 18180, 18272, 18365, 18457, 18550, 18642, 18735, 18827, 18919
	.dc.w 19011, 19104, 19196, 19288, 19380, 19472, 19564, 19656, 19747, 19839, 19931, 20022, 20114, 20205, 20296
	.dc.w 20387, 20478, 20569, 20660, 20751, 20841, 20932, 21022, 21112, 21203, 21293, 21382, 21472, 21562, 21651
	.dc.w 21740, 21829, 21918, 22007, 22096, 22184, 22272, 22361, 22449, 22536, 22624, 22711, 22798, 22885, 22972
	.dc.w 23059, 23145, 23232, 23318, 23403, 23489, 23574, 23659, 23744, 23829, 23913, 23998, 24082, 24165, 24249
	.dc.w 24332, 24415, 24498, 24580, 24662, 24744, 24826, 24908, 24989, 25070, 25150, 25230, 25311, 25390, 25470
	.dc.w 25549, 25628, 25706, 25785, 25863, 25940, 26018, 26095, 26171, 26248, 26324, 26399, 26475, 26550, 26625
	.dc.w 26699, 26773, 26847, 26920, 26993, 27066, 27138, 27210, 27282, 27353, 27424, 27495, 27565, 27634, 27704
	.dc.w 27773, 27842, 27910, 27978, 28045, 28112, 28179, 28245, 28311, 28377, 28442, 28507, 28571, 28635, 28698
	.dc.w 28761, 28824, 28886, 28948, 29009, 29070, 29131, 29191, 29250, 29310, 29368, 29427, 29485, 29542, 29599
	.dc.w 29656, 29712, 29767, 29823, 29877, 29932, 29986, 30039, 30092, 30144, 30196, 30248, 30299, 30349, 30399
	.dc.w 30449, 30498, 30546, 30595, 30642, 30689, 30736, 30782, 30828, 30873, 30918, 30962, 31006, 31049, 31091
	.dc.w 31134, 31175, 31216, 31257, 31297, 31337, 31376, 31414, 31453, 31490, 31527, 31564, 31600, 31635, 31670
	.dc.w 31704, 31738, 31772, 31804, 31837, 31869, 31900, 31930, 31961, 31990, 32019, 32048, 32076, 32103, 32130
	.dc.w 32156, 32182, 32208, 32232, 32256, 32280, 32303, 32326, 32348, 32369, 32390, 32410, 32430, 32449, 32468
	.dc.w 32486, 32503, 32520, 32537, 32553, 32568, 32583, 32597, 32610, 32623, 32636, 32648, 32659, 32670, 32680
	.dc.w 32690, 32699, 32707, 32715, 32723, 32729, 32736, 32741, 32746, 32751, 32755, 32758, 32761, 32764, 32765
	.dc.w 32766, 32767, 32767, 32766, 32765, 32764, 32761, 32758, 32755, 32751, 32746, 32741, 32736, 32729, 32723
	.dc.w 32715, 32707, 32699, 32690, 32680, 32670, 32659, 32648, 32636, 32623, 32610, 32597, 32583, 32568, 32553
	.dc.w 32537, 32520, 32503, 32486, 32468, 32449, 32430, 32410, 32390, 32369, 32348, 32326, 32303, 32280, 32256
	.dc.w 32232, 32208, 32182, 32156, 32130, 32103, 32076, 32048, 32019, 31990, 31961, 31930, 31900, 31869, 31837
	.dc.w 31804, 31772, 31738, 31704, 31670, 31635, 31600, 31564, 31527, 31490, 31453, 31414, 31376, 31337, 31297
	.dc.w 31257, 31216, 31175, 31134, 31091, 31049, 31006, 30962, 30918, 30873, 30828, 30782, 30736, 30689, 30642
	.dc.w 30595, 30546, 30498, 30449, 30399, 30349, 30299, 30248, 30196, 30144, 30092, 30039, 29986, 29932, 29877
	.dc.w 29823, 29767, 29712, 29656, 29599, 29542, 29485, 29427, 29368, 29310, 29250, 29191, 29131, 29070, 29009
	.dc.w 28948, 28886, 28824, 28761, 28698, 28635, 28571, 28507, 28442, 28377, 28311, 28245, 28179, 28112, 28045
	.dc.w 27978, 27910, 27842, 27773, 27704, 27634, 27565, 27495, 27424, 27353, 27282, 27210, 27138, 27066, 26993
	.dc.w 26920, 26847, 26773, 26699, 26625, 26550, 26475, 26399, 26324, 26248, 26171, 26095, 26018, 25940, 25863
	.dc.w 25785, 25706, 25628, 25549, 25470, 25390, 25311, 25230, 25150, 25070, 24989, 24908, 24826, 24744, 24662
	.dc.w 24580, 24498, 24415, 24332, 24249, 24165, 24082, 23998, 23913, 23829, 23744, 23659, 23574, 23489, 23403
	.dc.w 23318, 23232, 23145, 23059, 22972, 22885, 22798, 22711, 22624, 22536, 22449, 22361, 22272, 22184, 22096
	.dc.w 22007, 21918, 21829, 21740, 21651, 21562, 21472, 21382, 21293, 21203, 21112, 21022, 20932, 20841, 20751
	.dc.w 20660, 20569, 20478, 20387, 20296, 20205, 20114, 20022, 19931, 19839, 19747, 19656, 19564, 19472, 19380
	.dc.w 19288, 19196, 19104, 19011, 18919, 18827, 18735, 18642, 18550, 18457, 18365, 18272, 18180, 18087, 17995
	.dc.w 17902, 17810, 17717, 17625, 17532, 17439, 17347, 17254, 17162, 17069, 16977, 16884, 16792, 16699, 16607
	.dc.w 16515, 16423, 16330, 16238, 16146, 16054, 15962, 15870, 15778, 15686, 15595, 15503, 15411, 15320, 15229
	.dc.w 15137, 15046, 14955, 14864, 14773, 14682, 14592, 14501, 14411, 14321, 14230, 14140, 14050, 13961, 13871
	.dc.w 13782, 13692, 13603, 13514, 13425, 13336, 13248, 13160, 13071, 12983, 12895, 12808, 12720, 12633, 12546
	.dc.w 12459, 12372, 12286, 12199, 12113, 12027, 11942, 11856, 11771, 11686, 11601, 11517, 11432, 11348, 11264
	.dc.w 11181, 11097, 11014, 10931, 10849, 10767, 10684, 10603, 10521, 10440, 10359, 10278, 10198, 10117, 10037
	.dc.w 9958, 9879, 9800, 9721, 9642, 9564, 9487, 9409, 9332, 9255, 9178, 9102, 9026, 8951, 8875
	.dc.w 8801, 8726, 8652, 8578, 8504, 8431, 8358, 8286, 8214, 8142, 8070, 7999, 7929, 7858, 7788
	.dc.w 7719, 7649, 7581, 7512, 7444, 7376, 7309, 7242, 7176, 7110, 7044, 6979, 6914, 6849, 6785
	.dc.w 6721, 6658, 6595, 6533, 6471, 6409, 6348, 6287, 6227, 6167, 6108, 6049, 5990, 5932, 5874
	.dc.w 5817, 5760, 5704, 5648, 5593, 5538, 5483, 5429, 5376, 5323, 5270, 5218, 5166, 5115, 5064
	.dc.w 5014, 4964, 4915, 4866, 4817, 4769, 4722, 4675, 4629, 4583, 4537, 4492, 4448, 4404, 4361
	.dc.w 4318, 4275, 4233, 4192, 4151, 4111, 4071, 4031, 3993, 3954, 3917, 3879, 3842, 3806, 3770
	.dc.w 3735, 3701, 3666, 3633, 3600, 3567, 3535, 3504, 3473, 3442, 3412, 3383, 3354, 3326, 3298
	.dc.w 3271, 3245, 3218, 3193, 3168, 3144, 3120, 3096, 3073, 3051, 3030, 3008, 2988, 2968, 2948
	.dc.w 2929, 2911, 2893, 2876, 2859, 2843, 2828, 2813, 2798, 2784, 2771, 2758, 2746, 2734, 2723
	.dc.w 2713, 2703, 2694, 2685, 2677, 2669, 2662, 2655, 2649, 2644, 2639, 2635, 2631, 2628, 2625
	.dc.w 2623, 2622, 2621, 2621

#elif FFT_N == 512
	.dc.w 2621, 2622, 2625, 2631, 2639, 2649, 2662, 2677, 2694, 2713, 2735, 2759, 2785, 2813, 2844
	.dc.w 2877, 2912, 2949, 2989, 3030, 3074, 3121, 3169, 3220, 3272, 3327, 3385, 3444, 3505, 3569
	.dc.w 3635, 3703, 3773, 3845, 3919, 3995, 4074, 4154, 4237, 4321, 4408, 4496, 4587, 4679, 4774
	.dc.w 4870, 4968, 5069, 5171, 5275, 5381, 5489, 5598, 5710, 5823, 5938, 6055, 6174, 6294, 6416
	.dc.w 6540, 6666, 6793, 6922, 7052, 7184, 7318, 7453, 7590, 7728, 7868, 8009, 8152, 8296, 8442
	.dc.w 8589, 8737, 8887, 9038, 9190, 9344, 9499, 9655, 9812, 9971, 10131, 10292, 10454, 10617, 10781
	.dc.w 10946, 11112, 11280, 11448, 11617, 11787, 11958, 12130, 12302, 12476, 12650, 12825, 13001, 13177, 13355
	.dc.w 13532, 13711, 13890, 14069, 14250, 14430, 14611, 14793, 14975, 15158, 15341, 15524, 15707, 15891, 16075
	.dc.w 16260, 16444, 16629, 16814, 16999, 17184, 17370, 17555, 17740, 17926, 18111, 18296, 18481, 18666, 18851
	.dc.w 19036, 19220, 19405, 19589, 19772, 19956, 20139, 20321, 20504, 20686, 20867, 21048, 21228, 21408, 21588
	.dc.w 21766, 21945, 22122, 22299, 22475, 22650, 22825, 22999, 23172, 23344, 23516, 23686, 23856, 24024, 24192
	.dc.w 24359, 24525, 24689, 24853, 25015, 25177, 25337, 25496, 25654, 25811, 25967, 26121, 26274, 26426, 26576
	.dc.w 26725, 26873, 27019, 27164, 27308, 27450, 27590, 27729, 27867, 28003, 28137, 28270, 28401, 28531, 28659
	.dc.w 28785, 28910, 29033, 29154, 29274, 29391, 29507, 29622, 29734, 29845, 29953, 30060, 30165, 30268, 30370
	.dc.w 30469, 30566, 30662, 30755, 30847, 30936, 31024, 31109, 31193, 31274, 31354, 31431, 31506, 31579, 31650
	.dc.w 31719, 31786, 31851, 31913, 31974, 32032, 32088, 32142, 32194, 32243, 32291, 32336, 32379, 32419, 32458
	.dc.w 32494, 32528, 32560, 32589, 32617, 32642, 32664, 32685, 32703, 32719, 32733, 32744, 32753, 32760, 32764
	.dc.w 32767, 32767, 32764, 32760, 32753, 32744, 32733, 32719, 32703, 32685, 32664, 32642, 32617, 32589, 32560
	.dc.w 32528, 32494, 32458, 32419, 32379, 32336, 32291, 32243, 32194, 32142, 32088, 32032, 31974, 31913, 31851
	.dc.w 31786, 31719, 31650, 31579, 31506, 31431, 31354, 31274, 31193, 31109, 31024, 30936, 30847, 30755, 30662
	.dc.w 30566, 30469, 30370, 30268, 30165, 30060, 29953, 29845, 29734, 29622, 29507, 29391, 29274, 29154, 29033
	.dc.w 28910, 28785, 28659, 28531, 28401, 28270, 28137, 28003, 27867, 27729, 27590, 27450, 27308, 27164, 27019
	.dc.w 26873, 26725, 26576, 26426, 26274, 26121, 25967, 25811, 25654, 25496, 25337, 25177, 25015, 24853, 24689
	.dc.w 24525, 24359, 24192, 24024, 23856, 23686, 23516, 23344, 23172, 22999, 22825, 22650, 22475, 22299, 22122
	.dc.w 21945, 21766, 21588, 21408, 21228, 21048, 20867, 20686, 20504, 20321, 20139, 19956, 19772, 19589, 19405
	.dc.w 19220, 19036, 18851, 18666, 18481, 18296, 18111, 17926, 17740, 17555, 17370, 17184, 16999, 16814, 16629
	.dc.w 16444, 16260, 16075, 15891, 15707, 15524, 15341, 15158, 14975, 14793, 14611, 14430, 14250, 14069, 13890
	.dc.w 13711, 13532, 13355, 13177, 13001, 12825, 12650, 12476, 12302, 12130, 11958, 11787, 11617, 11448, 11280
	.dc.w 11112, 10946, 10781, 10617, 10454, 10292, 10131, 9971, 9812, 9655, 9499, 9344, 9190, 9038, 8887
	.dc.w 8737, 8589, 8442, 8296, 8152, 8009, 7868, 7728, 7590, 7453, 7318, 7184, 7052, 6922, 6793
	.dc.w 6666, 6540, 6416, 6294, 6174, 6055, 5938, 5823, 5710, 5598, 5489, 5381, 5275, 5171, 5069
	.dc.w 4968, 4870, 4774, 4679, 4587, 4496, 4408, 4321, 4237, 4154, 4074, 3995, 3919, 3845, 3773
	.dc.w 3703, 3635, 3569, 3505, 3444, 3385, 3327, 3272, 3220, 3169, 3121, 3074, 3030, 2989, 2949
	.dc.w 2912, 2877, 2844, 2813, 2785, 2759, 2735, 2713, 2694, 2677, 2662, 2649, 2639, 2631, 2625
	.dc.w 2622, 2621

#elif FFT_N == 256
	.dc.w 2621, 2625, 2639, 2662, 2694, 2735, 2785, 2845, 2913, 2990, 3076, 3171, 3275, 3388, 3509
	.dc.w 3639, 3777, 3924, 4079, 4243, 4414, 4594, 4782, 4977, 5181, 5391, 5610, 5835, 6068, 6308
	.dc.w 6555, 6808, 7069, 7335, 7608, 7887, 8172, 8463, 8759, 9061, 9368, 9680, 9997, 10319, 10645
	.dc.w 10975, 11310, 11648, 11990, 12336, 12684, 13036, 13391, 13748, 14108, 14469, 14833, 15199, 15566, 15934
	.dc.w 16303, 16673, 17044, 17415, 17787, 18158, 18529, 18900, 19270, 19638, 20006, 20372, 20737, 21100, 21460
	.dc.w 21819, 22175, 22528, 22878, 23225, 23569, 23909, 24246, 24578, 24907, 25230, 25550, 25864, 26174, 26478
	.dc.w 26777, 27071, 27359, 27641, 27917, 28187, 28450, 28707, 28957, 29201, 29437, 29666, 29888, 30103, 30310
	.dc.w 30509, 30701, 30885, 31060, 31228, 31387, 31538, 31681, 31815, 31941, 32058, 32166, 32265, 32356, 32438
	.dc.w 32510, 32574, 32629, 32674, 32711, 32738, 32757, 32766, 32766, 32757, 32738, 32711, 32674, 32629, 32574
	.dc.w 32510, 32438, 32356, 32265, 32166, 32058, 31941, 31815, 31681, 31538, 31387, 31228, 31060, 30885, 30701
	.dc.w 30509, 30310, 30103, 29888, 29666, 29437, 29201, 28957, 28707, 28450, 28187, 27917, 27641, 27359, 27071
	.dc.w 26777, 26478, 26174, 25864, 25550, 25230, 24907, 24578, 24246, 23909, 23569, 23225, 22878, 22528, 22175
	.dc.w 21819, 21460, 21100, 20737, 20372, 20006, 19638, 19270, 18900, 18529, 18158, 17787, 17415, 17044, 16673
	.dc.w 16303, 15934, 15566, 15199, 14833, 14469, 14108, 13748, 13391, 13036, 12684, 12336, 11990, 11648, 11310
	.dc.w 10975, 10645, 10319, 9997, 9680, 9368, 9061, 8759, 8463, 8172, 7887, 7608, 7335, 7069, 6808
	.dc.w 6555, 6308, 6068, 5835, 5610, 5391, 5181, 4977, 4782, 4594, 4414, 4243, 4079, 3924, 3777
	.dc.w 3639, 3509, 3388, 3275, 3171, 3076, 2990, 2913, 2845, 2785, 2735, 2694, 2662, 2639, 2625
	.dc.w 2621

#elif FFT_N == 128
	.dc.w 2621, 2639, 2695, 2787, 2915, 3080, 3280, 3516, 3786, 4091, 4428, 4798, 5200, 5632, 6094
	.dc.w 6584, 7102, 7645, 8213, 8804, 9417, 10050, 10702, 11371, 12055, 12753, 13464, 14184, 14914, 15650
	.dc.w 16391, 17135, 17880, 18626, 19368, 20107, 20840, 21565, 22281, 22985, 23677, 24354, 25014, 25657, 26280
	.dc.w 26882, 27462, 28018, 28548, 29052, 29528, 29975, 30393, 30779, 31133, 31454, 31741, 31994, 32212, 32395
	.dc.w 32542, 32652, 32726, 32762, 32762, 32726, 32652, 32542, 32395, 32212, 31994, 31741, 31454, 31133, 30779
	.dc.w 30393, 29975, 29528, 29052, 28548, 28018, 27462, 26882, 26280, 25657, 25014, 24354, 23677, 22985, 22281
	.dc.w 21565, 20840, 20107, 19368, 18626, 17880, 17135, 16391, 15650, 14914, 14184, 13464, 12753, 12055, 11371
	.dc.w 10702, 10050, 9417, 8804, 8213, 7645, 7102, 6584, 6094, 5632, 5200, 4798, 4428, 4091, 3786
	.dc.w 3516, 3280, 3080, 2915, 2787, 2695, 2639, 2621

#elif FFT_N == 64
	.dc.w 2621, 2696, 2920, 3291, 3804, 4456, 5240, 6147, 7169, 8296, 9516, 10817, 12187, 13612, 15077
	.dc.w 16568, 18070, 19568, 21048, 22495, 23893, 25230, 26493, 27668, 28743, 29709, 30556, 31274, 31858, 32301
	.dc.w 32599, 32748, 32748, 32599, 32301, 31858, 31274, 30556, 29709, 28743, 27668, 26493, 25230, 23893, 22495
	.dc.w 21048, 19568, 18070, 16568, 15077, 13612, 12187, 10817, 9516, 8296, 7169, 6147, 5240, 4456, 3804
	.dc.w 3291, 2920, 2696, 2621

#endif

#endif
