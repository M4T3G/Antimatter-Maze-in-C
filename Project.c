#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <unistd.h>
#include <time.h>
#define LINESIZE 100

//Kullan�c� struct a�t�k
typedef struct{
	char isim[50];
	char soyisim[50];
	char kullaniciAdi[50];
	char sifre[50];
}User;

//En y�ksek skorlar� txt dosyas�nda tutuyorum okurken skor ve ismi par�alamak i�in struct olu�turdum
typedef struct {
    char Name[50];
    double Value;
} Data;

//Ayn� kullan�c� ad�na sahip kullan�c� var m� diye kontrol ediyorum
int kayitKontrol(char *kullaniciAdi,User user){
    FILE *fs = fopen("kullanicilar.bin", "rb");
    if (fs == NULL) {
        printf("Hic kullanici yok.\n");
        exit(0);
    }

    // Dosyadaki her kullan�c�y� kontrol et
    while (fread(&user, sizeof(user), 1, fs) == 1) {
        if (strcmp(user.kullaniciAdi, kullaniciAdi) == 0) {
            // Kullan�c� ad� zaten mevcut
            fclose(fs);
            return 1;
        }
	}
	fclose(fs);
	return 0;
}

//Kullan�c� ilk kay�t olma yeri
void kayit(User user)
{
	char kullaniciAdi[50];
	FILE *fs=fopen ("kullanicilar.bin","ab");
	if (fs==NULL)
	{
		printf("Hic kullanici yok.\n");
		exit(0);
	}
	printf("Bilgilerini veriniz.\n");
	printf("Kullanici Ismi:\n");
	scanf("%s",user.isim);	
	printf("Kullanici Soyismi:\n");
	scanf("%s",user.soyisim);
	printf("Kullanici Adi:\n");
	scanf("%s",user.kullaniciAdi);
	printf("Sifre:\n");
	scanf("%s",user.sifre);
	strcpy(kullaniciAdi,user.kullaniciAdi);
	if(kayitKontrol(kullaniciAdi,user)==1){
		printf("Bu isimde kullanici var. Kayit olamazsiniz, giris yapiniz.\n");
	}
	if(kayitKontrol(kullaniciAdi,user)==0){
		fwrite (&user,sizeof(user),1,fs);
		printf("Kullanici basariyla kayit oldu.\n");
	}		
	fclose(fs);
}

//giris kontrolu yap�yoruz kullan�c� ismi varsa 1 d�nd�r�yoruz
int girisKontrol(char *kullaniciAdi,char *sifre,User user){
    FILE *fs = fopen("kullanicilar.bin", "rb");
    if (fs == NULL) {
        printf("Kullanici yok.\n");
        exit(0);
    }

    // Dosyadaki her kullan�c�y� kontrol et
    while (fread(&user, sizeof(user), 1, fs) == 1) {
        if ((strcmp(user.kullaniciAdi, kullaniciAdi) == 0)&&(strcmp(user.sifre, sifre) == 0)) {
            // Kullan�c� ad� zaten mevcut
            fclose(fs);
            return 1;
        }
	}
	fclose(fs);
	return 0;	
}

//Kullan�c�n�n ismine ula�am�yordum.Ula�abilmek i�in fonksiyonu char* �eklinde d�nd�r�yorum.��inde  do while ile kullan�c� do�ru giri� yapamad�ysa bir daha girmesini sagl�yorum
char *giris(User user){
	int dogru=0;
	char kullaniciAdi[50],sifre[50],*isim;
	do{
		printf("Kullanici adi giriniz.\n");
		scanf("%s",kullaniciAdi);
		printf("Sifre giriniz.\n");
		scanf("%s",sifre);
		isim=(char*)malloc(strlen(kullaniciAdi)*sizeof(char));
		strcpy(isim,kullaniciAdi);
		if(girisKontrol(kullaniciAdi,sifre,user)==1){
		printf("Basariyla giris yapildi.\n");
		dogru=1;
	}
	if(girisKontrol(kullaniciAdi,sifre,user)==0){
		printf("Kullanici adi veya sifre hatali.\n");
		dogru=0;
	}
	}while(dogru==0);
	return isim;
}

