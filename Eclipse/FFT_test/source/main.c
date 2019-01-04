/*
 * main.c
 *
 *  Created on: 29. 12. 2018
 *      Author: vikto
 */



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>//system("pause");
#define PI 3.14159265358979323846


static void make_sintbl(int n, float sintbl[])
{
	int i, n2, n4, n8;
	double c, s, dc, ds, t;
	n2 = n / 2;  n4 = n / 4;  n8 = n / 8;
	t = sin(PI / n);
	dc = 2 * t * t;  ds = sqrt(dc * (2 - dc));
	t = 2 * dc;  c = sintbl[n4] = 1;  s = sintbl[0] = 0;
	for (i = 1; i < n8; i++) {
		c -= dc;  dc += t * c;
		s += ds;  ds -= t * s;
		sintbl[i] = (float)s;
		sintbl[n4 - i] = (float)c;
	}
	if (n8 != 0) sintbl[n8] = (float)sqrt(0.5);
	for (i = 0; i < n4; i++)
		sintbl[n2 - i] = sintbl[i];
	for (i = 0; i < n2 + n4; i++)
		sintbl[i + n2] = - sintbl[i];
}

static void make_bitrev(int n, int bitrev[])
{
	int i, j, k, n2;
	n2 = n / 2;  i = j = 0;
	for ( ; ; ) {
		bitrev[i] = j;
		if (++i >= n) break;
		k = n2;
		while (k <= j) {  j -= k;  k /= 2;  }
		j += k;
	}
}

int fft(int n, float x[], float y[])
{
	static int    last_n = 0;
	static int   *bitrev = (int*) NULL;
	static float *sintbl = (float*) NULL;
	int i, j, k, ik, h, d, k2, n4, inverse;
	float t, s, c, dx, dy;

	if (n < 0) {
		n = -n;  inverse = 1;
	} else inverse = 0;
	n4 = n / 4;
	if (n != last_n || n == 0) {
		last_n = n;
		if (sintbl != NULL) free((float*)sintbl);
		if (bitrev != NULL) free((int*)bitrev);
		if (n == 0) return 0;
		sintbl = (float*)malloc((n + n4) * sizeof(float));
		bitrev = (int*)malloc(n * sizeof(int));
		if (sintbl == NULL || bitrev == NULL) {
			fprintf(stderr, "no storage\n");  return 1;
		}
		make_sintbl(n, sintbl);
		make_bitrev(n, bitrev);
	}
	for (i = 0; i < n; i++) {
		j = bitrev[i];
		if (i < j) {
			t = x[i];  x[i] = x[j];  x[j] = t;
			t = y[i];  y[i] = y[j];  y[j] = t;
		}
	}
	for (k = 1; k < n; k = k2) {
		h = 0;  k2 = k + k;  d = n / k2;
		for (j = 0; j < k; j++) {
			c = sintbl[h + n4];
			if (inverse) s = - sintbl[h];
			else         s =   sintbl[h];
			for (i = j; i < n; i += k2) {
				ik = i + k;
				dx = s * y[ik] + c * x[ik];
				dy = c * y[ik] - s * x[ik];
				x[ik] = x[i] - dx;  x[i] += dx;
				y[ik] = y[i] - dy;  y[i] += dy;
			}
			h += d;
		}
	}
	if (! inverse)
		for (i = 0; i < n; i++) {  x[i] /= n;  y[i] /= n;  }
	return 0;
}


int main(void)
{
	const int samples = 64;//Number of samples in the buffer
	const double time = 0.5;//total time recorded in the buffer
	double sine_freq = 4;// just a generated sine frequency as an example
	double sine_freq_period = PI / (((double)samples / time) / sine_freq);//calculating sine wave sample period

	int i;
	float  x1[samples], x2[samples], y2[samples], res[samples];


	for(i=0;i<samples;i++)
	{
		x1[i]= sin((double)(2)*(double)i*sine_freq_period) + 2 * sin((double)(2)*(double)i*sine_freq_period * 2) + 0.5* sin((double)(2)*(double)i*sine_freq_period * 4);

	    x2[i]=x1[i];
        y2[i]=0;
	}

	//FFT
	if (fft(samples, x2, y2)) return EXIT_FAILURE;


	printf("\n Frequency              FFT\n");
	for (i = 0; i < samples/2; i++)
	{
	    res[i]=(float)sqrt(x2[i]*x2[i]+y2[i]*y2[i]);
	    printf(" %.2f Hz:            %6.3f\n", ((samples / time) / samples)*i, res[i]);
	}

	system("pause");
	return 0;
}
