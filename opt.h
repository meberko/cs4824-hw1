uint image_bytes = nrows*ncols*sizeof(pixel);
char* id = alloc_filter();
id[0] = 0; id[1] = 0; id[2] = 0;
id[3] = 0; id[4] = 1; id[5] = 0;
id[6] = 0; id[7] = 0; id[8] = 0;
id[9] = 1;
if(memcmp(id, filt, FILTER_BYTES) == 0) memcpy(out, in, image_bytes);
else {
    for (int r = nrows-1; r >= 0; r--) {
      for (int c = ncols-1; c >= 0; c--) {
        uint sum_R = 0;
        uint sum_G = 0;
        uint sum_B = 0;

        uint x = 0;
        uint y = 0;
        uint f = 0;

        // Top Left
        if (__builtin_expect((r-1) >= 0 && (c-1) >= 0, 1)) {
            x = ncols*(r-1)+(c-1);
            y = 0;
            assert(x < ncols*nrows);
            assert(y < FILTER_BYTES);
            sum_R += in[x].R * filt[y];
            sum_G += in[x].G * filt[y];
            sum_B += in[x].B * filt[y];
        }

        // Top Center
        if(__builtin_expect((r-1) >= 0, 1)) {
            x = ncols*(r-1)+c;
            y = 3;
            assert(x < ncols*nrows);
            assert(y < FILTER_BYTES);
            sum_R += in[x].R * filt[y];
            sum_G += in[x].G * filt[y];
            sum_B += in[x].B * filt[y];

        }

        // Top Right
        if(__builtin_expect((r-1) >= 0 && (c+1) < ncols, 1)) {
            x = ncols*(r-1)+(c+1);
            y = 6;
            assert(x < ncols*nrows);
            assert(y < FILTER_BYTES);
            sum_R += in[x].R * filt[y];
            sum_G += in[x].G * filt[y];
            sum_B += in[x].B * filt[y];
        }

        // Center Left
        if(__builtin_expect((c-1) >= 0, 1)) {
            x = ncols*r+(c-1);
            y = 1;
            assert(x < ncols*nrows);
            assert(y < FILTER_BYTES);
            sum_R += in[x].R * filt[y];
            sum_G += in[x].G * filt[y];
            sum_B += in[x].B * filt[y];
        }

        // Center
        x = ncols*r+c;
        y = 4;
        assert(x < ncols*nrows);
        assert(y < FILTER_BYTES);
        sum_R += in[x].R * filt[y];
        sum_G += in[x].G * filt[y];
        sum_B += in[x].B * filt[y];

        // Center Right
        if(__builtin_expect((c+1) < ncols, 1)) {
            x = ncols*r+(c+1);
            y = 7;
            assert(x < ncols*nrows);
            assert(y < FILTER_BYTES);
            sum_R += in[x].R * filt[y];
            sum_G += in[x].G * filt[y];
            sum_B += in[x].B * filt[y];
        }

        // Bottom Left
        if(__builtin_expect((r+1) < nrows && (c-1) >= 0, 1)) {
            x = ncols*(r+1)+(c-1);
            y = 2;
            assert(x < ncols*nrows);
            assert(y < FILTER_BYTES);
            sum_R += in[x].R * filt[y];
            sum_G += in[x].G * filt[y];
            sum_B += in[x].B * filt[y];
        }

        // Bottom Center
        if(__builtin_expect((r+1) < nrows, 1)) {
            x = ncols*(r+1)+c;
            y = 5;
            assert(x < ncols*nrows);
            assert(y < FILTER_BYTES);
            sum_R += in[x].R * filt[y];
            sum_G += in[x].G * filt[y];
            sum_B += in[x].B * filt[y];
        }


        // Bottom Right
        if(__builtin_expect((r+1) < nrows && (c+1) < ncols, 1)) {
            x = ncols*(r+1)+(c+1);
            y = 8;
            assert(x < ncols*nrows);
            assert(y < FILTER_BYTES);
            sum_R += in[x].R * filt[y];
            sum_G += in[x].G * filt[y];
            sum_B += in[x].B * filt[y];
        }

        x = ncols*r+c;
        uchar d = filt[FILTER_BYTES-1];
        out[x].R = (float) sum_R / d;
        out[x].G = (float) sum_G / d;
        out[x].B = (float) sum_B / d;
      }
    }
}