//�ki say�dan hangisi k���kse onu buluyorum.Kar��t parcac�k hesaplarken i�ime yar�yor.
int min(int sayi1, int sayi2) {
    return (sayi1 < sayi2) ? sayi1 : sayi2;
}

//txt dosyas�ndan mapi okuyorum ve bunu Lab dizisinin i�ine at�yorum.
void ownMap(char filename[], char ***Lab, int *rowCount, int *colCount) {
	int i,j;
    FILE *dosya = fopen(filename, "r");

    if (dosya == NULL) {
        printf("Dosya a�ma hatasi\n");
        exit(EXIT_FAILURE);
    }

    *rowCount = 0;
    *colCount = 0;

    // Dosyadan sat�r say�s�n� ve s�tun say�s�n� bulma
    char karakter;
    while ((karakter = fgetc(dosya)) != EOF) {
        if (karakter == '\n') {
            (*rowCount)++;
            *colCount = 0; // S�tun say�s�n� s�f�rla
        } else {
            (*colCount)++;
        }
    }
    (*rowCount)++;
    //Dinamik olarak Lab dizisine sat�r s�t�n say�s� kadar yer ay�r�yorum.
    *Lab = (char **)malloc((*rowCount) * sizeof(char *));
    for ( i = 0; i < *rowCount; i++) {
        (*Lab)[i] = (char *)malloc((*colCount) * sizeof(char));
    }

    fseek(dosya, 0, SEEK_SET); 
    for (i = 0; i < *rowCount; i++) {
        for ( j = 0; j < *colCount; j++) {
            (*Lab)[i][j] = fgetc(dosya);
        }
        // Sat�r sonunu oku (newline karakterini atla)
        if(i<*rowCount-1 && karakter!='\n'){
        	fgetc(dosya);
		}       
    }

    fclose(dosya);
}

//Yukar� hamlem i�in if else ler ile olabilecek t�m ihtimalleri kodluyorum.e+ ve p- say�lar�n� bunlarla kar��la�t�k�a art�r�yorum.
//e- g�r�nce Esayisini azalt�yorum.p+ g�r�nce psayisini azalt�yorum. 
void yukari(char ***Lab, int *k, int m, int *oyun, char **madde, int *n, int *psayisi,int *Esayisi,double *skor) {
    int i, j;
    if ((*Lab)[(*k)-1][m] == '0'){
    	(*Lab)[(*k)][m] = '0';
        (*Lab)[(*k)-1][m] = 'X';
        (*k)--;
	}else{if ((*Lab)[(*k)-1][m] == 'P') {
        (*madde)[(*n)] = 'P';
        (*n)++;
        (*madde) = (char **)realloc((*madde), (*n) * sizeof(char *));
        (*Lab)[(*k)][m] = '0';
        (*Lab)[(*k)-1][m] = 'X';
        (*k)--;
    }else{if ((*Lab)[(*k)-1][m] == 'p') {
        (*madde)[(*n)] = 'p';
        (*n)++;
        (*madde) = (char **)realloc((*madde), (*n) * sizeof(char *));
        (*Lab)[(*k)][m] = '0';
        (*Lab)[(*k)-1][m] = 'X';
        (*k)--;
    }else{if ((*Lab)[(*k)-1][m] == 'e') {
        (*madde)[(*n)] = 'e';
        (*n)++;
        (*madde) = (char **)realloc((*madde), (*n) * sizeof(char *));
        (*Lab)[(*k)][m] = '0';
        (*Lab)[(*k)-1][m] = 'X';
        (*k)--;
    }else{if ((*Lab)[(*k)-1][m] == 'E') {
        (*madde)[(*n)] = 'E';
        (*n)++;
        (*madde) = (char **)realloc((*madde), (*n) * sizeof(char *));
        (*Lab)[(*k)][m] = '0';
        (*Lab)[(*k)-1][m] = 'X';
        (*k)--;
    }else{if ((*Lab)[(*k)-1][m] == 'K') {
        printf("\nKaybettiniz.\n");
        (*oyun) = 0;
    }else{if ((*Lab)[(*k)-1][m] == 'C') {
        printf("\nOyun bitti.\n");       
        for (i = 0; i < (*n); i++) {
            if((*madde)[i]=='E'){
            	(*Esayisi)++;
			}
			if((*madde)[i]=='p'){
				(*psayisi)++;
			}
			if((*madde)[i]=='P'){
				(*psayisi)--;
			}
			if((*madde)[i]=='e'){
				(*Esayisi)--;
			}
        }
        if(((*psayisi)<0)||((*Esayisi)<0)){       // e�er ikisinde birisi eksiye inerse puan�m eksi d�nmemesi ve 0 d�nmesi i�in ikisini de s�f�rl�yorum.
        	(*psayisi)=0;
        	(*Esayisi)=0;
		}
        printf("\n %d tane karsit madde uretildi.\n",min((*psayisi),(*Esayisi)));//Hangisi kucukse onun say�s� kadar kars�t madde olu�turabiliyorum.
        (*skor)=5*min((*psayisi),(*Esayisi));//Skor hesaplamas� yap�yorum.
        (*oyun) = 0;// Bu de�i�kenle oyunu bitiriyorum.
    }else{if ((*Lab)[(*k)-1][m] == 'G'){
    	(*Lab)[(*k)][m] = '0';
        (*Lab)[(*k)-1][m] = 'X';
        (*k)--;
	}
	}
	}
	}
	}
	}
	} 
}
}

