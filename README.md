# TinyML İçin Bellek Verimli Dinamik Tensör Yapısı

Bu proje, ESP32 ve Arduino gibi kısıtlı RAM kapasitesine sahip mikrodenetleyiciler (MCU) üzerinde Makine Öğrenmesi (TinyML) modellerini çalıştırmak amacıyla geliştirilmiş, C dilinde ilkel (primitive) bir tensör veri yapısıdır.

## 📌 Proje Özeti
Yapay zeka modellerindeki standart 32-bit Float (F32) ağırlık matrisleri, gömülü sistemlerin belleğini saniyeler içinde doldurabilir. Bu sorunu çözmek için proje kapsamında, tek bir veri yapısı içinde duruma göre 32-bit Float, 16-bit Float veya 8-bit Integer (Quantized) saklayabilen esnek bir `Tensor` mimarisi tasarlanmıştır.

## 🚀 Temel Özellikler
* **Dinamik Bellek Yönetimi (Union):** Farklı veri tipleri (`f32_data`, `f16_data`, `i8_data`) C dilindeki `union` yapısı ile aynı fiziksel bellek adresini paylaşır. Bu sayede kullanılmayan tipler için ekstra RAM harcanmaz.
* **Asimetrik Kuantizasyon (Quantization):** Float32 tipindeki sürekli (continuous) ağırlık değerleri, hesaplanan `Scale` ve `Zero Point` parametreleri ile Int8 tipine dönüştürülür. Bu işlem matris başına **%75 oranında bellek tasarrufu** sağlar.
* **Güvenli Dönüşüm:** Veri dönüşümü sırasında bellek taşmalarını engellemek için Explicit Type Casting ve Clamping işlemleri uygulanmıştır.

## 🤖 Agentic Kodlama ve Geliştirme Süreci
Bu projenin mimari tasarımı ve kodlama süreci, **Agentic Workflow (Temsilci İş Akışı)** prensipleriyle yürütülmüştür:
* **Kullanılan Dil Modeli:** Gemini / Qwen
* **IDE:** [Kullandığınız IDE'yi yazın, örn: VS Code / Dev-C++]
* **Yöntem:** Dil modeli basit bir kod üretici olarak değil; bellek sızıntılarını denetleyen, `union` yapısının RAM üzerindeki kısıtlamalarını hesaba katan ve kuantizasyon matematiğini C standartlarına (C89/C99) göre optimize eden bir mühendislik partneri olarak kullanılmıştır. Kodlar doğrudan kopyalanmamış, iteratif promptlar ile mikrodenetleyici kısıtlarına göre adım adım şekillendirilmiştir.

## 📂 Proje Yapısı
```text
├── tensor.h    # Tensör veri yapısı, enum ve union tanımları
├── main.c      # Asimetrik kuantizasyon algoritması ve test demosu
└── README.md   # Proje dokümantasyonu
