//
// Forex forecasting tool by using FANN library
// Copyright (C) 2008  F. Volkan YALÇINER
// http://www.globalekonomi.com/
// fvolkany@globalekonomi.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>
//
 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fann.h"
#include "floatfann.h"
 
int main()
{
 
    const unsigned int num_input = 10; // Girilen veri adedi.
    const unsigned int num_output = 1; // Bu verilerin karşılığında alınan sonuç adedi.
    const unsigned int num_layers = 3; // Katman sayısı.
    const unsigned int num_neurons_hidden = 20; // Gizli.
    const float desired_error = (const float) 0.0001; // Kabul edilebilir hata
    const unsigned int max_epochs = 2000; // Test adedi.
    const unsigned int epochs_between_reports = 20; // Test sıklığı.
 
    fann_type *calc_out; // Tahmin burada
    fann_type input[10]; // Tahmin etmek için gerekli 10 giriş.
 
    FILE *fp; // Aynı anda birden fazla dosya ile işlem yapmaya gerek yok.
 
 
    int yyyy,mm,dd,hh,mi,volume; // Kullanılmayacak dizi yardımcıları.
    int dYear[60000],dMonth[6000],dDay[6000],dHour[6000],dMinute[6000],dVolume[6000]; // Bunlar da kullanılmıyor.
    float dOpen[6000],dHigh[6000],dLow[6000],dClose[6000]; // .csv dosyadan çekilecek sütunlar için diziler.
    float open,high,low,close; // OHLC indeksleme yardımcıları.
    int x=0; // Satır sayısı belirtecinin ilklendirilmesi
    int nr; // Normalleştirme döngüsü için.
    float normOpen[6000],normHigh[6000],normLow[6000],normClose[6000];
    float PPoint=10000; // EURUSD paritesinde en küçük fiyat hareketi 0.0001 = 1 pip.
    float PMax=300; // İki veri arasındaki en büyük fark ( 10 - 20 pip daha fazlası ).
    int train =10; // .data dosyasındaki giriş adedi.
    int bos; // .dat dosyasındaki ilk satırları 0 doldurmak için.
    int z; // dizilere veri doldurmak için.
    float d1,d2,d3,d4,d5,d6,d7,d8,d9,d10; // .dat dosyasındaki girişler...
    float d11; // ve çıkış.
    float tahmin; // Bir sonraki değeri tahmin eden çıktı için.
 
    // MetaTrade dosyasını oku, O,H,L,C datayı dizilere at
    fp=fopen("EURUSDm1440.csv","r");
    while (fscanf(fp, "%d%*c%d%*c%d%*c%d%*c%d%*c%f%*c%f%*c%f%*c%f%*c%d",&yyyy,&mm,&dd,&hh,&mi,&open,&high,&low,&close,&volume) != EOF)
    {
        dYear[x]=yyyy;
        dMonth[x]=mm;
        dDay[x]=dd;
        dHour[x]=hh;
        dMinute[x]=mi;
        dOpen[x]=open;
        dHigh[x]=high;
        dLow[x]=low;
        dClose[x]=close;
        dVolume[x]=volume;
        x++;
    }
    fclose(fp);
 
    // Elimizde kaç satır data var?
    printf("MetaTrader dosyasindan %d satir H ve L datasi dizilere yerlestirildi.\n",x);
 
 
    // Normalleştirme işleminde...
    // Her satırda aktif veri ile bir önceki veri arasındaki fark kullanılıyor.
    // rastgele_veri[n]'nin karşılığı, bir önceki veriye göre normalleştirilmiş_fark[n] oluyor.
    for(nr=1;nr<=x-1;nr++) normHigh[nr]=((dHigh[nr]-dHigh[nr-1])*PPoint)/PMax;
    for(nr=1;nr<=x-1;nr++) normLow[nr]=((dLow[nr]-dLow[nr-1])*PPoint)/PMax;
 
    // İlk veri için elimizde bir önceki veri olmadığından...
    normHigh[0]=0;
    normLow[0]=0;
 
    // Ara sonuç ekrana yazdırılıyor.
    printf("H ve L datalari normallestirildi.\n");
 
    // Normalleştirilmiş verileri kullanarak .dat dosyaları hazırlanıyor.
 
    fp=fopen("high.dat","w+");
    fprintf(fp,"%d %d 1\n", x-10, train);
    //for (bos=0;bos<10;bos++) fprintf(fp,"0 0 0 0 0 0 0 0 0 0\n0\n");
    for (z=10;z<=x-1;z++)
    {
        d1=normHigh[z-10];
        d2=normHigh[z-9];
        d3=normHigh[z-8];
        d4=normHigh[z-7];
        d5=normHigh[z-6];
        d6=normHigh[z-5];
        d7=normHigh[z-4];
        d8=normHigh[z-3];
        d9=normHigh[z-2];
        d10=normHigh[z-1];
        d11=normHigh[z];
        fprintf(fp,"%f %f %f %f %f %f %f %f %f %f\n",d1,d2,d3,d4,d5,d6,d7,d8,d9,d10);
        fprintf(fp,"%f\n",d11);
    }
    fclose(fp);
 
    fp=fopen("low.dat","w+");
    fprintf(fp,"%d %d 1\n", x-10, train);
    //for (bos=0;bos<10;bos++) fprintf(fp,"0 0 0 0 0 0 0 0 0 0\n0\n");
    for (z=10;z<=x-1;z++)
    {
        d1=normLow[z-10];
        d2=normLow[z-9];
        d3=normLow[z-8];
        d4=normLow[z-7];
        d5=normLow[z-6];
        d6=normLow[z-5];
        d7=normLow[z-4];
        d8=normLow[z-3];
        d9=normLow[z-2];
        d10=normLow[z-1];
        d11=normLow[z];
        fprintf(fp,"%f %f %f %f %f %f %f %f %f %f\n",d1,d2,d3,d4,d5,d6,d7,d8,d9,d10);
        fprintf(fp,"%f\n",d11);
    }
    fclose(fp);
 
    fp=fopen("high.net","a+");
    fclose(fp);
    fp=fopen("low.net","a+");
    fclose(fp);
 
    // .dat ve şimdilik boş olan .net dosyaları kaydedildi.
    printf("H ve L icin .dat ve .net dosyalari kaydedildi\n");
 
 
    // Yapay zekayı eğitme çalışması
    struct fann *ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);
 
    fann_set_activation_function_hidden(ann, FANN_GAUSSIAN);
    fann_set_activation_function_output(ann, FANN_COS_SYMMETRIC);
    fann_train_on_file(ann, "high.dat", max_epochs, epochs_between_reports, desired_error);
    fann_save(ann, "high.net");
 
    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC_STEPWISE);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);
    fann_train_on_file(ann, "low.dat", max_epochs, epochs_between_reports, desired_error);
    fann_save(ann, "low.net");
 
    fann_destroy(ann);
 
 
    // Sonuç : Eğitim dosyasını kullanarak bir sonraki değerleri tahmin et...
 
    struct fann *annhigh = fann_create_from_file("high.net");
    fp=fopen("sonuc_high.txt","w+");
    for (z=10;z<=x-1;z++)
    {
        input[0]=normHigh[z-10];
        input[1]=normHigh[z-9];
        input[2]=normHigh[z-8];
        input[3]=normHigh[z-7];
        input[4]=normHigh[z-6];
        input[5]=normHigh[z-5];
        input[6]=normHigh[z-4];
        input[7]=normHigh[z-3];
        input[8]=normHigh[z-2];
        input[9]=normHigh[z-1];
        calc_out = fann_run(annhigh, input);
        tahmin = dHigh[z-1] + ((calc_out[0]*PMax)/10000);
        fprintf(fp,"%d,%f,%f\n",z-9,dHigh[z],tahmin);
    }
    fclose(fp);
    fann_destroy(annhigh);
 
    struct fann *annlow = fann_create_from_file("low.net");
    fp=fopen("sonuc_low.txt","w+");
    for (z=10;z<=x-1;z++)
    {
        input[0]=normLow[z-10];
        input[1]=normLow[z-9];
        input[2]=normLow[z-8];
        input[3]=normLow[z-7];
        input[4]=normLow[z-6];
        input[5]=normLow[z-5];
        input[6]=normLow[z-4];
        input[7]=normLow[z-3];
        input[8]=normLow[z-2];
        input[9]=normLow[z-1];
        calc_out = fann_run(annlow, input);
        tahmin = dLow[z-1] + ((calc_out[0]*PMax)/10000);
        fprintf(fp,"%d,%f,%f\n",z-9,dLow[z],tahmin);
    }
    fclose(fp);
    fann_destroy(annlow);
 
    return 0;
}