//Asag� giderse hangi ihtimallerle kars�lasacag�n� tek tek kodluyorum.
void asagi(char ***Lab, int *k, int m, int *oyun, char **madde, int *n, int *psayisi,int *Esayisi,double *skor) {
    int i, j;
    if ((*Lab)[(*k)+1][m] == '0'){
    	(*Lab)[(*k)][m] = '0';
        (*Lab)[(*k)+1][m] = 'X';
        (*k)++;
	}else{if ((*Lab)[(*k)+1][m] == 'P') {
        (*madde)[(*n)] = 'P';
        (*n)++;
        (*madde) = (char **)realloc((*madde), (*n) * sizeof(char *));
        (*Lab)[(*k)][m] = '0';
        (*Lab)[(*k)+1][m] = 'X';
        (*k)++;
    }else{if ((*Lab)[(*k)+1][m] == 'p') {
        (*madde)[(*n)] = 'p';
        (*n)++;
        (*madde) = (char **)realloc((*madde), (*n) * sizeof(char *));
        (*Lab)[(*k)][m] = '0';
        (*Lab)[(*k)+1][m] = 'X';
        (*k)++;
    }else{if ((*Lab)[(*k)+1][m] == 'e') {
        (*madde)[(*n)] = 'e';
        (*n)++;
        (*madde) = (char **)realloc((*madde), (*n) * sizeof(char *));
        (*Lab)[(*k)][m] = '0';
        (*Lab)[(*k)+1][m] = 'X';
        (*k)++;
    }else{if ((*Lab)[(*k)+1][m] == 'E') {
        (*madde)[(*n)] = 'E';
        (*n)++;
        (*madde) = (char **)realloc((*madde), (*n) * sizeof(char *));
        (*Lab)[(*k)][m] = '0';
        (*Lab)[(*k)+1][m] = 'X';
        (*k)++;
    }else{if ((*Lab)[(*k)+1][m] == 'K') {
        printf("\nKaybettiniz.\n");
        (*oyun) = 0;
    }else{if ((*Lab)[(*k)+1][m] == 'C') {
        printf("\nOyun bitti.\n");        
        for (i = 0; i < (*n); i++) {
            if((*madde)[i]=='E'){
            	(*Esayisi)++;
			}
			if((*madde)[i]=='p'){
				(*psayisi)++;
			}
			if((*madde)[i]=='P'){
				(*psayisi)--;
			}
			if((*madde)[i]=='e'){
				(*Esayisi)--;
			}
        }     
        if(((*psayisi)<0)||((*Esayisi)<0)){
        	(*psayisi)=0;
        	(*Esayisi)=0;
		}
		printf("\n %d tane karsit madde uretildi.\n",min((*psayisi),(*Esayisi)));
        (*skor)=5*min((*psayisi),(*Esayisi));
        (*oyun) = 0;
    }else{if ((*Lab)[(*k)+1][m] == 'G'){
    	(*Lab)[(*k)][m] = '0';
        (*Lab)[(*k)+1][m] = 'X';
        (*k)++;
	}
	}
	}
	}
	}
	}
	}   
}
}

