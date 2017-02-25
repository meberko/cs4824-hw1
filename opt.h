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

        // Top Left
        if (__builtin_expect((r-1) >= 0 && (c-1) >= 0, 1)) {
            sum_R += in[ncols*(r-1)+(c-1)].R * filt[0];
            sum_G += in[ncols*(r-1)+(c-1)].G * filt[0];
            sum_B += in[ncols*(r-1)+(c-1)].B * filt[0];
        }

        // Top Center
        if(__builtin_expect((r-1) >= 0, 1)) {
            sum_R += in[ncols*(r-1)+c].R * filt[3];
            sum_G += in[ncols*(r-1)+c].G * filt[3];
            sum_B += in[ncols*(r-1)+c].B * filt[3];

        }

        // Top Right
        if(__builtin_expect((r-1) >= 0 && (c+1) < ncols, 1)) {
            sum_R += in[ncols*(r-1)+(c+1)].R * filt[6];
            sum_G += in[ncols*(r-1)+(c+1)].G * filt[6];
            sum_B += in[ncols*(r-1)+(c+1)].B * filt[6];
        }

        // Center Left
        if(__builtin_expect((c-1) >= 0, 1)) {
            sum_R += in[ncols*r+(c-1)].R * filt[1];
            sum_G += in[ncols*r+(c-1)].G * filt[1];
            sum_B += in[ncols*r+(c-1)].B * filt[1];
        }

        // Center
        sum_R += in[ncols*r+c].R * filt[4];
        sum_G += in[ncols*r+c].G * filt[4];
        sum_B += in[ncols*r+c].B * filt[4];

        // Center Right
        if(__builtin_expect((c+1) < ncols, 1)) {
            sum_R += in[ncols*r+(c+1)].R * filt[7];
            sum_G += in[ncols*r+(c+1)].G * filt[7];
            sum_B += in[ncols*r+(c+1)].B * filt[7];
        }

        // Bottom Left
        if(__builtin_expect((r+1) < nrows && (c-1) >= 0, 1)) {
            sum_R += in[ncols*(r+1)+(c-1)].R * filt[2];
            sum_G += in[ncols*(r+1)+(c-1)].G * filt[2];
            sum_B += in[ncols*(r+1)+(c-1)].B * filt[2];
        }

        // Bottom Center
        if(__builtin_expect((r+1) < nrows, 1)) {
            sum_R += in[ncols*(r+1)+c].R * filt[5];
            sum_G += in[ncols*(r+1)+c].G * filt[5];
            sum_B += in[ncols*(r+1)+c].B * filt[5];
        }

        // Bottom Right
        if(__builtin_expect((r+1) < nrows && (c+1) < ncols, 1)) {
            sum_R += in[ncols*(r+1)+(c+1)].R * filt[8];
            sum_G += in[ncols*(r+1)+(c+1)].G * filt[8];
            sum_B += in[ncols*(r+1)+(c+1)].B * filt[8];
        }

        out[ncols*r+c].R = (float) sum_R / filt[FILTER_BYTES-1];
        out[ncols*r+c].G = (float) sum_G / filt[FILTER_BYTES-1];
        out[ncols*r+c].B = (float) sum_B / filt[FILTER_BYTES-1];

        sum_R = 0;
        sum_G = 0;
        sum_B = 0;
        c = c - 1;

        // Top Left
        if (__builtin_expect((r-1) >= 0 && (c-1) >= 0, 1)) {
            sum_R += in[ncols*(r-1)+(c-1)].R * filt[0];
            sum_G += in[ncols*(r-1)+(c-1)].G * filt[0];
            sum_B += in[ncols*(r-1)+(c-1)].B * filt[0];
        }

        // Top Center
        if(__builtin_expect((r-1) >= 0, 1)) {
            sum_R += in[ncols*(r-1)+c].R * filt[3];
            sum_G += in[ncols*(r-1)+c].G * filt[3];
            sum_B += in[ncols*(r-1)+c].B * filt[3];

        }

        // Top Right
        if(__builtin_expect((r-1) >= 0 && (c+1) < ncols, 1)) {
            sum_R += in[ncols*(r-1)+(c+1)].R * filt[6];
            sum_G += in[ncols*(r-1)+(c+1)].G * filt[6];
            sum_B += in[ncols*(r-1)+(c+1)].B * filt[6];
        }

        // Center Left
        if(__builtin_expect((c-1) >= 0, 1)) {
            sum_R += in[ncols*r+(c-1)].R * filt[1];
            sum_G += in[ncols*r+(c-1)].G * filt[1];
            sum_B += in[ncols*r+(c-1)].B * filt[1];
        }

        // Center
        sum_R += in[ncols*r+c].R * filt[4];
        sum_G += in[ncols*r+c].G * filt[4];
        sum_B += in[ncols*r+c].B * filt[4];

        // Center Right
        if(__builtin_expect((c+1) < ncols, 1)) {
            sum_R += in[ncols*r+(c+1)].R * filt[7];
            sum_G += in[ncols*r+(c+1)].G * filt[7];
            sum_B += in[ncols*r+(c+1)].B * filt[7];
        }

        // Bottom Left
        if(__builtin_expect((r+1) < nrows && (c-1) >= 0, 1)) {
            sum_R += in[ncols*(r+1)+(c-1)].R * filt[2];
            sum_G += in[ncols*(r+1)+(c-1)].G * filt[2];
            sum_B += in[ncols*(r+1)+(c-1)].B * filt[2];
        }

        // Bottom Center
        if(__builtin_expect((r+1) < nrows, 1)) {
            sum_R += in[ncols*(r+1)+c].R * filt[5];
            sum_G += in[ncols*(r+1)+c].G * filt[5];
            sum_B += in[ncols*(r+1)+c].B * filt[5];
        }

        // Bottom Right
        if(__builtin_expect((r+1) < nrows && (c+1) < ncols, 1)) {
            sum_R += in[ncols*(r+1)+(c+1)].R * filt[8];
            sum_G += in[ncols*(r+1)+(c+1)].G * filt[8];
            sum_B += in[ncols*(r+1)+(c+1)].B * filt[8];
        }

        out[ncols*r+c].R = (float) sum_R / filt[FILTER_BYTES-1];
        out[ncols*r+c].G = (float) sum_G / filt[FILTER_BYTES-1];
        out[ncols*r+c].B = (float) sum_B / filt[FILTER_BYTES-1];
      }
    }
}