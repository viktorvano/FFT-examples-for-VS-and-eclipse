/*
 * main.c
 *
 *  Created on: 29. 12. 2018
 *      Author: vikto
 */


#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>

double PI;

void _fft(complex buf[], complex out[], int n, int step)
{
	if (step < n) {
		_fft(out, buf, n, step * 2);
		_fft(out + step, buf + step, n, step * 2);

		for (int i = 0; i < n; i += 2 * step) {
			complex t = cexp(-I * PI * i / n) * out[i + step];
			buf[i / 2]     = out[i] + t;
			buf[(i + n)/2] = out[i] - t;
		}
	}
}

void fft(complex buf[], int n)
{
	complex out[n];
	for (int i = 0; i < n; i++) out[i] = buf[i];

	_fft(buf, out, n, 1);
}


void show(const char * s, complex buf[], unsigned int samples) {
	printf("%s", s);
	for (int i = 0; i < samples; i++)
		if (!cimag(buf[i]))
			printf("%.3f \n", creal(buf[i]));
		else
			printf("(%.3f, %.3f) \n", creal(buf[i]), cimag(buf[i]));
}

void show_results(const char * s, complex buf[], unsigned int samples, float samples_time_window)
{
	float result[samples/2], maximum=0.0f, delta_f=((samples / samples_time_window) / samples);
	for(int i=0; i<samples/2; i++)
	{
		result[i]=sqrt(creal(buf[i])*creal(buf[i])+cimag(buf[i])*cimag(buf[i]));
	}

	for(int i=0; i<samples/2; i++)
	{
		if(result[i]>maximum)
			maximum=result[i];
	}

	for(int i=0; i<samples/2; i++)
	{
		result[i]/=maximum;
	}

	printf("%s", s);
	for (int i = 0; i < samples/2; i++)
			printf("%.3f Hz: %.3f \n", delta_f*i, result[i]);
}

int main()
{
	PI = atan2(1, 1) * 4;
	const int samples = 64;//Number of samples in the buffer
	const double time = 0.5;//total time recorded in the buffer
	double sine_freq = 4;// just a generated sine frequency as an example
	double sine_freq_period = PI / (((double)samples / time) / sine_freq);//calculating sine wave sample period
	complex buf[samples];// = {1, 1, 1, 1, 0, 0, 0, 0};

	for (int i = 0; i < samples; i++)
	{
		//generating sine into the sampling buffer i.e. 1sine @4Hz, 2sine @8Hz, 0.5sine @16Hz
		buf[i] = sin((double)(2)*(double)i*sine_freq_period) + 2 * sin((double)(2)*(double)i*sine_freq_period * 2) + 0.5* sin((double)(2)*(double)i*sine_freq_period * 4);
	}

	show("Data: \n", buf, samples);
	fft(buf, samples);
	show_results("\nFFT : \n", buf, samples, time);
	printf("\n");
	system("pause");
	return 0;
}