//Saga giderse hangi ihtimallerle kars�lasacag�n� tek tek kodluyorum.
void sag(char ***Lab, int k, int *m, int *oyun, char **madde, int *n, int *psayisi,int *Esayisi,double *skor) {
    int i, j;
    if((*Lab)[k][(*m) + 1] == '0'){
    	(*Lab)[k][(*m)] = '0';
        (*Lab)[k][(*m) + 1] = 'X';
        (*m)++;
	}else{if ((*Lab)[k][(*m) + 1] == 'P') {
        (*madde)[(*n)] = 'P';
        (*n)++;
        (*madde) = (char **)realloc((*madde), (*n) * sizeof(char *));
        (*Lab)[k][(*m)] = '0';
        (*Lab)[k][(*m) + 1] = 'X';
        (*m)++;
    }else{if ((*Lab)[k][(*m) + 1] == 'p') {
        (*madde)[(*n)] = 'p';
        (*n)++;
        (*madde) = (char **)realloc((*madde), (*n) * sizeof(char *));
        (*Lab)[k][(*m)] = '0';
        (*Lab)[k][(*m) + 1] = 'X';
        (*m)++;
    }else{if ((*Lab)[k][(*m) + 1] == 'e') {
        (*madde)[(*n)] = 'e';
        (*n)++;
        (*madde) = (char **)realloc((*madde), (*n) * sizeof(char *));
        (*Lab)[k][(*m)] = '0';
        (*Lab)[k][(*m) + 1] = 'X';
        (*m)++;
    }else{if ((*Lab)[k][(*m) + 1] == 'E') {
        (*madde)[(*n)] = 'E';
        (*n)++;
        (*madde) = (char **)realloc((*madde), (*n) * sizeof(char *));
        (*Lab)[k][(*m)] = '0';
        (*Lab)[k][(*m) + 1] = 'X';
        (*m)++;
    }else{if ((*Lab)[k][(*m) + 1] == 'K') {
        printf("\nKaybettiniz.\n");
        (*oyun) = 0;
    }else{if ((*Lab)[k][(*m) + 1] == 'C') {
        printf("\nOyun bitti.\n");        
        for (i = 0; i < (*n); i++) {
            if((*madde)[i]=='E'){
            	(*Esayisi)++;
			}
			if((*madde)[i]=='p'){
				(*psayisi)++;
			}
			if((*madde)[i]=='P'){
				(*psayisi)--;
			}
			if((*madde)[i]=='e'){
				(*Esayisi)--;
			}
        }
        if(((*psayisi)<0)||((*Esayisi)<0)){
        	(*psayisi)=0;
        	(*Esayisi)=0;
		}
        printf("\n %d tane karsit madde uretildi.\n",min((*psayisi),(*Esayisi)));
        (*skor)=5*min((*psayisi),(*Esayisi));
        (*oyun) = 0;
    }else{if((*Lab)[k][(*m) + 1] == 'G'){
    	(*Lab)[k][(*m)] = '0';
        (*Lab)[k][(*m) + 1] = 'X';
        (*m)++;
	}
	}
	}
	}
	}
	}
	} 
}
}

