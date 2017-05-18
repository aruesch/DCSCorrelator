// Correlator_try.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;
#define TIME_FLEX  0.1048576 //Time taken for acquiring each trace (Correlation v.s time-lag)
#define FIRSTDELAY   1E-7
typedef void(__cdecl* USBSTART)();
// autocorr = 0, cross correlation mode (AxB, BxA, CxD, DxC)  otherwise AxA BxB, CxC DxD.
typedef void(__cdecl* USBSTOP)(void);
typedef BYTE(__cdecl* USBINITIALIZE)(); // Returns 1 if successful, 0 if not.
typedef BYTE(__cdecl* USBUPDATE)(float *ElapsedTime, unsigned short int *NumOfIntensityElements, float *Correlation, float *Intensity);  // Return real time data
//ElapsedTime: Returns the time elapsed from calling USBStart function
//NumOfIntensityElements : The number of valid elements in the intensity series, each element is the number of photons integrated over 0.05767168 seconds
//Correlation: Normalized Correlation function for
//AxB : First 256 points
//BxA : The second 256 points
//CxD : The third 256 points
//DxC : The fourth 256 points
//IntensityA : first NumOfIntensityElements elements
//IntensityB : second NumOfIntensityElements elements
//IntensityC : third NumOfIntensityElements elements
//IntensityD : fourth NumOfIntensityElements elements

typedef BYTE(__cdecl* USBUPDATERAWDATA)(float *, unsigned short int *,
	double *, double *, double *, double *, float *);  // Return real time data, see example.
typedef void(__cdecl* USBFREE)(void); // Clean up.

