# forex_neural
Short term forex - EurUsd prediction using fann lib. @2008

Forex EurUsd tahminleri yapmak için Fann kütüphanesi kullanan yapayzeka - yapay sinir ağları örneği.

Ref:
https://web.archive.org/web/20080927211820/http://www.globalekonomi.com/fvolkany/yapay-zeka-kullanarak-eurusd-parite-tahmini-yapan-programin-egitim-yapan-bolumunun-kaynak-kodlari/


Her ne kadar program içinde belli oluyorsa da kısaca açıklamak gerekirse…

1-) .csv dosyadan High ve Low değerlerini alıp normalleştirdik.
2-) FANN kütüphanesinin kullanacağı 10 giriş ve 1 çıkışlık dosyayı ( .dat ) oluşturduk.
3-) High değerlerini hesaplamak için FANN_GAUSSIAN FANN_COS_SYMMETRIC ikilisini,
4-) Low değerlerini hesaplamak için de FANN_SIGMOID_SYMMETRIC_STEPWISE FANN_SIGMOID_SYMMETRIC ikilisini kullanarak yapay zekayı eğittik.
5-) Eğitim verilerini daha sonra kullanmak üzere .net uzantılı dosyalara kaydettik.

Aslında programımız şu hali ile bile tahminde bulunabiliyor. Zaten diğer bölüm de bundan farklı değil. .txt dosyaları üreten son kısmını incelerseniz bir sonraki yazıyı beklemenize gerek kalmadan kendi programınızı hazırlayabilirsiniz.

Merakımızı gidermesi açısından, gerçek veriler ile tahmin verilerini karşılaştıracağımız sonuc_high.txt ve sonuc_low.txt dosyalarını inceleyebilir, gretl gibi istatistik programlarında işleyebilir, grafik üzerinde gerçek veri ile tahmin arasındaki ilişkiyi görebiliriz.

Fakat bu tür tahmin üreteçlerinde genellikle eğitim için kullanılan geçmiş veriler toplam verinin % 70′ini oluşturuyor. Kalan %30′luk kısım hiç test edilmiyor.

Şöyle ki; Elimizde günlük O,H,L,C olarak 5 yıllık geçmiş veri olduğunu düşünelim. 3500 günlük veriyi programda eğitiyoruz. Elimizde mevcut veriler ve tahminler var.
Eğer sonuçları beğenirsek, diğer 1500 günlük veriyi kullanarak programın ürettiği tahminlere bakıyoruz ki;
Program eğitim boyunca hiç görmediği veriler karşısında doğru tahminlerde bulunabiliyor mu? Ürettiği tahminlerin karşılığı nedir? Öğreniyoruz.
Bu %30′luk kısımda da başarılı olduysa, programımızın gerçek alım satımlar yapmaya hazır olduğunu düşünebiliriz.

Örnek:
1. Test :
MetaTrader .csv dosyasındaki O,H,L,C verilerinden 2003 - 2007 yılları arasındakiler eğitim için, 2007 - 2008 arasındakiler de test için kullanılabilir.
2. Test :
İlk testte de başarılı olduğu düşünülüyorsa;
2003 - 2008 arası eğitim ve 2008 den bu güne kadar olan verilerle son testleri yapılır.
Doğal olarak 2. Testin sonuçları ilkinden daha tutarlı olmalıdır.

Bundan sonraki bölümde; Eğittiğimiz yapay zekayı daha önce görmediği veriler ile test eden programın detayları var. Bakalım yapay zekamız ne kadar öğrenebilmiş?

Not: Kodlamada fonksiyonlar kullanılarak kısaltmalar, dosya - tarih seçimi gibi kullanıcı etkileşimi, ucundan kıyısından optimizasyon yapılabilir.