//Sola giderse hangi ihtimallerle kars�lasacag�n� tek tek kodluyorum.
void sol(char ***Lab, int k, int *m, int *oyun, char **madde, int *n, int *psayisi,int *Esayisi,double *skor) {
    int i, j;
    if((*Lab)[k][(*m) - 1] == '0'){
    	(*Lab)[k][(*m)] = '0';
        (*Lab)[k][(*m) - 1] = 'X';
        (*m)--;
	}else{if ((*Lab)[k][(*m) - 1] == 'P') {
        (*madde)[(*n)] = 'P';
        (*n)++;
        (*madde) = (char **)realloc((*madde), (*n) * sizeof(char *));
        (*Lab)[k][(*m)] = '0';
        (*Lab)[k][(*m) - 1] = 'X';
        (*m)--;
    }else{if ((*Lab)[k][(*m) - 1] == 'p') {
        (*madde)[(*n)] = 'p';
        (*n)++;
        (*madde) = (char **)realloc((*madde), (*n) * sizeof(char *));
        (*Lab)[k][(*m)] = '0';
        (*Lab)[k][(*m) - 1] = 'X';
        (*m)--;
    }else{if ((*Lab)[k][(*m) - 1] == 'e') {
        (*madde)[(*n)] = 'e';
        (*n)++;
        (*madde) = (char **)realloc((*madde), (*n) * sizeof(char *));
        (*Lab)[k][(*m)] = '0';
        (*Lab)[k][(*m) - 1] = 'X';
        (*m)--;
    }else{if ((*Lab)[k][(*m) - 1] == 'E') {
        (*madde)[(*n)] = 'E';
        (*n)++;
        (*madde) = (char **)realloc((*madde), (*n) * sizeof(char *));
        (*Lab)[k][(*m)] = '0';
        (*Lab)[k][(*m) - 1] = 'X';
        (*m)--;
    }else{if ((*Lab)[k][(*m) - 1] == 'K') {
        printf("\nKaybettiniz.\n");
        (*oyun) = 0;
    }else{if ((*Lab)[k][(*m) - 1] == 'C') {
        printf("\nOyun bitti.\n");        
        for (i = 0; i < (*n); i++) {
            if((*madde)[i]=='E'){
            	(*Esayisi)++;
			}
			if((*madde)[i]=='p'){
				(*psayisi)++;
			}
			if((*madde)[i]=='P'){
				(*psayisi)--;
			}
			if((*madde)[i]=='e'){
				(*Esayisi)--;
			}
        }
        if(((*psayisi)<0)||((*Esayisi)<0)){
        	(*psayisi)=0;
        	(*Esayisi)=0;
		}
        printf("\n %d tane karsit madde uretildi.\n",min((*psayisi),(*Esayisi)));
        (*skor)=5*min((*psayisi),(*Esayisi));
        (*oyun) = 0;
    }else{if((*Lab)[k][(*m) - 1] == 'G'){
    	(*Lab)[k][(*m)] = '0';
        (*Lab)[k][(*m) - 1] = 'X';
        (*m)--;
	}
	}
	}
	}
	}
	}
	}
	}
}

