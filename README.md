# cam-ino

<div align=center>
<img src="assets/1.avif" height=250>
<img src="assets/2.avif" height=250>
<img src="assets/4.avif" height=250>
<img src="assets/3.avif" height=250>
</div>

## Components:

- [Arduino Uno R4 Minima](https://store.arduino.cc/products/uno-r4-minima?utm_source=google&utm_medium=cpc&utm_campaign=EU-Pmax&gad_source=1&gad_campaignid=22591753150&gclid=Cj0KCQjw58PGBhCkARIsADbDilx_TAh3w0jdO3l3JqwxRJk4iB5ktNN-RDxPpkH0SYOuEU9TuWpYbtIaAiF1EALw_wcB)
- [ESP32-CAM](https://www.amazon.it/-/en/dp/B093GSCBWJ?ref=ppx_yo2ov_dt_b_fed_asin_title)
  - [OV2640](https://www.amazon.it/-/en/OV2640-Camera-Megapixel-Support-T-Camera/dp/B0BXSL76L8/ref=sr_1_7?dib=eyJ2IjoiMSJ9.qHo00YHP3aaP5UdydLFZ3rSEdK0mDMOzicrb1zak2A1sKR-OCnEZQ4MrG_nmo8PQllK4wgRuaht3xM_4fAeG9_e4MXPT5h_8Qyr9sTQAL0YDp8CG2EOYEALkXVbFdKN9cUlBR8RvH6Wboe4EptuIB2z-wTVArroGmN2TkbO_AmUuDr8dPbGdeReylzUdXfJLqApM9tb69wLkMX2FNoJHOwLMXYWesd2_6vDX2teVul3EkaJFxAnq2r8JXQIeh2OhmuKGGC05nGrMj_LACeD28PZXUVQRKjSmnIDylZfDtbc.tnwaWs29im2LnQGBAyyCNgK334bsq8v0ENxwNqygAuU&dib_tag=se&keywords=ov2640&qid=1758558941&sr=8-7)
  - [SanDisk Ultra 32GB microSDHC](https://www.amazon.it/-/en/dp/B08GY9NYRM?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1)
- [DHT sensor](https://www.amazon.it/-/en/dp/B0CK195FRJ?ref=ppx_yo2ov_dt_b_fed_asin_title)
- [HC-SR04 Ultrasonic Sensor](https://www.amazon.it/ELEGOO-HC-SR04-Ultrasonic-Distance-Sensor/dp/B01COSN7O6/ref=sr_1_2?crid=YYKUKT3DK29D&dib=eyJ2IjoiMSJ9.uHYik-fiQDBNhwQz-q2d4VYyIBahinERcRU2S7XWuS9PGxL5ws191wDtMm8E9Xc3A6t_XD3YviwvZJZ3pSo386YS4K6668ZzphHH9dCvhywfHS6mgdJkClRB7MyYteUb-KYoacXOLhsHGwPgOtVPrWRdU1vOfWW6oNshRn22noKonKJQM1jIQkGRG53Iw9Bol1lYQkN6trzILwRdtR_27PP1L7uI8hiOVxhlwSOG1AtQU1jwqPGhew9_tafTUxWyxxU95WIsQi6JO65p2KvQjnNKQOtTbu-emOyGFo-EA20.8gLoBgAfhiVJ-GIfS2EFbVrK-ZH3GporWQ9AgyhkH2E&dib_tag=se&keywords=HS-C+ultrasonic+sensor&qid=1758557098&sprefix=hs-c+ultrasonic+sensor%2Caps%2C79&sr=8-2)

## How It Works

### Object Detection

Arduino Uno manages the HC-SR04 ultrasonic sensor and the DHT sensor. The HC-SR04 sensor works by sending sound waves from the transmitter, which then bounce off of an object and then return to the receiver. One can determine how far away something is by measuring time it takes for the sound waves to get back to the sensor. The speed of sound in the air is approximately $343 m/s$ at $20\degree \text{C}$ (and $0,314 cm/\mu s$) so in order to calculate the distance of an object one can use

$$ d = (v \times duration) / 2; $$

where $v$ is the speed of sound in $cm/\mu s$ and duration is the duration of the sonic burst in $\mu s$. All divided by two because it is a round trip.
To better approximate the real distance of an object Arduino Uno manages a DHT sensor that can measure ambient humidity and temperature. While the humidity is not quite negligible for the speed of sound calculation, the temperature is quite relevant. One can adjust the speed of sound calculation (in $m/s$) in the air parametrizing it with temperature and humidity:

$$ v = 331.4 + (0.6 _ t) + (0.0124 _ h); $$

where $t$ and $v$ are temperature and humidity respectively.

### Trigger Photo Shoot

When an object is quite near (~100cm) Arduino Uno sends a 5v signal to ESP32-CAM using one of its digital pin. Since the ESP32-CAM works with 3.3v the two are connected using a [voltage divider](https://en.wikipedia.org/wiki/Voltage_divider) circuit that lowers the input for the ESP32-CAM at 3.3v. Once the ESP32-CAM receives the trigger signal it starts the capturing process. The photo is taken using a [OV2640](https://blog.arducam.com/ov2640/) camera that shoots at SVGA (800x600) resolution. ESP32-CAM generates random names for the pictures and stores them on a SanDisk Ultra 32GB microSDHC.

## 3D Model Case

The components are "organized" inside a cool 3D printed box with its own cover. The two `.stl` filese are available in this repository and can be 3D printed in a couple of hours.

- [3d-case/hunter-case.stl](3d-case/hunter-case.stl)
- [3d-case/hunter-case-cover.stl](3d-case/hunter-case-cover.stl)

The two holes at the top are for the HC-SR04 sensor, while the hole at the bottom is for the camera. Originally the camera was meant to be a [OV7670](https://futuranet.it/prodotto/ov7670-modulo-telecamera-digitale-per-arduino/?utm_source=Google+Shopping&utm_medium=cpc&utm_campaign=futuranet_gs&gad_source=1&gad_campaignid=17338545980&gclid=CjwKCAjwisnGBhAXEiwA0zEORypyMFtdpkvOQo0-dmvAMKoPEPmqwl3E5K_EAcJn--W6_fJySwDAihoCzq8QAvD_BwE) or a [TTL Serial JPEG Camera](https://www.adafruit.com/product/397) but for many reasons that was not the case.

If only I were able to paint the case would look like this.

![render](assets/hunter-case-render.avif)

## Energy Consumption

- Arduino states that Uno R4 minima [consumes on average](https://docs.arduino.cc/resources/datasheets/ABX00080-datasheet.pdf) average current consumption while powered with USB-C (5v) and running the default firmware shipped with the board is 33.39mA which is 0.16695W.
- ESP32-CAM [ranges](https://lastminuteengineers.com/getting-started-with-esp32-cam/) from 80 mA when not streaming video to around 100~160 mA when streaming video; with the flash on, it can reach 270 mA. So on average, the ESP32-CAM consumes about 120–150 mA at 3.3 V, which is roughly 0.4–0.5 W of power. Since I’m powering it via the [MB](https://it.aliexpress.com/item/1005008888498301.html?pdp_npi=4%40dis%21EUR%211.35%210.94%21%21%2111.08%217.76%21%40210173a317583828404868611eed26%2112000047088917398%21affd%21%21%21&dp=CjwKCAjwisnGBhAXEiwA0zEOR_CE4X0oQlBUEgvSb_xF0ZTw29wcMatZYvj_27kyF_33uSFVq2zynRoCxwgQAvD_BwE%7C0AAAAA_4-di9sl2vv3r1FfKTX-CHvgMEfb%7CCj8KCQjw58PGBhCtARIuAA6hE6gERz3YcHpwMx_VjtXP9o-eqLT_S5vJhnUbhKZhWVcmqmwdgVNxIuYqoRoCv84%7Cv1&cn=it_a_2&gad_source=1&aff_fcid=dd465c2760cc4f098ea2356ee25ef323-1758636388248-00137-_onKPRpM&aff_fsk=_onKPRpM&aff_platform=api-new-product-query&sk=_onKPRpM&aff_trace_key=dd465c2760cc4f098ea2356ee25ef323-1758636388248-00137-_onKPRpM&terminal_id=cbe0758e485645f8b662884cbf5d29ba&afSmartRedirect=y&gatewayAdapt=glo2ita) adapter and USB (5V), the board’s regulator steps this down to 3.3 V, so the actual current from USB will be a bit lower (80–100 mA at 5V, depending on efficiency).
- HC-SR04 [has a peak current draw](https://makersportal.com/shop/hc-sr04-ultrasonic-distance-sensor) of 15mA.
- DHT sensor consumes draws on average 1mA and at 5V it's 0.005W (negligible).

Summing up on average the Wh consumption should be

$$ (0.167W+0.45W+0.075W+0.005W) \times 1h \approx 0.70Wh $$

With a USB Power Banks

- A 10,000 mAh power bank at 5V will last ~60h runtime.
- A 5,000 mAh power bank will last ~30h runtime.
