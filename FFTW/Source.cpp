#include <iostream>
#include <fftw3.h>
#include <vector>
#define PI	3.14159265358979323846
using namespace std;

#define REAL 0
#define IMAG 1

//1D Fast Fourier Transform
void fft(fftw_complex *in, fftw_complex *out, int count)
{
	fftw_plan plan = fftw_plan_dft_1d(count, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(plan);
	fftw_destroy_plan(plan);
	fftw_cleanup();
}

//1D Inverse Fast Fourier Transform
void ifft(fftw_complex *in, fftw_complex *out, int samples)
{
	fftw_plan plan = fftw_plan_dft_1d(samples, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
	fftw_execute(plan);
	fftw_destroy_plan(plan);
	fftw_cleanup();
	for (int i = 0; i < samples; i++)
	{
		out[i][REAL] /= samples;
		out[i][IMAG] /= samples;
	}
}

//displays complex numbers: +/- bi
void displayComplex(fftw_complex *y, int samples)
{
	for (int i = 0; i < samples; i++)
		if (y[i][IMAG] < 0)
			cout << fixed << y[i][REAL] << " -" << abs(y[i][IMAG]) << "i" << endl;
		else
			cout << fixed << y[i][REAL] << " +" << y[i][IMAG] << "i" << endl;
}

//displays real part of complex numbers
void displayReal(fftw_complex *y, int samples)
{
	for (int i = 0; i < samples; i++)
		cout << fixed << y[i][REAL] << endl;

}

//displays absolute spectrum for human
void displayAbsoluteSpectrum(fftw_complex *y, int samples, double samples_time_window)
{
	cout << endl << "Absolute Spectrum:" << endl;
	for (int i = 0; i < samples / 2; i++)
		cout << fixed << ((samples / samples_time_window) / samples)*i << " Hz: " << sqrt(y[i][REAL] * y[i][REAL] + y[i][IMAG] * y[i][IMAG]) / (samples / 2) << endl;
}

//displays absolute spectrum for human
void displayRelativeSpectrum(fftw_complex *y, const int samples, double samples_time_window)
{
	vector <double> RelativeSpectrum;

	for (int i = 0; i < samples / 2; i++)
		RelativeSpectrum.push_back(sqrt(y[i][REAL] * y[i][REAL] + y[i][IMAG] * y[i][IMAG]));

	double maximum = 0.0;
	for (int i = 0; i < samples / 2; i++)
	{
		if (RelativeSpectrum[i] > maximum)
			maximum = RelativeSpectrum[i];
	}

	for (int i = 0; i < samples / 2; i++)
	{
		RelativeSpectrum[i] /= maximum;
	}

	cout << endl << "Relative Spectrum:" << endl;
	for (int i = 0; i < samples / 2; i++)
		cout << fixed << ((samples / samples_time_window) / samples)*i << " Hz: " << RelativeSpectrum[i] << endl;
}

int main()
{
	const int samples = 50;//Number of samples in the buffer
	const double time = 0.5;//total time recorded in the buffer
	double sine_freq = 4;// just a generated sine frequency as an example
	double sine_freq_period = PI / (((double)samples / time) / sine_freq);//calculating sine wave sample period
	fftw_complex x[samples];
	fftw_complex y[samples];
	cout.precision(3);

	for (int i = 0; i < samples; i++)
	{
		//generating sine into the sampling buffer i.e. 1sine @4Hz, 2sine @8Hz, 0.5sine @16Hz
		x[i][REAL] = sin((double)(2)*(double)i*sine_freq_period) + 2 * sin((double)(2)*(double)i*sine_freq_period * 2) + 0.5* sin((double)(2)*(double)i*sine_freq_period * 4);
		x[i][IMAG] = 0;
	}

	/*fft(x, y, samples);
	cout << "FFT:" << endl;
	displayComplex(y, samples);

	ifft(x, y, samples);
	cout << endl << "IFFT:" << endl;
	displayReal(x, samples);*/

	fft(x, y, samples);

	//Displays Absolute Spectrum i.e. 1amplitude @4Hz, 2amplitude @8Hz, 0.5amplitude @16Hz
	displayAbsoluteSpectrum(y, samples, time);

	//Displays Relative Spectrum i.e. 0.5amplitude @4Hz, 1amplitude @8Hz, 0.25amplitude @16Hz
	displayRelativeSpectrum(y, samples, time);

	system("pause");
	return 0;
}