void oyun(char filename[], char **Lab, int rowCount, int colCount,User user,char *isim){
	time_t start ,current;
	int i,j,k,m,a,b,n=0,oyun=1,psayisi=0,Esayisi=0,secenek,secenek2;
	char tus;
	float kullanilanSure,kalanSure,toplamSure=120;
	char *madde=NULL;
	double skor=0;
	madde=(char *)malloc(sizeof(char));//Elimizdeki maddeleri tutmak i�in tan�ml�yoruz ve yer a��yoruz.
	do{
		printf("Hazir haritalardan oynamak icin 1,kendi haritanizi yuklemek icin 2'ye basin.\n");
		scanf("%d",&secenek);
	}while((secenek!=1)&&(secenek!=2));
	if(secenek==1){
		do{
			printf("Hangi haritayi oynamak istiyorsunuz?\n");
			printf("1.harita\n");
			printf("2.harita\n");
			printf("3.harita\n");
			scanf("%d",&secenek2);
		}while((secenek2!=1)&&(secenek2!=2)&&(secenek2!=3));
		if(secenek2==1){
			ownMap("1.txt", &Lab, &rowCount, &colCount);
		}
		if(secenek2==2){
			ownMap("2.txt", &Lab, &rowCount, &colCount);
		}
		if(secenek2==3){
			ownMap("3.txt", &Lab, &rowCount, &colCount);
		}
	}
	if(secenek==2){
		printf("Haritanizi aciniz.\n");
    	printf("dosya ismi veriniz\n");
		scanf( "%s", filename );
		ownMap(filename, &Lab, &rowCount, &colCount);
	}	
	//kullanicinin ba�lad��� nokta yani x'i bulunuyor.k ve m ad�m�m�za g�re x'in g�ncel konumunu g�steriyo.a ve b ise ilk baslad�g�m�z yani G olacak yeri gosteriyo.
    for(i=0;i<rowCount;i++){
    	for(j=0;j<colCount;j++){
    		if(Lab[i][j]=='X'){
    			k=i;
    			m=j;
    			a=i;
    			b=j;
			}
		}
	}
	system("cls");
	matrisYazdir(Lab, rowCount, colCount);
	start=time(NULL);
    do{
    	printf("Hangi yone gitmek istiyorsunuz?\n");
    	getch();         //bu kodu yazmazsam yon u�lar�na bas�nca ilk hamle yapmam�s gibi matris yazd�r�yo.Sonra dogrusunu yazd�r�yo
    	tus = getch();   //san�r�m �stteki koddan dolay� esc'yi tek basmada alg�lam�yor 2 kere basmak gerek
    	if(tus==72){
    		yukari(&Lab,&k,m,&oyun,&madde,&n,&psayisi,&Esayisi,&skor);
		}
		if(tus==80){
			asagi(&Lab,&k,m,&oyun,&madde,&n,&psayisi,&Esayisi,&skor);
		}
		if(tus==77){
			sag(&Lab,k,&m,&oyun,&madde,&n,&psayisi,&Esayisi,&skor);
		}
		if(tus==75){
			sol(&Lab,k,&m,&oyun,&madde,&n,&psayisi,&Esayisi,&skor);
		}
		sleep(1); 
		system("cls");
		if((a!=k)||(b!=m)){
			Lab[a][b]='G';
		} //�lk baslad�g�m�z konuma G yani giri� sembolunu koyuyoruz her hamleden sonra
		matrisYazdir(Lab, rowCount, colCount);
		printf("\nTopladiginiz Maddeleriniz:\n");
		for(i=0;i<n;i++){
			printf("%c ",madde[i]);
		}
		printf("\n");		
		current=time(NULL);// her ad�mdan sonra zaman�m�z� guncelliyoruz
		kalanSure=toplamSure-difftime(current,start);	//Kalan sure hesapl�yoruz
		printf("\nKalan sure:%f\n",kalanSure);	
	}while((tus!=27)&&(oyun!=0)&&(difftime(current,start) <toplamSure));
	if(tus==27){//ESC ile ��k�s yap�lma durumunda oluyor.
		printf("\nOyundan cikis yapildi.\n");
		printf("Puan�n�z:0\n");
	}
	if(kalanSure<=0){
		printf("\nZamaniniz bitti.\n");
		printf("Puan�n�z:0\n");
	}else{
		skor*=kalanSure;
	}
	kullanilanSure=difftime(current,start);
	printf("\n %d tane karsit madde uretildi.\n",min(psayisi,Esayisi));
	printf("\nBitirme sureniz:%f\n",kullanilanSure);
	printf("\nPuaniniz:%lf\n",skor);
	yuksekSkor(isim,skor);
	matrisFree(Lab,rowCount,madde);
}

