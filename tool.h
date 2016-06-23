#ifndef tool_h
#define tool_h

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <iomanip>
#include <string.h>
using namespace std;

double min(double a,double b) {
	return a > b ? b : a;
}

double bit2double(string data){
    double result=0;
    double tmp=0.5;
    for(int i=0;i<data.size();i++)
    {
        result+=((data.at(i)-'0')*tmp);
    }
    tmp+=0.5;
    return result;
}

double getLocationWithRatio(double a,double b,double ratios){
    return a+(b-a)*ratios;
}

int string2int(string a){
    double result=0;
    int i=0;
    for(i=0;i<a.size();i++){
        result*=10;
        result+=(a[i]-'0');
    }
    return result;
}
double string2double(string a){
    double result=0;
    int i=0;
    for(i=0;i<a.size()&&a[i]!='.';i++){
        result*=10;
        result+=(a[i]-'0');
    }
    double tmp=0.1;
    for(i=i+1;i<a.size();i++){
        result+=(tmp*(a[i]-'0'));
        tmp*=0.1;
    }
    return result;
}

char* int2time(int time){
    char *result=new char[50];
    int hour=time/3600;
    time%=3600;
    int minute=time/60;
    time%=60;
    int second=time;
    sprintf(result,"%02d:%02d:%02d",hour,minute,second);
    return result;
}

int GetNextChar(int x, int y, string next_path,int line_length)
{
	char a[10];
	sprintf(a, "%d", x / 50);
	string path = next_path;
	path += "/next";
	path += a;
	path += ".txt";
	FILE* fp = fopen(path.c_str(), "rb");
	if (!fp)
	{
		printf("Open file error!\n");
		system("pause");
		return -1;
	}
	fseek(fp, y + (x % 50) * line_length, 0);
	char c[5];
	fread(c, 1, 1, fp);
	int sum;
	sum = c[0] - '0';
	fclose(fp);
	return sum;
}
/*
double abs(double a){
return (a>=0)?a:-a;
}
*/
double getRad(double d)
{
	const double PI = 3.1415926535898;
	return d * PI / 180.0;
}

double calDistance(float fLati1, float fLong1, float fLati2, float fLong2)
{
	const double EARTH_RADIUS = 6378137.0;
	double radLat1 = getRad(fLati1);
	double radLat2 = getRad(fLati2);
	double a = radLat1 - radLat2;
	double b = getRad(fLong1) - getRad(fLong2);
	double tmp = sqrt(pow(sin(a / 2), 2) + cos(radLat1)*cos(radLat2)*pow(sin(b / 2), 2));
	if (tmp > 1 || tmp < -1)
		return 0;
	double s = 2 * asin(tmp);
	s = s * EARTH_RADIUS;
	return s;
}

int getDistance(double startLng, double startLat, double endLng, double endLat, double poiLng, double poiLat) {
	double longitude = (startLng + endLng) / 2;
	double latitude = (startLat + endLat) / 2;
	double dis = calDistance(poiLat, poiLng, latitude, longitude);
	if (dis <= 1.0)
	{
		return -1;
	}
	else if (calDistance(poiLat, poiLng, startLat, startLng)<calDistance(poiLat, poiLng, endLat, endLng))
	{
		return 0;
	}
	else return 1;
}
double vectorProduct(double p_lat, double p_lng, double s_lat, double s_lng, double e_lat, double e_lng) {
	double x1 = s_lat - p_lat;
	double y1 = s_lng - p_lng;
	double x2 = e_lat = p_lat;
	double y2 = e_lng - p_lng;
	return x1 * y2 - x2 * y1;
}
double scalarProduct(double p_lat,double p_lng,double s_lat, double s_lng, double e_lat, double e_lng) {
	double x1 = s_lat-p_lat;
	double y1 = s_lng-p_lng;
	double x2 = e_lat = p_lat;
	double y2 = e_lng-p_lng;
	return x1 * x2 + y1 * y2;
}
double getDistanceFromPointToSegment(double s_lat,double s_lng,double e_lat,double e_lng,double p_lat,double p_lng) {
	double result = min(calDistance(p_lat,p_lng,s_lat,s_lng), calDistance(p_lat, p_lng, e_lat, e_lng));
	if (scalarProduct(s_lat, s_lng, e_lat, e_lng, p_lat, p_lng)>0 &&
		scalarProduct(e_lat, e_lng, s_lat, s_lng, p_lat, p_lng) > 0) {
		result = min(result, fabs(vectorProduct(e_lat, e_lng, s_lat, s_lng, p_lat, p_lng) / calDistance(s_lat, s_lng, e_lat, e_lng)));
	}
	return result;
}