int main()
{
	int					i, j, k;
	float				ElapsedTime;
	unsigned short int	tracecnt;
	char				filename[256], filenumber[5];
	float				corr[256 * 8];
	double				rawcorr[256 * 8];
	double				baseA[256 * 8];
	double				baseB[256 * 8];
	double				sample[256];
	float				DelayTime[256];
	float				trace[8 * 256];
	double				Intensity[8];
	FILE				*stream, *stream_corr;
	HINSTANCE hDLL;               // Handle to DLL
	USBSTART Start;    // Function pointer
	USBSTOP Stop;    // Function pointer
	USBINITIALIZE Initialize;    // Function pointer
	USBUPDATE Update;    // Function pointer
	USBUPDATERAWDATA UpdateRawdata;    // Function pointer
	USBFREE Usbfree;    // Function pointer
	//char outPath[255] = "corr results\\";
	char outPath[255] = "F:\\Results";
	char inName[255];
	cout << "directory name: ";
	cin.getline(inName, 255);
	strcat_s(outPath, inName);
	size_t newsize = strlen(outPath) + 1;
	wchar_t * wideName = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wideName, newsize, outPath, _TRUNCATE);
	CreateDirectoryW(wideName, NULL);

	//cout << "my directory is " << ExePath() << "\n";

	char sub[255];
	strcpy_s(sub, outPath);
	strcat_s(sub, "\\raw data");
	cout << sub;
	cin.get();
	size_t subsize = strlen(sub) + 1;
	wchar_t * wideSub = new wchar_t[subsize];
	//size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wideSub, subsize, sub, _TRUNCATE);
	CreateDirectoryW(wideSub, NULL);
	int					DurationTime;
	cout << "Duration Time: " << endl;
	cin >> DurationTime;
	//cin.ignore();
	memset(corr, 0, 2048 * sizeof(float));
	// Calculate the delay times
	for (i = 0; i < 16; i++) {
		DelayTime[i] = (i + 1)*FIRSTDELAY;
		
	}
	//cin.get();
	for (j = 1; j < 31; j++) {
		for (i = 0; i < 8; i++) {
			DelayTime[i + (j - 1) * 8 + 16] = DelayTime[(j - 1) * 8 + 16 + i - 1] + FIRSTDELAY*(float)pow(2, j);
			//cout << DelayTime[i+(j-1)*8+16] << "\n";
		}
	}
	
	//	hDLL = LoadLibrary("flex02LQOEMMT32x4cross.dll");
	hDLL = LoadLibrary(L"flex05oem8chauto100ns_win7.dll");
	
	cout << "ready ";
	//cin.get();
	cin.ignore();
	if (hDLL != NULL) {
		Initialize = (USBINITIALIZE)GetProcAddress(hDLL, "_USBInitialize");
		cout << "initialize: " << Initialize << "\n";
		if (!Initialize) {
			// handle the error
			FreeLibrary(hDLL);
			return 1;
		}
		Start = (USBSTART)GetProcAddress(hDLL, "_USBStart");
		cout << "start: " << Start << "\n";
		if (!Start) {
			// handle the error
			FreeLibrary(hDLL);
			return 1;
		}
		Stop = (USBSTOP)GetProcAddress(hDLL, "_USBStop");
		cout << "stop: " << Stop << "\n";
		if (!Stop) {
			// handle the error
			FreeLibrary(hDLL);
			return 1;
		}
		Update = (USBUPDATE)GetProcAddress(hDLL,"_USBUpdate");
		cout << "update: " << Update << "\n";
		if (!Update)
		{
			// handle the error
			FreeLibrary(hDLL);
			return 1;
		}
		UpdateRawdata = (USBUPDATERAWDATA)GetProcAddress(hDLL, "_USBUpdateRawdata");
		cout << "updaterawdata: " << UpdateRawdata << "\n";
		if (!UpdateRawdata)
		{
			// handle the error
			FreeLibrary(hDLL);
			return 1;
		}
		Usbfree = (USBFREE)GetProcAddress(hDLL, "_USBFree");
		cout << "usbfree: " << Usbfree << "\n";
		if (!Usbfree)
		{
			// handle the error
			FreeLibrary(hDLL);
			return 1;
		}
		// Test the presence of the correlator
		
		if (Initialize()==1) {
			cout << "The card is present" << endl;
		}
		else {
			cout << "The card is not present" << endl;
			return 1;
		}
		
		// starts the correlator in autocorrelation mode
		for (k = 0; k<10; k++)
		{
			strcpy_s(filename, outPath);
			strcat_s(filename, "\\intensity");
			_itoa_s(k, filenumber, 10);
			strcat_s(filename, filenumber);
			strcat_s(filename, ".dat");
			stream = fopen(filename, "wt");
			Start();
			ElapsedTime = 0;
			//runs for DurationTime seconds
			while (ElapsedTime < DurationTime)
			{
				// Sleep for a second
				//Sleep(1000);
				// Hello correlator
				//cout << "Elapsed time: " << ElapsedTime << "\n";
				//cout << "trace count: " << tracecnt << "\n";
				if (UpdateRawdata(&ElapsedTime, &tracecnt, rawcorr, sample, baseA, baseB, trace) == 0) {
					cout << "The card is disconnected" << endl;
					return 1;
				}
				cout << "Elapsed time" << ElapsedTime << "Trace count" << tracecnt;

				//for (int g = 0; g < 256; g++) {
				//	//cout << "sample: " << rawcorr[i] * sample[i] / baseA[i] / baseB[i] << "\n";
				//}
				for (i = 0; i<256; i++)
				{
					for (j = 0; j<8; j++)
					{
						//cout << baseA[i + j * 256] << "  " << baseB[i + j * 256] << "  " << rawcorr[i + j * 256];
						if ((baseA[i + j * 256] != 0) && (baseB[i + j * 256] != 0))
							corr[i + j * 256] = rawcorr[i + j * 256] * sample[i] / baseA[i + j * 256] / baseB[i + j * 256];
					}
				}
				// Calculate the average intensity for channel A
				cout << tracecnt << endl;
				for (i = 0; i<8; i++)
				{
					Intensity[i] = 0;
					if (tracecnt != 0)
					{
						for (j = 0; j<tracecnt; j++)
							Intensity[i] += trace[j + i*tracecnt];
						Intensity[i] /= tracecnt*TIME_FLEX;
					}
				}
				for (i = 0; i < tracecnt; i++) {
					for (j = 0; j < 8; j++) {
						fprintf(stream, "%e,", trace[i + tracecnt*j]);
					}
					fprintf(stream, "\n");
				}
				// Display it
				cout << "Intensities: " << Intensity[0] << " ," << Intensity[1] << " ," << Intensity[2] << " ," << Intensity[3] << endl;
				cout << "Intensities: " << Intensity[4] << " ," << Intensity[5] << " ," << Intensity[6] << " ," << Intensity[7] << endl;
			}//End of while loop (elapsed time within Duration time (as set by user)

			fclose(stream);
			//stops the correlator
			cout << "ready to stop "<<k+1<<"\n";
			cin.get();
			Stop();
			//Clean up

			//File for storing all correlation data
			strcpy_s(filename, outPath);
			strcat_s(filename, "\\corr");
			strcat_s(filename, ".txt");
			stream_corr = fopen(filename, "a");
			fprintf(stream_corr, "time = %d \n", k);

			//strcpy_s(filename, outPath);
			//strcat_s(filename, "\\corr");
			//_itoa_s(k, filenumber, 10);
			//strcat_s(filename, filenumber);
			//strcat_s(filename, ".dat");
			//stream = fopen(filename, "wt");
			for (i = 0; i<256; i++)
			{
				fprintf(stream_corr, "%e,", DelayTime[i]);
				//cout << DelayTime[i] << " ";
				for (j = 0; j < 8; j++) {
					fprintf(stream_corr, "%e,", corr[i + 256 * j]);
					//cout << corr[i + 256 * j] << " ";
				}
				fprintf(stream_corr, "\n");
				//cout << "\n";
			}

			fclose(stream_corr);

			//fclose(stream);
			strcpy_s(filename, sub);
			strcat_s(filename, "\\rawcorra");
			_itoa_s(k, filenumber, 10);
			strcat_s(filename, filenumber);
			strcat_s(filename, ".dat");
			stream = fopen(filename, "wt");
			for (i = 0; i<256; i++)
				fprintf(stream, "%e,%e, %e, %e, %e, %e\n", DelayTime[i], rawcorr[i], sample[i], baseA[i], baseB[i], corr[i]);
			fclose(stream);

			strcpy_s(filename, sub);
			strcat_s(filename, "\\rawcorrb");
			_itoa_s(k, filenumber, 10);
			strcat_s(filename, filenumber);
			strcat_s(filename, ".dat");
			stream = fopen(filename, "wt");
			for (i = 0; i<256; i++)
				fprintf(stream, "%e,%e, %e, %e, %e, %e\n", DelayTime[i], rawcorr[i + 256], sample[i], baseA[i + 256], baseB[i + 256], corr[i + 256]);
			fclose(stream);
			strcpy_s(filename, sub);
			strcat_s(filename, "\\rawcorrc");
			_itoa_s(k, filenumber, 10);
			strcat_s(filename, filenumber);
			strcat_s(filename, ".dat");
			stream = fopen(filename, "wt");
			for (i = 0; i<256; i++)
				fprintf(stream, "%e,%e, %e, %e, %e, %e\n", DelayTime[i], rawcorr[i + 256 * 2], sample[i], baseA[i + 256 * 2], baseB[i + 256 * 2], corr[i + 256 * 2]);
			fclose(stream);
			strcpy_s(filename, sub);
			strcat_s(filename, "\\rawcorrd");
			_itoa_s(k, filenumber, 10);
			strcat_s(filename, filenumber);
			strcat_s(filename, ".dat");
			stream = fopen(filename, "wt");
			for (i = 0; i<256; i++)
				fprintf(stream, "%e,%e, %e, %e, %e, %e\n", DelayTime[i], rawcorr[i + 256 * 3], sample[i], baseA[i + 256 * 3], baseB[i + 256 * 3], corr[i + 256 * 3]);
			fclose(stream);
			strcpy_s(filename, sub);
			strcat_s(filename, "\\rawcorre");
			_itoa_s(k, filenumber, 10);
			strcat_s(filename, filenumber);
			strcat_s(filename, ".dat");
			stream = fopen(filename, "wt");
			for (i = 0; i<256; i++)
				fprintf(stream, "%e,%e, %e, %e, %e, %e\n", DelayTime[i], rawcorr[i + 256 * 4], sample[i], baseA[i + 256 * 4], baseB[i + 256 * 4], corr[i + 256 * 4]);
			fclose(stream);
			strcpy_s(filename, sub);
			strcat_s(filename, "\\rawcorrf");
			_itoa_s(k, filenumber, 10);
			strcat_s(filename, filenumber);
			strcat_s(filename, ".dat");
			stream = fopen(filename, "wt");
			for (i = 0; i<256; i++)
				fprintf(stream, "%e,%e, %e, %e, %e, %e\n", DelayTime[i], rawcorr[i + 256 * 5], sample[i], baseA[i + 256 * 5], baseB[i + 256 * 5], corr[i + 256 * 5]);
			fclose(stream);
			strcpy_s(filename, sub);
			strcat_s(filename, "\\rawcorrg");
			_itoa_s(k, filenumber, 10);
			strcat_s(filename, filenumber);
			strcat_s(filename, ".dat");
			stream = fopen(filename, "wt");
			for (i = 0; i<256; i++)
				fprintf(stream, "%e,%e, %e, %e, %e, %e\n", DelayTime[i], rawcorr[i + 256 * 6], sample[i], baseA[i + 256 * 6], baseB[i + 256 * 6], corr[i + 256 * 6]);
			fclose(stream);
			strcpy_s(filename, sub);
			strcat_s(filename, "\\rawcorrh");
			_itoa_s(k, filenumber, 10);
			strcat_s(filename, filenumber);
			strcat_s(filename, ".dat");
			stream = fopen(filename, "wt");
			for (i = 0; i<256; i++)
				fprintf(stream, "%e,%e, %e, %e, %e, %e\n", DelayTime[i], rawcorr[i + 256 * 7], sample[i], baseA[i + 256 * 7], baseB[i + 256 * 7], corr[i + 256 * 7]);
			fclose(stream);
		}
		Usbfree();
		FreeLibrary(hDLL);
		cout << "all done";
		cin.get();
		// Display the final correlation function
	}
	else {
		cout << "DLL 'flex05oem8chauto100ns_win7.dll' not found. Abort.";
	}

	// Wait for any key input
	system("pause");
	return 0;
}