//Elimizdeki skorlar� bir txt dosyas�na bast�rd�k.Ve s�rala fonksiyonumuzu �ag�rd�k.
//Dosyay� oyun bittikten sonra elle kapat�nca  skorlar dogru kaydedililiyor ama menude geri gelince her geri gelince ayn� veriyi bir daha yaz�yor.
void yuksekSkor(char *isim,double skor){
	char buf[255];
	Data skorlar[100];
	FILE *fs=fopen ("skorlar.txt","r");
	if (fs==NULL)
	{
		FILE *fs=fopen ("skorlar.txt","w");
		fprintf(fs,"%s:%lf\n",isim,skor);//Eger elimde hic skor yoksa boyle bir dosya ac�yor ve ilk isim ve skoru yaz�yor
		fclose(fs);
	}else{
		fs=fopen ("skorlar.txt","a");				
		fprintf(fs,"%s:%lf\n",isim,skor); //elime gelen bundan sonraki verileri de dosyaya yaz�yor.
		fclose(fs);
		sirala(skorlar);
	}	
}

//Burada txt dosyam�z� a��p t�m skorlar� okuduk.Bunlar� struct dizisi i�inde isim ve deger olarak tuttuk
//Alt�nda bular� dosyada buyukten kucuge dogru s�ralad�k. S�ralanm�� halini struct dizimizden �ekerek yeniden dosyaya yazd�rd�k.
void sirala(Data data[]) {
    FILE *fp = fopen("skorlar.txt", "r");
    char buf[255], tmp[50];
    int k = 0, i, j;
    double temp;
    while (fgets(buf, 255, fp) != NULL) {
        parcala(&data[k], buf, ":\0"); //Burada dosyaya isim:skor olarak kaydedilen verileri parcalad�k.Ve data struct i�ine att�k
        k++;
    }
    for (i = 0; i < k; i++) {
    	for (j = i + 1; j < k; j++) {
    		int a=data[i].Value < data[j].Value;  //direkt bu kar��la�t�rmay� if i�ine at��g�mda �al��m�yordu.B�yle ��z�m bulabildim.
       		 if (a) {
        	  	temp = data[i].Value;
        	    data[i].Value = data[j].Value;
        	    data[j].Value = temp;
				strcpy(tmp, data[i].Name);
            	strcpy(data[i].Name, data[j].Name);
            	strcpy(data[j].Name, tmp);
        	}
    	}
	}
    fclose(fp);
    fp = fopen("skorlar.txt", "w");
    fseek(fp, 0, SEEK_SET);
    for (i = 0; i < k+1; i++) {
        fprintf(fp, "%s:%lf\n", data[i].Name, data[i].Value);
    }
    fclose(fp);
}

//Burada dosyaya isim:skor olarak kaydedilen verileri parcalad�k. 
void parcala(Data *holder, char buf[255], char delimiter[2]) {
    char *token;

    token = strtok(buf, delimiter);
    strcpy(holder->Name, token);
    token = strtok(NULL, delimiter);
    holder->Value = atof(token);
}

// Matrisi ekrana yazd�rma
void matrisYazdir(char **Lab, int rowCount, int colCount){
	int i,j;	
    for (i = 0; i < rowCount; i++) {
        for ( j = 0; j < colCount; j++) {
            printf("%c ", Lab[i][j]);
        }
        printf("\n");
    }
}

