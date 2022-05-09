/******************************************************************************
*                        ETSI TS 103 634 V1.3.1                               *
*              Low Complexity Communication Codec Plus (LC3plus)              *
*                                                                             *
* Copyright licence is solely granted through ETSI Intellectual Property      *
* Rights Policy, 3rd April 2019. No patent licence is granted by implication, *
* estoppel or otherwise.                                                      *
******************************************************************************/
                                                                               

/* guard against unindended includes */
#ifndef INCLUDED_FROM_IISFFT_C
#error "this file must not be included"
#endif

static void fft60(LC3_FLOAT* in)
{
    const LC3_INT  table1[] = {0,  45, 30, 15, 16, 1,  46, 31, 32, 17, 2,  47, 48, 33, 18, 3,  4,  49, 34, 19,
                              20, 5,  50, 35, 36, 21, 6,  51, 52, 37, 22, 7,  8,  53, 38, 23, 24, 9,  54, 39,
                              40, 25, 10, 55, 56, 41, 26, 11, 12, 57, 42, 27, 28, 13, 58, 43, 44, 29, 14, 59};
    const LC3_INT  table2[] = {0,  15, 30, 45, 4,  19, 34, 49, 8,  23, 38, 53, 12, 27, 42, 57, 16, 31, 46, 1,
                              20, 35, 50, 5,  24, 39, 54, 9,  28, 43, 58, 13, 32, 47, 2,  17, 36, 51, 6,  21,
                              40, 55, 10, 25, 44, 59, 14, 29, 48, 3,  18, 33, 52, 7,  22, 37, 56, 11, 26, 41};
    const LC3_INT  a        = 4;
    const LC3_INT  b        = 15;
    const LC3_INT  L        = 60;
    const LC3_INT* idx1     = table1;
    const LC3_INT* idx2     = table2;

    LC3_FLOAT temp[30], out[120];
    LC3_INT   k, l;

    for (k = 0; k < a; k++) {
        for (l = 0; l < b; l++) {
            temp[2 * l]     = in[2 * *idx1];
            temp[2 * l + 1] = in[2 * *idx1 + 1];
            idx1 += a;
        }

        fft15(temp); /* 15-point FFT */
        idx1 -= L;

        for (l = 0; l < b; l++) {
            in[2 * *idx1]     = temp[2 * l];
            in[2 * *idx1 + 1] = temp[2 * l + 1];
            idx1 += a;
        }
        idx1 -= L - 1;
    }

    idx1 -= a;

    for (k = 0; k < b; k++) {
        for (l = 0; l < a; l++) {
            temp[2 * l]     = in[2 * *idx1];
            temp[2 * l + 1] = in[2 * *idx1++ + 1];
        }

        fft4(temp); /* 4-point FFT */

        for (l = 0; l < a; l++) {
            out[2 * *idx2]       = temp[2 * l];
            out[2 * *idx2++ + 1] = temp[2 * l + 1];
        }
    }
    memmove(in, out, 2 * L * sizeof(LC3_FLOAT));
}

static void fft64(LC3_FLOAT* vec)
{
    const LC3_FLOAT w[] = {
        1.0000000000f,  0.9951847267f,  0.9807852804f,  0.9569403357f,  0.9238795325f,  0.8819212643f,  0.8314696123f,
        0.7730104534f,  0.7071067812f,  0.6343932842f,  0.5555702330f,  0.4713967368f,  0.3826834324f,  0.2902846773f,
        0.1950903220f,  0.0980171403f,  0.0000000000f,  -0.0980171403f, -0.1950903220f, -0.2902846773f, -0.3826834324f,
        -0.4713967368f, -0.5555702330f, -0.6343932842f, -0.7071067812f, -0.7730104534f, -0.8314696123f, -0.8819212643f,
        -0.9238795325f, -0.9569403357f, -0.9807852804f, -0.9951847267f, -1.0000000000f, -0.9951847267f, -0.9807852804f,
        -0.9569403357f, -0.9238795325f, -0.8819212643f, -0.8314696123f, -0.7730104534f, -0.7071067812f, -0.6343932842f,
        -0.5555702330f, -0.4713967368f, -0.3826834324f, -0.2902846773f, -0.1950903220f, -0.0980171403f};

    LC3_FLOAT temp1[64], temp2[64];
    LC3_INT   i;

    for (i = 0; i < 32; i++) {
        temp1[2 * i]     = vec[4 * i];
        temp1[2 * i + 1] = vec[4 * i + 1];
        temp2[2 * i]     = vec[4 * i + 2];
        temp2[2 * i + 1] = vec[4 * i + 3];
    }

    fft32(temp1);
    fft32(temp2);

    for (i = 0; i < 32; i++) {
        LC3_FLOAT re, im, wre, wim, tre, tim;

        re = temp2[2 * i];
        im = temp2[2 * i + 1];

        wre = w[i];
        wim = w[i + 16];

        tre = re * wre - im * wim;
        tim = re * wim + im * wre;

        vec[2 * i]      = temp1[2 * i] + tre;
        vec[2 * i + 1]  = temp1[2 * i + 1] + tim;
        vec[2 * i + 64] = temp1[2 * i] - tre;
        vec[2 * i + 65] = temp1[2 * i + 1] - tim;
    }
}