//convertor between char and binary char
union Char2Binary {
	char value;
	unsigned char data[1];
};
union Char2Binary char2Binary;

//convertor between int and binary int
union Int2Binary {
	int value;
	unsigned char data[4];
};
union Int2Binary int2Binary;

//convertor between double and binary double
union Double2Binary {
	double value;
	unsigned char data[8];
};
union Double2Binary double2Binary;

class FileReader {
public:
	FileReader(const char* filenamme, bool isbinary) {
		this->m_isbinary = isbinary;
		this->m_fileptr = fopen(filenamme, isbinary ? "rb" : "r");
	}
	FileReader(char* filenamme, char* options, bool isbinary) {
		this->m_isbinary = isbinary;
		this->m_fileptr = fopen(filenamme, options);
	}
	char GetNextChar() {
		if (this->m_isbinary) {
			if (fread(char2Binary.data, sizeof(unsigned char), 1, this->m_fileptr) < 1) return EOF;
			return char2Binary.value;
		}
		else {
			char temp;
			if (fscanf(this->m_fileptr, "%c", &temp) == EOF) {
				return EOF;
			}
			return temp;
		}
	}
	int GetNextInt() {
		if (this->m_isbinary) {
			if (fread(int2Binary.data, sizeof(unsigned char), 4, this->m_fileptr) < 4) return EOF;
			return int2Binary.value;
		}
		else {
			int temp;
			if (fscanf(this->m_fileptr, "%d", &temp) == EOF) {
				return EOF;
			}
			return temp;
		}
	}
	double GetNextDouble() {
		if (this->m_isbinary) {
			if (fread(double2Binary.data, sizeof(unsigned char), 8, this->m_fileptr) < 8) return EOF;
			return double2Binary.value;
		}
		else {
			double temp;
			if (fscanf(this->m_fileptr, "%lf", &temp) == EOF) {
				return EOF;
			}
			return temp;
		}
	}
	char* GetNextString() {
		char* temp = new char[256];
		if (this->m_isbinary) {
			int i = 0;
			while (fread(char2Binary.data, sizeof(unsigned char), 1, this->m_fileptr) > 0) {
				temp[i] = (char)char2Binary.value;
				if (temp[i++] == 0 || i > 254) {
					break;
				}
			}
			if (i == 0) {
				return temp;
			}
			temp[i] = 0;
			return temp;
		}
		else {
			if (fscanf(this->m_fileptr, "%s", temp) == EOF) {
				return temp;
			}
			return temp;
		}
	}
	bool IsBinary() {
		return this->m_isbinary;
	}
	void Close() {
		fclose(this->m_fileptr);
	}
	~FileReader() {
		fclose(this->m_fileptr);
	}
private:
	FILE* m_fileptr;
	bool m_isbinary;
};

class FileWriter {
public:
	FileWriter(const char* filename, bool isbinary) {
		this->m_isbinary = isbinary;
		this->m_fileptr = fopen(filename, isbinary ? "wb" : "w");
	}

	FileWriter(const char* filename, char* options, bool isbinary) {
		this->m_isbinary = isbinary;
		this->m_fileptr = fopen(filename, options);
	}
	void WriteChar(char value) {
		if (this->m_isbinary) {
			char2Binary.value = value;
			fwrite(char2Binary.data, sizeof(unsigned char), 1, this->m_fileptr);
		}
		else {
			fprintf(this->m_fileptr, "%c", value);
		}
	}
	void WriteInt(int value) {
		if (this->m_isbinary) {
			int2Binary.value = value;
			fwrite(int2Binary.data, sizeof(unsigned char), 4, this->m_fileptr);
		}
		else {
			fprintf(this->m_fileptr, "%d ", value);
		}
	}
	void WriteDouble(double value) {
		if (this->m_isbinary) {
			double2Binary.value = value;
			fwrite(double2Binary.data, sizeof(unsigned char), 8, this->m_fileptr);
		}
		else {
			fprintf(this->m_fileptr, "%lf ", value);
		}
	}
	bool IsBinary() {
		return this->m_isbinary;
	}
	void Close() {
		fclose(this->m_fileptr);
	}
	~FileWriter() {
		fclose(this->m_fileptr);
	}
private:
	FILE* m_fileptr;
	bool m_isbinary;
};

#endif