//En yuksek 5 skoru ekrana yazd�rma
void dereceler(){
	FILE *dosya = fopen("skorlar.txt", "r");
	if (dosya == NULL) {
        printf("Hic skor yok.\n");
        exit(0);
    }
    char isim[50];
    double skor;
    int satirSayisi=0;
    while (satirSayisi < 5 && (fscanf(dosya, "%[^:]:%lf\n", isim, &skor) == 2)) {
    	if(skor!=0){
    		printf("Isim: %s, Skor: %.2lf\n", isim, skor);
		}   
        satirSayisi++;
    }
    fclose(dosya);
}

//dinamik olarak a�t���m�z dizileri free ettik
void matrisFree(char**Lab,int rowCount,char *madde){
	int i;
	for(i=0;i<rowCount;i++){
		free(Lab[i]);
	}
	free(Lab);
	free(madde);
}

void anaMenu(User user,int rowCount,int colCount,char filename[],char **Lab,char *isim) {
    int choice;
    sleep(1);system("cls");
    while (1) {
        printf("\n Ana Menu:\n");
        printf("1. Oyunun en yuksek skorlari ve sahipleri\n");
        printf("2. Oyun kurallari \n");
        printf("3. Oyuna basla\n");
        printf("0. Onceki Menuye Don\n");
        
        printf("Bir secenek girin: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
            	system("cls");
                printf("En Yuksek Skorlar\n");
                dereceler();
                sleep(10);
                system("cls");
                break;
            case 2:
            	system("cls");
                printf("Oyun Kurallari.\n");
                printf("Puaniniz 5*Urettiginiz Karsit Madde Sayisi*Kalan Zamaniniza gore hesaplanmaktadir.\n");
                printf("P+ 	: proton\ne-	: elektron\nP-	: karsit proton\ne+	: karsit elektron\n");
				printf("Karsit hidrojen uretebilmek icin labirentin cikisinda elinizde sadece P- ve e+ parcaciklari bulunmalidir.\n");
				printf("Bir parcacikla o parcacigin zit isaretli karsit parcacigi bir araya gelirse birbirini yok eder.\n");
				printf("Karadelikler K ile gosterilmistir. Karadeliklerin bulundugu hucrelerden gecildi�i takdirde oyun sonlanir ve 0 puan alirsiniz.\n");
				printf("2 dakika icinde kullanici cikisa ulasamazsa oyun sonlanir ve 0 puan alirsiniz.\n");
				printf("Labirentte gezinmek icin ok tuslari kullanilacaktir.Cikis icin esc 2 kere basiniz.");
				printf("Labirentin elemanlari asagidaki gibi gosterilmektedir.\n");
				printf("P+	:P		e-	: e		P-	: p		e+	: E	\n");
				printf("Kara Delik	: K		Kullanici	: X		Cikis	: C		Yol	: 0	\n");
				printf("Duvar		: 1		Giris	: G\n");
				sleep(10);
				system("cls");
                break;
            case 3:
            	system("cls");
                printf("Oyuna baslaniyor.\n");
                oyun(filename, Lab, rowCount, colCount,user,isim);
                sleep(10);
                system("cls");
                break;
            case 0:
                printf("Onceki menuye donuluyor...\n");
                return;
            default:
                printf("Gecersiz secenek! Lutfen tekrar deneyin.\n");
                break;
        }
        //sleep(1);system("cls");
    }
}

int main(){	
 	char filename[50];
    char **Lab;
    int rowCount, colCount,secenek;  
	User user;
	char *isim;
    while (1) {
        printf("Hosgeldiniz\n");
        printf("1. Kayit Ol\n");
        printf("2. Giris Yap\n");
        printf("0. Cikis\n");
    
        scanf("%d", &secenek);
        switch (secenek) {
            case 1:
                kayit(user);
                break;
            case 2:
                	isim=giris(user);
                    anaMenu(user,rowCount,colCount,filename,Lab,isim);
                break;
            case 0:
                printf("Programdan cikiliyor...\n");
                return 0;
            default:
                printf("Gecersiz secenek! Lutfen tekrar deneyin.\n");
                break;
        }
        //sleep(1);system("cls");
    }

	  
	return 0;
}