static void fft128(LC3_FLOAT* vec)
{
    const LC3_FLOAT w[] = {
        1.0000000000f,  0.9987954562f,  0.9951847267f,  0.9891765100f,  0.9807852804f,  0.9700312532f,  0.9569403357f,
        0.9415440652f,  0.9238795325f,  0.9039892931f,  0.8819212643f,  0.8577286100f,  0.8314696123f,  0.8032075315f,
        0.7730104534f,  0.7409511254f,  0.7071067812f,  0.6715589548f,  0.6343932842f,  0.5956993045f,  0.5555702330f,
        0.5141027442f,  0.4713967368f,  0.4275550934f,  0.3826834324f,  0.3368898534f,  0.2902846773f,  0.2429801799f,
        0.1950903220f,  0.1467304745f,  0.0980171403f,  0.0490676743f,  0.0000000000f,  -0.0490676743f, -0.0980171403f,
        -0.1467304745f, -0.1950903220f, -0.2429801799f, -0.2902846773f, -0.3368898534f, -0.3826834324f, -0.4275550934f,
        -0.4713967368f, -0.5141027442f, -0.5555702330f, -0.5956993045f, -0.6343932842f, -0.6715589548f, -0.7071067812f,
        -0.7409511254f, -0.7730104534f, -0.8032075315f, -0.8314696123f, -0.8577286100f, -0.8819212643f, -0.9039892931f,
        -0.9238795325f, -0.9415440652f, -0.9569403357f, -0.9700312532f, -0.9807852804f, -0.9891765100f, -0.9951847267f,
        -0.9987954562f, -1.0000000000f, -0.9987954562f, -0.9951847267f, -0.9891765100f, -0.9807852804f, -0.9700312532f,
        -0.9569403357f, -0.9415440652f, -0.9238795325f, -0.9039892931f, -0.8819212643f, -0.8577286100f, -0.8314696123f,
        -0.8032075315f, -0.7730104534f, -0.7409511254f, -0.7071067812f, -0.6715589548f, -0.6343932842f, -0.5956993045f,
        -0.5555702330f, -0.5141027442f, -0.4713967368f, -0.4275550934f, -0.3826834324f, -0.3368898534f, -0.2902846773f,
        -0.2429801799f, -0.1950903220f, -0.1467304745f, -0.0980171403f, -0.0490676743f,
    };

    LC3_FLOAT temp1[128], temp2[128];
    LC3_INT   i;

    for (i = 0; i < 64; i++) {
        temp1[2 * i]     = vec[4 * i];
        temp1[2 * i + 1] = vec[4 * i + 1];
        temp2[2 * i]     = vec[4 * i + 2];
        temp2[2 * i + 1] = vec[4 * i + 3];
    }

    fft64(temp1);
    fft64(temp2);

    for (i = 0; i < 64; i++) {
        LC3_FLOAT re, im, wre, wim, tre, tim;

        re = temp2[2 * i];
        im = temp2[2 * i + 1];

        wre = w[i];
        wim = w[i + 32];

        tre = re * wre - im * wim;
        tim = re * wim + im * wre;

        vec[2 * i]       = temp1[2 * i] + tre;
        vec[2 * i + 1]   = temp1[2 * i + 1] + tim;
        vec[2 * i + 128] = temp1[2 * i] - tre;
        vec[2 * i + 129] = temp1[2 * i + 1] - tim;
    }
}
