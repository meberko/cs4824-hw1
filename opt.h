void conv_optt(uint nrows, uint ncols, pixel* in, char* filt, pixel* out) {
    for (int r = 0; r < nrows; r++) {
      for (int c = 0; c < ncols; c++) {
        uint sum_R = 0;
        uint sum_G = 0;
        uint sum_B = 0;

        uint x = 0;
        uint y = 0;
        uint f = 0;
        int cc0 = c - 1;
        int rr0 = r - 1;
        int cc1 = c;
        int rr1 = r;
        int cc2 = c + 1;
        int rr2 = r + 1;

        // Top Left
        if (rr0 >= 0 && cc0 >= 0) {
            x = ncols*rr0+cc0;
            y = 0;
            assert(x < ncols*nrows);
            assert(y < FILTER_BYTES);
            f = filt[y];
            sum_R += in[x].R * f;
            sum_G += in[x].G * f;
            sum_B += in[x].B * f;
        }

        // Center Left
        if(cc0 >= 0) {
            x = ncols*rr1+cc0;
            y = 1;
            assert(x < ncols*nrows);
            assert(y < FILTER_BYTES);
            f = filt[y];
            sum_R += in[x].R * f;
            sum_G += in[x].G * f;
            sum_B += in[x].B * f;
        }

        // Bottom Left
        if(rr2 < nrows && cc0 >= 0) {
            x = ncols*rr2+cc0;
            y = 2;
            assert(x < ncols*nrows);
            assert(y < FILTER_BYTES);
            f = filt[y];
            sum_R += in[x].R * f;
            sum_G += in[x].G * f;
            sum_B += in[x].B * f;
        }

        // Top Center
        if(rr0 >= 0) {
            x = ncols*rr0+cc1;
            y = 3;
            assert(x < ncols*nrows);
            assert(y < FILTER_BYTES);
            f = filt[y];
            sum_R += in[x].R * f;
            sum_G += in[x].G * f;
            sum_B += in[x].B * f;

        }

        // Center
        x = ncols*rr1+cc1;
        y = 4;
        assert(x < ncols*nrows);
        assert(y < FILTER_BYTES);
        f = filt[y];
        sum_R += in[x].R * f;
        sum_G += in[x].G * f;
        sum_B += in[x].B * f;

        // Bottom Center
        if(rr2 < nrows) {
            x = ncols*rr2+cc1;
            y = 5;
            assert(x < ncols*nrows);
            assert(y < FILTER_BYTES);
            f = filt[y];
            sum_R += in[x].R * f;
            sum_G += in[x].G * f;
            sum_B += in[x].B * f;
        }

        // Top Right
        if(rr0 >= 0 && cc2 < ncols) {
            x = ncols*rr0+cc2;
            y = 6;
            assert(x < ncols*nrows);
            assert(y < FILTER_BYTES);
            f = filt[y];
            sum_R += in[x].R * f;
            sum_G += in[x].G * f;
            sum_B += in[x].B * f;
        }

        // Center Right
        if(cc2 < ncols) {
            x = ncols*rr1+cc2;
            y = 7;
            assert(x < ncols*nrows);
            assert(y < FILTER_BYTES);
            f = filt[y];
            sum_R += in[x].R * f;
            sum_G += in[x].G * f;
            sum_B += in[x].B * f;
        }

        // Bottom Left
        if(rr2 < nrows && cc2 < ncols) {
            x = ncols*rr2+cc2;
            y = 8;
            assert(x < ncols*nrows);
            assert(y < FILTER_BYTES);
            f = filt[y];
            sum_R += in[x].R * f;
            sum_G += in[x].G * f;
            sum_B += in[x].B * f;
        }

        x = ncols*r+c;
        uchar d = filt[FILTER_BYTES-1];
        out[x].R = (float) sum_R / d;
        out[x].G = (float) sum_G / d;
        out[x].B = (float) sum_B / d;

      }
    }
}

conv_optt(nrows, ncols, in, filt, out);